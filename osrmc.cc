#include <cassert>
#include <utility>

#include <osrm/osrm.hpp>
#include <osrm/status.hpp>
#include <osrm/coordinate.hpp>
#include <osrm/engine_config.hpp>
#include <osrm/storage_config.hpp>
#include <osrm/json_container.hpp>
#include <osrm/route_parameters.hpp>

#include "osrmc.h"

/* ABI stability */

unsigned osrmc_get_version(void) { return OSRMC_VERSION; }
bool osrmc_is_abi_compatible(void) { return osrmc_get_version() >> 16u == OSRMC_VERSION_MAJOR; }

/* API */

osrmc_config_t osrmc_config_construct(const char* base_path) try {
  auto* out = new osrm::EngineConfig;

  out->storage_config = osrm::StorageConfig(base_path);
  out->use_shared_memory = false;

  return reinterpret_cast<osrmc_config_t>(out);

} catch (...) {
  return nullptr;
}

void osrmc_config_destruct(osrmc_config_t config) { delete reinterpret_cast<osrm::EngineConfig*>(config); }

osrmc_osrm_t osrmc_osrm_construct(osrmc_config_t config) try {
  auto* config_typed = reinterpret_cast<osrm::EngineConfig*>(config);
  auto* out = new osrm::OSRM(*config_typed);

  return reinterpret_cast<osrmc_osrm_t>(out);

} catch (...) {
  return nullptr;
}

void osrmc_osrm_destruct(osrmc_osrm_t osrm) { delete reinterpret_cast<osrm::OSRM*>(osrm); }

osrmc_route_params_t osrmc_route_params_construct(void) try {
  auto* out = new osrm::RouteParameters;

  out->steps = false;
  out->alternatives = false;
  out->overview = osrm::RouteParameters::OverviewType::False;

  return reinterpret_cast<osrmc_route_params_t>(out);

} catch (...) {
  return nullptr;
}

void osrmc_route_params_destruct(osrmc_route_params_t params) {
  delete reinterpret_cast<osrm::RouteParameters*>(params);
}

void osrmc_route_params_add_coordinate(osrmc_route_params_t params, float longitude, float latitude) try {
  auto* params_typed = reinterpret_cast<osrm::RouteParameters*>(params);

  auto longitude_typed = osrm::util::FloatLongitude(longitude);
  auto latitude_typed = osrm::util::FloatLatitude(latitude);

  params_typed->coordinates.emplace_back(std::move(longitude_typed), std::move(latitude_typed));

} catch (...) {
  assert(false);
}

osrmc_route_response_t osrmc_route(osrmc_osrm_t osrm, osrmc_route_params_t params) try {
  auto* osrm_typed = reinterpret_cast<osrm::OSRM*>(osrm);
  auto* params_typed = reinterpret_cast<osrm::RouteParameters*>(params);

  auto* out = new osrm::json::Object;

  const auto status = osrm_typed->Route(*params_typed, *out);

  if (status == osrm::Status::Ok)
    return reinterpret_cast<osrmc_route_response_t>(out);
  else
    return nullptr;

} catch (...) {
  return nullptr;
}

void osrmc_route_response_destruct(osrmc_route_response_t response) {
  delete reinterpret_cast<osrm::json::Object*>(response);
}

float osrmc_route_response_distance(osrmc_route_response_t response) try {
  auto* response_typed = reinterpret_cast<osrm::json::Object*>(response);

  auto& routes = response_typed->values["routes"].get<osrm::json::Array>();
  auto& route = routes.values.at(0).get<osrm::json::Object>();

  const auto distance = route.values["distance"].get<osrm::json::Number>().value;

  return distance;

} catch (...) {
  assert(false);
  return -1;
}

float osrmc_route_response_duration(osrmc_route_response_t response) try {
  auto* response_typed = reinterpret_cast<osrm::json::Object*>(response);

  auto& routes = response_typed->values["routes"].get<osrm::json::Array>();
  auto& route = routes.values.at(0).get<osrm::json::Object>();

  const auto duration = route.values["duration"].get<osrm::json::Number>().value;

  return duration;

} catch (...) {
  assert(false);
  return -1;
}

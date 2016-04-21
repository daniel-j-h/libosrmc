#include <cassert>
#include <cmath>
#include <utility>
#include <string>
#include <stdexcept>

#include <osrm/coordinate.hpp>
#include <osrm/engine_config.hpp>
#include <osrm/json_container.hpp>
#include <osrm/osrm.hpp>
#include <osrm/route_parameters.hpp>
#include <osrm/table_parameters.hpp>
#include <osrm/nearest_parameters.hpp>
#include <osrm/match_parameters.hpp>
#include <osrm/status.hpp>
#include <osrm/storage_config.hpp>

#include "osrmc.h"

/* ABI stability */

unsigned osrmc_get_version(void) { return OSRMC_VERSION; }

int osrmc_is_abi_compatible(void) { return osrmc_get_version() >> 16u == OSRMC_VERSION_MAJOR; }

/* API */

struct osrmc_error final {
  std::string message;
};

const char* osrmc_error_message(osrmc_error_t error) { return error->message.c_str(); }

void osrmc_error_destruct(osrmc_error_t error) { delete error; }

osrmc_config_t osrmc_config_construct(const char* base_path, osrmc_error_t* error) try {
  auto* out = new osrm::EngineConfig;

  out->storage_config = osrm::StorageConfig(base_path);
  out->use_shared_memory = false;

  return reinterpret_cast<osrmc_config_t>(out);
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
  return nullptr;
}

void osrmc_config_destruct(osrmc_config_t config) { delete reinterpret_cast<osrm::EngineConfig*>(config); }

osrmc_osrm_t osrmc_osrm_construct(osrmc_config_t config, osrmc_error_t* error) try {
  auto* config_typed = reinterpret_cast<osrm::EngineConfig*>(config);
  auto* out = new osrm::OSRM(*config_typed);

  return reinterpret_cast<osrmc_osrm_t>(out);
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
  return nullptr;
}

void osrmc_osrm_destruct(osrmc_osrm_t osrm) { delete reinterpret_cast<osrm::OSRM*>(osrm); }

void osrmc_params_add_coordinate(osrmc_params_t params, float longitude, float latitude, osrmc_error_t* error) try {
  auto* params_typed = reinterpret_cast<osrm::engine::api::BaseParameters*>(params);

  auto longitude_typed = osrm::util::FloatLongitude(longitude);
  auto latitude_typed = osrm::util::FloatLatitude(latitude);

  params_typed->coordinates.emplace_back(std::move(longitude_typed), std::move(latitude_typed));
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
}

void osrmc_params_add_coordinate_with(osrmc_params_t params, float longitude, float latitude, float radius, int bearing,
                                      int range, osrmc_error_t* error) try {
  auto* params_typed = reinterpret_cast<osrm::engine::api::BaseParameters*>(params);

  auto longitude_typed = osrm::util::FloatLongitude(longitude);
  auto latitude_typed = osrm::util::FloatLatitude(latitude);

  osrm::engine::Bearing bearing_typed{static_cast<short>(bearing), static_cast<short>(range)};

  params_typed->coordinates.emplace_back(std::move(longitude_typed), std::move(latitude_typed));
  params_typed->radiuses.emplace_back(radius);
  params_typed->bearings.emplace_back(std::move(bearing_typed));
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
}

osrmc_route_params_t osrmc_route_params_construct(osrmc_error_t* error) try {
  auto* out = new osrm::RouteParameters;

  return reinterpret_cast<osrmc_route_params_t>(out);
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
  return nullptr;
}

void osrmc_route_params_destruct(osrmc_route_params_t params) {
  delete reinterpret_cast<osrm::RouteParameters*>(params);
}

void osrmc_route_params_add_steps(osrmc_route_params_t params, int on) {
  auto* params_typed = reinterpret_cast<osrm::RouteParameters*>(params);
  params_typed->steps = on;
}

void osrmc_route_params_add_alternatives(osrmc_route_params_t params, int on) {
  auto* params_typed = reinterpret_cast<osrm::RouteParameters*>(params);
  params_typed->alternatives = on;
}

osrmc_route_response_t osrmc_route(osrmc_osrm_t osrm, osrmc_route_params_t params, osrmc_error_t* error) try {
  auto* osrm_typed = reinterpret_cast<osrm::OSRM*>(osrm);
  auto* params_typed = reinterpret_cast<osrm::RouteParameters*>(params);

  auto* out = new osrm::json::Object;
  const auto status = osrm_typed->Route(*params_typed, *out);

  if (status == osrm::Status::Ok)
    return reinterpret_cast<osrmc_route_response_t>(out);

  *error = new osrmc_error{"service request failed"};
  return nullptr;
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
  return nullptr;
}

void osrmc_route_response_destruct(osrmc_route_response_t response) {
  delete reinterpret_cast<osrm::json::Object*>(response);
}

float osrmc_route_response_distance(osrmc_route_response_t response, osrmc_error_t* error) try {
  auto* response_typed = reinterpret_cast<osrm::json::Object*>(response);

  auto& routes = response_typed->values["routes"].get<osrm::json::Array>();
  auto& route = routes.values.at(0).get<osrm::json::Object>();

  const auto distance = route.values["distance"].get<osrm::json::Number>().value;
  return distance;
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
  return INFINITY;
}

float osrmc_route_response_duration(osrmc_route_response_t response, osrmc_error_t* error) try {
  auto* response_typed = reinterpret_cast<osrm::json::Object*>(response);

  auto& routes = response_typed->values["routes"].get<osrm::json::Array>();
  auto& route = routes.values.at(0).get<osrm::json::Object>();

  const auto duration = route.values["duration"].get<osrm::json::Number>().value;
  return duration;
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
  return INFINITY;
}

osrmc_table_params_t osrmc_table_params_construct(osrmc_error_t* error) try {
  auto* out = new osrm::TableParameters;
  return reinterpret_cast<osrmc_table_params_t>(out);
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
  return nullptr;
}

void osrmc_table_params_destruct(osrmc_table_params_t params) {
  delete reinterpret_cast<osrm::TableParameters*>(params);
}

void osrmc_table_params_add_source(osrmc_table_params_t params, size_t index, osrmc_error_t* error) try {
  auto* params_typed = reinterpret_cast<osrm::TableParameters*>(params);
  params_typed->sources.emplace_back(index);
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
}

void osrmc_table_params_add_destination(osrmc_table_params_t params, size_t index, osrmc_error_t* error) try {
  auto* params_typed = reinterpret_cast<osrm::TableParameters*>(params);
  params_typed->destinations.emplace_back(index);
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
}

osrmc_table_response_t osrmc_table(osrmc_osrm_t osrm, osrmc_table_params_t params, osrmc_error_t* error) try {
  auto* osrm_typed = reinterpret_cast<osrm::OSRM*>(osrm);
  auto* params_typed = reinterpret_cast<osrm::TableParameters*>(params);

  auto* out = new osrm::json::Object;
  const auto status = osrm_typed->Table(*params_typed, *out);

  if (status == osrm::Status::Ok)
    return reinterpret_cast<osrmc_table_response_t>(out);

  *error = new osrmc_error{"service request failed"};
  return nullptr;
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
  return nullptr;
}

void osrmc_table_response_destruct(osrmc_table_response_t response) {
  delete reinterpret_cast<osrm::json::Object*>(response);
}

float osrmc_table_response_duration(osrmc_table_response_t response, unsigned long from, unsigned long to,
                                    osrmc_error_t* error) try {
  auto* response_typed = reinterpret_cast<osrm::json::Object*>(response);

  auto& durations = response_typed->values["durations"].get<osrm::json::Array>();
  auto& durations_from_to_all = durations.values.at(from).get<osrm::json::Array>();
  auto duration = durations_from_to_all.values.at(to).get<osrm::json::Number>().value;

  return duration;
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
  return INFINITY;
}

osrmc_nearest_params_t osrmc_nearest_params_construct(osrmc_error_t* error) try {
  auto* out = new osrm::NearestParameters;
  return reinterpret_cast<osrmc_nearest_params_t>(out);
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
  return nullptr;
}

void osrmc_nearest_params_destruct(osrmc_nearest_params_t params) {
  delete reinterpret_cast<osrm::NearestParameters*>(params);
}

osrmc_match_params_t osrmc_match_params_construct(osrmc_error_t* error) try {
  auto* out = new osrm::MatchParameters;
  return reinterpret_cast<osrmc_match_params_t>(out);
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
  return nullptr;
}

void osrmc_match_params_destruct(osrmc_match_params_t params) {
  delete reinterpret_cast<osrm::MatchParameters*>(params);
}

void osrmc_nearest_set_number_of_results(osrmc_nearest_params_t params, unsigned n) {
  auto* params_typed = reinterpret_cast<osrm::NearestParameters*>(params);
  params_typed->number_of_results = n;
}

void osrmc_match_params_add_timestamp(osrmc_match_params_t params, unsigned timestamp, osrmc_error_t* error) try {
  auto* params_typed = reinterpret_cast<osrm::MatchParameters*>(params);
  params_typed->timestamps.emplace_back(timestamp);
} catch (const std::exception& e) {
  *error = new osrmc_error{e.what()};
}

#ifndef OSRM_C_H_
#define OSRM_C_H_

#include "visibility.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ABI Stability */

#define OSRMC_VERSION_MAJOR 1
#define OSRMC_VERSION_MINOR 0
#define OSRMC_VERSION ((OSRMC_VERSION_MAJOR << 16) | OSRMC_VERSION_MINOR)

OSRMC_API unsigned osrmc_get_version(void);
OSRMC_API int osrmc_is_abi_compatible(void);

/* Opaque Types */

typedef struct osrmc_config* osrmc_config_t;
typedef struct osrmc_osrm* osrmc_osrm_t;

typedef struct osrmc_params* osrmc_params_t;

typedef struct osrmc_route_params* osrmc_route_params_t;
typedef struct osrmc_route_response* osrmc_route_response_t;

typedef struct osrmc_table_params* osrmc_table_params_t;
typedef struct osrmc_table_response* osrmc_table_response_t;

/* API */

OSRMC_API osrmc_config_t osrmc_config_construct(const char* base_path);
OSRMC_API void osrmc_config_destruct(osrmc_config_t config);

OSRMC_API osrmc_osrm_t osrmc_osrm_construct(osrmc_config_t config);
OSRMC_API void osrmc_osrm_destruct(osrmc_osrm_t osrm);

OSRMC_API void osrmc_params_add_coordinate(osrmc_params_t params, float longitude, float latitude);
OSRMC_API void osrmc_params_add_coordinate_with(osrmc_params_t params, float longitude, float latitude, float radius, int bearing, int range);

OSRMC_API osrmc_route_params_t osrmc_route_params_construct(void);
OSRMC_API void osrmc_route_params_destruct(osrmc_route_params_t params);
OSRMC_API void osrmc_route_params_add_steps(osrmc_route_params_t params, bool on);
OSRMC_API void osrmc_route_params_add_alternatives(osrmc_route_params_t params, bool on);

OSRMC_API osrmc_route_response_t osrmc_route(osrmc_osrm_t osrm, osrmc_route_params_t params);
OSRMC_API void osrmc_route_response_destruct(osrmc_route_response_t response);
OSRMC_API float osrmc_route_response_distance(osrmc_route_response_t response);
OSRMC_API float osrmc_route_response_duration(osrmc_route_response_t response);

OSRMC_API osrmc_table_params_t osrmc_table_params_construct(void);
OSRMC_API void osrmc_table_params_destruct(osrmc_table_params_t params);
OSRMC_API void osrmc_table_params_add_source(osrmc_table_params_t params, size_t index);
OSRMC_API void osrmc_table_params_add_destination(osrmc_table_params_t params, size_t index);

OSRMC_API osrmc_table_response_t osrmc_table(osrmc_osrm_t osrm, osrmc_table_params_t params);
OSRMC_API void osrmc_table_response_destruct(osrmc_table_response_t response);
OSRMC_API float osrmc_table_response_duration(osrmc_table_response_t response, unsigned long from, unsigned long to);

#ifdef __cplusplus
}
#endif

#endif

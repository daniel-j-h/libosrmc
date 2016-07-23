#ifndef OSRMC_H_
#define OSRMC_H_

/*
 * The following provides a high-level interface overview for libosrmc.
 *
 *
 * Workflow
 * ========
 *
 * The library provides access to the following services: Route, Table, Nearest, Match.
 * These services are hidden behind the osrmc_osrm_t type which you have to create first.
 * This can be done by constructing it from osrmc_config_t setting the .osrm extract path.
 * Note: in the following error handling is omitted for brevity. See section Error Handling.
 *
 * Example:
 *
 *   osrmc_config_t config = osrmc_config_construct(path, &error);
 *   osrmc_osrm_t osrm = osrmc_osrm_construct(config, &error);
 *
 * For querying a service you have to first create a service-specific parameters object.
 * Constructing an object follows the naming convention osrmc_service_params_construct.
 * Destructing an object follows the naming convention osrmc_service_params_destruct.
 *
 * You then add coordinates and service-specific setting to the parameters.
 *
 * Adding coordinates works across all parameters using osrmc_params_add_coordinate and the
 * more specific osrmc_params_add_coordinate_with allowing for more control over snapping.
 * Both functions work on the structural base type osrmc_params_t.
 *
 * Example:
 *
 *   osrmc_route_params_t params = osrmc_route_params_construct(&error);
 *   osrmc_params_add_coordinate((osrmc_params_t)params, longitude, latitude, &error);
 *
 * There are service-specific functions for fine-tuning responses and behavior.
 *
 * Example:
 *
 *   osrmc_route_params_t params = osrmc_route_params_construct(&error);
 *   osrmc_route_params_add_alternatives(params, 1);
 *
 * Finally, you query the service passing the parameters object and extract specific results from it.
 *
 * Example:
 *
 *   response = osrmc_route(osrm, params, &error);
 *   distance = osrmc_route_response_distance(response, &error);
 *
 *
 * Types
 * =====
 *
 * The library provides opaque types hiding the implementation.
 * Constructing an object of such a type follows the osrmc_type_construct naming convention.
 * Destructing an object of such a type follows the osrmc_type_destruct naming convention.
 *
 * Example:
 *
 *   config = osrmc_config_construct(path, &error);
 *   osrmc_config_destruct(config);
 *
 *
 * Error handling
 * ==============
 *
 * Error handling is done by passing an osrmc_error_t out parameter to functions that may fail.
 * Only on failure: the library fills the error object from which you can get a error message via osrmc_error_message.
 * Only on failure: you take over ownership and have to destruct the error object via osrmc_error_destruct.
 *
 * Example:
 *
 *   osrmc_error_t error = NULL;
 *   params = osrmc_route_params_construct(&error);
 *   if (error) {
 *     fprintf(stderr, "Error: %s\n", osrmc_error_message(error));
 *     osrmc_error_destruct(error);
 *     return EXIT_FAILURE;
 *   }
 *
 *
 * Responses and Callbacks
 * =======================
 *
 * The library provides both functions returning response objects from services as well as functions taking callbacks.
 *
 * Response object types follow the osrmc_service_response_t naming convention.
 * You take over ownership and have to destruct the response object via osrmc_service_response_destruct.
 * The library provides functions for extracting data from the response objects.
 *
 * Example:
 *
 *   response = osrmc_route(osrm, params, longitude, latitude, &error);
 *   distance = osrmc_route_response_distance(response, &error);
 *   osrmc_route_response_destruct(response);
 *
 * For functions taking callbacks, the library expects you to provide handlers, for example for waypoints.
 * The library handles response ownership and cleanup internally, that is all you have to do is check for failure.
 * All handlers and functions invoking handlers take user-provided void* for transparently passing through client data.
 *
 * Example:
 *
 *   void my_waypoint_handler(void* data, const char* name, float longitude, float latitude) {
 *     printf("Longitude: %f, Latitude: %f\n", longitude, latitude);
 *   }
 *
 *   osrmc_route_with(osrm, params, my_waypoint_handler, NULL, &error);
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ABI Stability */

#if __GNUC__ >= 4
  #define OSRMC_API __attribute__((visibility("default")))
#else
  #define OSRMC_API
#endif

#define OSRMC_VERSION_MAJOR 5
#define OSRMC_VERSION_MINOR 3
#define OSRMC_VERSION ((OSRMC_VERSION_MAJOR << 16) | OSRMC_VERSION_MINOR)

OSRMC_API unsigned osrmc_get_version(void);
OSRMC_API int osrmc_is_abi_compatible(void);

/* Error handling */

typedef struct osrmc_error* osrmc_error_t;

/* Config and osrmc */

typedef struct osrmc_config* osrmc_config_t;
typedef struct osrmc_osrm* osrmc_osrm_t;

/* Generic parameters */

typedef struct osrmc_params* osrmc_params_t;

/* Service-specific parameters */

typedef struct osrmc_route_params* osrmc_route_params_t;
typedef struct osrmc_table_params* osrmc_table_params_t;
typedef struct osrmc_nearest_params* osrmc_nearest_params_t;
typedef struct osrmc_match_params* osrmc_match_params_t;

/* Service-specific responses */

typedef struct osrmc_route_response* osrmc_route_response_t;
typedef struct osrmc_table_response* osrmc_table_response_t;

/* Service-specific callbacks */

typedef void (*osrmc_waypoint_handler_t)(void* data, const char* name, float longitude, float latitude);


/* Error handling */

OSRMC_API const char* osrmc_error_message(osrmc_error_t error);
OSRMC_API void osrmc_error_destruct(osrmc_error_t error);

/* Config and osrmc */

OSRMC_API osrmc_config_t osrmc_config_construct(const char* base_path, osrmc_error_t* error);
OSRMC_API void osrmc_config_destruct(osrmc_config_t config);

OSRMC_API osrmc_osrm_t osrmc_osrm_construct(osrmc_config_t config, osrmc_error_t* error);
OSRMC_API void osrmc_osrm_destruct(osrmc_osrm_t osrm);

/* Generic parameters */

OSRMC_API void osrmc_params_add_coordinate(osrmc_params_t params, float longitude, float latitude,
                                           osrmc_error_t* error);
OSRMC_API void osrmc_params_add_coordinate_with(osrmc_params_t params, float longitude, float latitude, float radius,
                                                int bearing, int range, osrmc_error_t* error);

/* Route service */

OSRMC_API osrmc_route_params_t osrmc_route_params_construct(osrmc_error_t* error);
OSRMC_API void osrmc_route_params_destruct(osrmc_route_params_t params);
OSRMC_API void osrmc_route_params_add_steps(osrmc_route_params_t params, int on);
OSRMC_API void osrmc_route_params_add_alternatives(osrmc_route_params_t params, int on);

OSRMC_API osrmc_route_response_t osrmc_route(osrmc_osrm_t osrm, osrmc_route_params_t params, osrmc_error_t* error);
OSRMC_API void osrmc_route_with(osrmc_osrm_t osrm, osrmc_route_params_t params, osrmc_waypoint_handler_t handler,
                                void* data, osrmc_error_t* error);
OSRMC_API void osrmc_route_response_destruct(osrmc_route_response_t response);
OSRMC_API float osrmc_route_response_distance(osrmc_route_response_t response, osrmc_error_t* error);
OSRMC_API float osrmc_route_response_duration(osrmc_route_response_t response, osrmc_error_t* error);

/* Table service */

OSRMC_API osrmc_table_params_t osrmc_table_params_construct(osrmc_error_t* error);
OSRMC_API void osrmc_table_params_destruct(osrmc_table_params_t params);
OSRMC_API void osrmc_table_params_add_source(osrmc_table_params_t params, size_t index, osrmc_error_t* error);
OSRMC_API void osrmc_table_params_add_destination(osrmc_table_params_t params, size_t index, osrmc_error_t* error);

OSRMC_API osrmc_table_response_t osrmc_table(osrmc_osrm_t osrm, osrmc_table_params_t params, osrmc_error_t* error);
OSRMC_API void osrmc_table_response_destruct(osrmc_table_response_t response);
OSRMC_API float osrmc_table_response_duration(osrmc_table_response_t response, unsigned long from, unsigned long to,
                                              osrmc_error_t* error);
/* Nearest service */

OSRMC_API osrmc_nearest_params_t osrmc_nearest_params_construct(osrmc_error_t* error);
OSRMC_API void osrmc_nearest_params_destruct(osrmc_nearest_params_t params);
OSRMC_API void osrmc_nearest_set_number_of_results(osrmc_nearest_params_t params, unsigned n, osrmc_error_t* error);

/* Match service */

OSRMC_API osrmc_match_params_t osrmc_match_params_construct(osrmc_error_t* error);
OSRMC_API void osrmc_match_params_destruct(osrmc_match_params_t params);
OSRMC_API void osrmc_match_params_add_timestamp(osrmc_match_params_t params, unsigned timestamp, osrmc_error_t* error);

#ifdef __cplusplus
}
#endif

#endif

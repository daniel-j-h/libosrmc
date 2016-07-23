#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <osrmc/osrmc.h>


/* Custom handler for printing information about all waypoints */
void waypoint_handler(void* data, const char* name, float longitude, float latitude) {
  (void)data;
  printf("Name: %s, Longitude: %.5f, Latitude: %.5f\n", name, longitude, latitude);
}


int main(int argc, char** argv) {
  osrmc_error_t error;
  osrmc_config_t config;
  osrmc_osrm_t osrm;
  osrmc_route_params_t params;
  osrmc_route_response_t response;

  float distance;
  float duration;

  /* Make sure installed header interface matches installed library */
  assert(osrmc_is_abi_compatible());

  if (argc != 2) {
    fprintf(stderr, "Usage: %s monaco.osrm", argv[0]);
    return EXIT_FAILURE;
  }

  error = NULL;

  /* Create osrmc from a config setting the .osrm base path */
  config = osrmc_config_construct(argv[1], &error);
  if (error)
    goto config_cleanup;

  osrm = osrmc_osrm_construct(config, &error);
  if (error)
    goto osrm_cleanup;

  /* Create parameters for the Route service and add some locations */
  params = osrmc_route_params_construct(&error);
  if (error)
    goto params_cleanup;

  osrmc_params_add_coordinate((osrmc_params_t)params, 7.419758, 43.731142, &error);
  osrmc_params_add_coordinate((osrmc_params_t)params, 7.419505, 43.736825, &error);

  if (error)
    goto params_cleanup;

  /* Send request to the Route service, extract distance and duration from the response */
  response = osrmc_route(osrm, params, &error);
  if (error)
    goto response_cleanup;

  distance = osrmc_route_response_distance(response, &error);
  duration = osrmc_route_response_duration(response, &error);

  if (error)
    goto response_cleanup;

  printf("Distance: %.0f meters\n", distance);
  printf("Duration: %.0f seconds\n", duration);

  /* Alternatively let the library run our custom waypoint handler for each waypoint */
  osrmc_route_with(osrm, params, waypoint_handler, NULL, &error);
  if (error)
    goto response_cleanup;


/* Properly clean up resources, emulate stack-unwinding */
response_cleanup:
  osrmc_route_response_destruct(response);
params_cleanup:
  osrmc_route_params_destruct(params);
osrm_cleanup:
  osrmc_osrm_destruct(osrm);
config_cleanup:
  osrmc_config_destruct(config);

  /* If we got here on a failure path notify the user */
  if (error) {
    fprintf(stderr, "Error: %s\n", osrmc_error_message(error));
    osrmc_error_destruct(error);
    return EXIT_FAILURE;
  }

  /* Otherwise we're done */
  return EXIT_SUCCESS;
}

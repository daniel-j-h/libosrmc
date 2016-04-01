#include <new>

#include "osrmc.h"

// ABI stability
unsigned osrmc_get_version(void) { return OSRMC_VERSION; }

bool osrmc_is_abi_compatible(void) {
  return osrmc_get_version() >> 16 == OSRMC_VERSION_MAJOR;
}

// Types
struct osrm {};
struct status {};

// Constructor
osrm_t osrm_construct(void) { return nullptr; }

// Destructor
void osrm_destruct(osrm_t osrm) { (void)osrm; }

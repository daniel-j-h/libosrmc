#ifndef OSRM_C_H_
#define OSRM_C_H_

#include <stdbool.h>

#include "visibility.h"

#ifdef __cplusplus
extern "C" {
#endif

// ABI Stability
#define OSRMC_VERSION_MAJOR 1
#define OSRMC_VERSION_MINOR 0
#define OSRMC_VERSION ((OSRMC_VERSION_MAJOR << 16) | OSRMC_VERSION_MINOR)

OSRMC_API unsigned osrmc_get_version(void);
OSRMC_API bool osrmc_is_abi_compatible(void);

// Types
typedef struct osrm* osrm_t;
typedef struct status* status_t;

// Constructor
OSRMC_API osrm_t osrm_construct(void);

// Destructor
OSRMC_API void osrm_destruct(osrm_t osrm);

#ifdef __cplusplus
}
#endif

#endif

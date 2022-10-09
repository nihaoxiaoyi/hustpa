#ifndef __COMMON_H__
#define __COMMON_H__

// #define DEBUG            // if you want to DEBUG, please start Uncomment
// #define DIFF_TEST           

#if _SHARE
// do not enable these features while building a reference design
#undef DIFF_TEST
#undef DEBUG
#endif

/* You will define this macro in PA2 */
//#define HAS_IOE

#include <stdint.h>
#include <assert.h>
#include <string.h>

typedef uint8_t bool;

typedef uint32_t rtlreg_t;

typedef uint32_t paddr_t;
typedef uint32_t vaddr_t;

typedef uint16_t ioaddr_t;

#define false 0
#define true 1

/* my diff test */
/* Start */

// #define DIFF_TEST       // dinfined the DIFF_TEST macro to start DIFF_TEST
// #define DEBUG           // if you want to DEBUG, please start Uncomment
#define HAS_IOE         // to start device

/* End */

#include "debug.h"
#include "macro.h"

#endif

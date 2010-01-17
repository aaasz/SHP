#include "co.h"
#ifdef IMPULSE_C_TARGET
#define printf xil_printf
#include <xparameters.h>
#else
#include <cosim_log.h>
#endif
#if defined(XPAR_MICROBLAZE_ID)
#include <mb_interface.h>
#include <xbasic_types.h>
#include <xutil.h>
#include <xtmrctr.h>
#define printf xil_printf
#endif
static double to_double(uint64 i) {
    return *(double *)&i;
}


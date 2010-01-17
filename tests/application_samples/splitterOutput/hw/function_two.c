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
void function_two(co_stream s_in, co_stream s_out) {
    co_int32 i, a, b, c, n;
    co_stream_open(s_in, 0, co_type_create(co_int_sort, 32));
    co_stream_open(s_out, 1, co_type_create(co_int_sort, 32));
    co_stream_read(s_in, &n, sizeof(co_int32));
    a = 0;
    b = 1;
    for (i = 1; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    b = ((b) < 0 ? -(b) : (b)) % n;
    co_stream_write(s_out, &b, sizeof(co_int32));
    co_stream_close(s_out);
    co_stream_close(s_in);
}


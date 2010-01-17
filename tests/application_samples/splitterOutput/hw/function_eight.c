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
void function_eight(co_stream s_in1, co_stream s_in2, co_stream s_out) {
    co_int32 i, a, b, c, n1, n2;
    co_stream_open(s_in1, 0, co_type_create(co_int_sort, 32));
    co_stream_open(s_in2, 0, co_type_create(co_int_sort, 32));
    co_stream_open(s_out, 1, co_type_create(co_int_sort, 32));
    co_stream_read(s_in1, &n1, sizeof(co_int32));
    co_stream_read(s_in2, &n2, sizeof(co_int32));
    a = 0;
    b = 1;
    for (i = 1; i <= n1; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    b = ((b + n2) < 0 ? -(b + n2) : (b + n2)) % n1;
    co_stream_write(s_out, &b, sizeof(co_int32));
    co_stream_close(s_out);
    co_stream_close(s_in1);
    co_stream_close(s_in2);
}


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
void function_one(co_stream s_in, co_stream s_out1, co_stream s_out2, co_stream s_out3) {
    co_int32 i, a, b1, b2, b3, c, n;
    co_stream_open(s_in, 0, co_type_create(co_int_sort, 32));
    co_stream_open(s_out1, 1, co_type_create(co_int_sort, 32));
    co_stream_open(s_out2, 1, co_type_create(co_int_sort, 32));
    co_stream_open(s_out3, 1, co_type_create(co_int_sort, 32));
    co_stream_read(s_in, &n, sizeof(co_int32));
    a = 0;
    b1 = 1;
    for (i = 1; i <= n; i++) {
        c = a + b1;
        a = b1;
        b1 = c;
    }
    b1 = ((b1) < 0 ? -(b1) : (b1)) % n;
    b2 = b1 + 5;
    b3 = b1 + 10;
    co_stream_write(s_out1, &b1, sizeof(co_int32));
    co_stream_write(s_out2, &b2, sizeof(co_int32));
    co_stream_write(s_out3, &b3, sizeof(co_int32));
    co_stream_close(s_out3);
    co_stream_close(s_out2);
    co_stream_close(s_out1);
    co_stream_close(s_in);
}


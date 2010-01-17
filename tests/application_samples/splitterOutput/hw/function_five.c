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
void function_five(co_stream s_in1, co_stream s_in2, co_stream s_in3, co_stream s_out1, co_stream s_out2) {
    co_int32 i, a, b1, b2, b3, c, n1, n2, n3;
    co_stream_open(s_in1, 0, co_type_create(co_int_sort, 32));
    co_stream_open(s_in2, 0, co_type_create(co_int_sort, 32));
    co_stream_open(s_in3, 0, co_type_create(co_int_sort, 32));
    co_stream_open(s_out1, 1, co_type_create(co_int_sort, 32));
    co_stream_open(s_out2, 1, co_type_create(co_int_sort, 32));
    co_stream_read(s_in1, &n1, sizeof(co_int32));
    co_stream_read(s_in2, &n2, sizeof(co_int32));
    co_stream_read(s_in3, &n3, sizeof(co_int32));
    a = 0;
    b1 = 1;
    for (i = 1; i <= n1; i++) {
        c = a + b1;
        a = b1;
        b1 = c;
    }
    a = 0;
    b2 = 1;
    for (i = 1; i <= n2; i++) {
        c = a + b2;
        a = b2;
        b2 = c;
    }
    a = 0;
    b3 = 1;
    for (i = 1; i <= n3; i++) {
        c = a + b3;
        a = b3;
        b3 = c;
    }
    b1 = ((b1 + b2) < 0 ? -(b1 + b2) : (b1 + b2)) % n1;
    b2 = ((b2 + b3) < 0 ? -(b2 + b3) : (b2 + b3)) % n2;
    co_stream_write(s_out1, &b1, sizeof(co_int32));
    co_stream_write(s_out2, &b2, sizeof(co_int32));
    co_stream_close(s_out1);
    co_stream_close(s_out2);
    co_stream_close(s_in1);
    co_stream_close(s_in2);
    co_stream_close(s_in3);
}


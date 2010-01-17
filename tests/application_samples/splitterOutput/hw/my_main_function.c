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
void my_main_function(co_stream s_in, co_stream s_out) {
    int i;
    int hi = 10000000;
    int32 err;
    co_stream_open(s_in, 0, co_type_create(co_int_sort, 32));
    co_stream_open(s_out, 1, co_type_create(co_int_sort, 32));
    co_stream_write(s_out, &(hi), sizeof (hi));
    (err = (co_stream_read(s_in, &(hi), sizeof (hi)) != co_err_none));
    printf("Rez = %d\n", hi);
    printf("SERIALSTOP");
    co_stream_close(s_in);
    co_stream_close(s_out);
}


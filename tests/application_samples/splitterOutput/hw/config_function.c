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
extern void my_main_function(co_stream s_in,co_stream s_out);
extern void function_one(co_stream s_in,co_stream s_out1,co_stream s_out2,co_stream s_out3);
extern void function_two(co_stream s_in,co_stream s_out);
extern void function_three(co_stream s_in,co_stream s_out);
extern void function_four(co_stream s_in,co_stream s_out);
extern void function_five(co_stream s_in1,co_stream s_in2,co_stream s_in3,co_stream s_out1,co_stream s_out2);
extern void function_six(co_stream s_in,co_stream s_out);
extern void function_seven(co_stream s_in,co_stream s_out);
extern void function_eight(co_stream s_in1,co_stream s_in2,co_stream s_out);
void config_function(void *arg) {
    co_process my_main_function_proc, function_one_proc, function_two_proc, function_three_proc, function_four_proc, function_five_proc, function_six_proc, function_seven_proc, function_eight_proc;
    co_stream stream_M_one = co_stream_create("stream_M_one", co_type_create(co_int_sort, 32), 8);
    co_stream stream_one_two = co_stream_create("stream_one_two", co_type_create(co_int_sort, 32), 8);
    co_stream stream_one_three = co_stream_create("stream_one_three", co_type_create(co_int_sort, 32), 8);
    co_stream stream_one_four = co_stream_create("stream_one_four", co_type_create(co_int_sort, 32), 8);
    co_stream stream_two_five = co_stream_create("stream_two_five", co_type_create(co_int_sort, 32), 8);
    co_stream stream_three_five = co_stream_create("stream_three_five", co_type_create(co_int_sort, 32), 8);
    co_stream stream_four_five = co_stream_create("stream_four_five", co_type_create(co_int_sort, 32), 8);
    co_stream stream_five_six = co_stream_create("stream_five_six", co_type_create(co_int_sort, 32), 8);
    co_stream stream_five_seven = co_stream_create("stream_five_seven", co_type_create(co_int_sort, 32), 8);
    co_stream stream_six_eight = co_stream_create("stream_six_eight", co_type_create(co_int_sort, 32), 8);
    co_stream stream_seven_eight = co_stream_create("stream_seven_eight", co_type_create(co_int_sort, 32), 8);
    co_stream stream_eight_M = co_stream_create("stream_eight_M", co_type_create(co_int_sort, 32), 8);
    my_main_function_proc = co_process_create("my_main_function", (co_function)my_main_function, 2, stream_M_one, stream_eight_M);
    function_one_proc = co_process_create("function_one", (co_function)function_one, 4, stream_M_one, stream_one_two, stream_one_three, stream_one_four);
    function_two_proc = co_process_create("function_two", (co_function)function_two, 2, stream_one_two, stream_two_five);
    function_three_proc = co_process_create("function_three", (co_function)function_three, 2, stream_one_three, stream_three_five);
    function_four_proc = co_process_create("function_four", (co_function)function_four, 2, stream_one_four, stream_four_five);
    function_five_proc = co_process_create("function_five", (co_function)function_five, 5, stream_two_five, stream_three_five, stream_four_five, stream_five_six, stream_five_seven);
    function_six_proc = co_process_create("function_six", (co_function)function_six, 2, stream_five_six, stream_six_eight);
    function_seven_proc = co_process_create("function_seven", (co_function)function_seven, 2, stream_five_seven, stream_seven_eight);
    function_eight_proc = co_process_create("function_eight", (co_function)function_eight, 3, stream_six_eight, stream_seven_eight, stream_eight_M);
    co_process_config(function_one_proc, co_loc, "PE0");
    co_process_config(function_two_proc, co_loc, "PE0");
    co_process_config(function_three_proc, co_loc, "PE0");
    co_process_config(function_four_proc, co_loc, "PE0");
    co_process_config(function_five_proc, co_loc, "PE0");
    co_process_config(function_six_proc, co_loc, "PE0");
    co_process_config(function_seven_proc, co_loc, "PE0");
    co_process_config(function_eight_proc, co_loc, "PE0");
}


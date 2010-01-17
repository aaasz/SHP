#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include "co.h"


extern void test_function_eight(co_stream s_in, co_stream s_out1, co_stream s_out2);
extern void function_eight(co_stream s_in1, co_stream s_in2, co_stream s_out);
extern void stub_function_eight(co_stream s_in, co_stream s_out);

void config_function(void * arg)
{
    co_process test_function_eight_proc, function_eight_proc, stub_function_eight_proc;

    co_stream stream_test_function1 = co_stream_create("stream_test_function1", INT_TYPE(32), 8);
    co_stream stream_test_function2 = co_stream_create("stream_test_function2", INT_TYPE(32), 8);
    co_stream stream_stub_test = co_stream_create("stream_stub_test", INT_TYPE(32), 8);
	co_stream stream_function_stub = co_stream_create("stream_function_stub", INT_TYPE(32), 8);


    test_function_eight_proc = co_process_create("test_function_eight", (co_function)test_function_eight,
                                       3,                  
                                       stream_stub_test,
									   stream_test_function1,
									   stream_test_function2);

    function_eight_proc = co_process_create("function_eight", (co_function)function_eight,
                                        3,
										stream_test_function1,
										stream_test_function2,
										stream_function_stub);

    stub_function_eight_proc = co_process_create("stub_function_eight", (co_function)stub_function_eight,
                                        2,
										stream_function_stub,
										stream_stub_test);


    co_process_config(function_eight_proc, co_loc, "PE0");
    co_process_config(stub_function_eight_proc, co_loc, "PE0");

}

co_architecture co_initialize(void * arg)
{
    return co_architecture_create("hello", "xilinx_mb_fsl", config_function,(void *) arg);
}

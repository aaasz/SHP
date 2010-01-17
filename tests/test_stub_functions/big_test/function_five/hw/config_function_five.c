#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include "co.h"


extern void test_function_five(co_stream s_in, co_stream s_out1, co_stream s_out2, co_stream s_out3);
extern void function_five(co_stream s_in1, co_stream s_in2, co_stream s_in3, co_stream s_out1, co_stream s_out2);
extern void stub_function_five(co_stream s_in1, co_stream s_in2, co_stream s_out);

void config_function(void * arg)
{
    co_process test_function_five_proc, function_five_proc, stub_function_five_proc;
    
	co_stream stream_test_function1 = co_stream_create("stream_test_function1", INT_TYPE(32), 8);
	co_stream stream_test_function2 = co_stream_create("stream_test_function2", INT_TYPE(32), 8);
	co_stream stream_test_function3 = co_stream_create("stream_test_function3", INT_TYPE(32), 8);
	co_stream stream_stub_test = co_stream_create("stream_stub_test", INT_TYPE(32), 8);    
	co_stream stream_function_stub1 = co_stream_create("stream_function_stub1", INT_TYPE(32), 8);
    co_stream stream_function_stub2 = co_stream_create("stream_function_stub2", INT_TYPE(32), 8);
    

    test_function_five_proc = co_process_create("test_function_five", (co_function)test_function_five,
                                       4,                  
                                       stream_stub_test,
									   stream_test_function1,
									   stream_test_function2,
									   stream_test_function3);

    function_five_proc = co_process_create("function_five", (co_function)function_five,
                                        5,
										stream_test_function1,
										stream_test_function2,
										stream_test_function3,
										stream_function_stub1,
										stream_function_stub2);

    stub_function_five_proc = co_process_create("stub_function_five", (co_function)stub_function_five,
                                        3,
										stream_function_stub1,
										stream_function_stub2,
										stream_stub_test);

    co_process_config(function_five_proc, co_loc, "PE0");
    co_process_config(stub_function_five_proc, co_loc, "PE0");

}

co_architecture co_initialize(void * arg)
{
    return co_architecture_create("hello", "xilinx_mb_fsl", config_function,(void *) arg);
}

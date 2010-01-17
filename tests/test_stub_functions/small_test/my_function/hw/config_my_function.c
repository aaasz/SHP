#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include "co.h"


extern void my_test_function(co_stream s_in, co_stream s_out);
extern void my_function(co_stream s_in, co_stream s_out);

void config_function(void * arg)
{
    co_process my_function_proc, my_test_function_proc;
    co_stream stream_in = co_stream_create("stream_in", INT_TYPE(32), 8);
    co_stream stream_out = co_stream_create("stream_out", INT_TYPE(32), 8);

    my_function_proc = co_process_create("my_function", (co_function)my_function,
                                       2,                  
                                       stream_in,
									   stream_out);

    my_test_function_proc = co_process_create("my_test_function", (co_function)my_test_function,
                                        2,
										stream_out,
										stream_in);

    co_process_config(my_function_proc, co_loc, "PE0");
}

co_architecture co_initialize(void * arg)
{
    return co_architecture_create("hello", "xilinx_mb_fsl", config_function,(void *) arg);
}

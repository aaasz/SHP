#include <stdio.h>
#include <math.h>
#include "co.h"

void stub_function_three(co_stream s_in, co_stream s_out)
{
    co_int32 n;
    
	co_stream_open(s_in, O_RDONLY, INT_TYPE(32));
	co_stream_open(s_out, O_WRONLY, INT_TYPE(32));
    
   	co_stream_read(s_in, &n, sizeof(co_int32));
   	
	co_stream_write(s_out, &n, sizeof(co_int32));

	co_stream_close(s_out);
	co_stream_close(s_in);
}


#include "co.h"
#include <stdio.h>	

#ifdef IMPULSE_C_TARGET
#define printf xil_printf
#include "xparameters.h"
#else
#include "cosim_log.h"
#endif

#if defined(XPAR_MICROBLAZE_ID)
#include "mb_interface.h"
#include "xbasic_types.h"
#include "xutil.h"
#include "xtmrctr.h"
#define printf xil_printf
#define TIMER_FREQ_MILLI (XPAR_CPU_CORE_CLOCK_FREQ_HZ/1000)
#define INIT_TIMER() \
  XTmrCtr_Initialize(&TimerInst, XPAR_XPS_TIMER_1_DEVICE_ID);\
  XTmrCtr_SetResetValue(&TimerInst,0,0);\
  XTmrCtr_Start(&TimerInst,0)

#define START_TIMER(var) XTmrCtr_Reset(&TimerInst,0)
#define END_TIMER(var) var=XTmrCtr_GetValue(&TimerInst,0)

#define TIMER_UNITS "cycles"
	XTmrCtr		TimerInst;
	Xuint32		StartIF_TS;
	Xuint32		FinishIF_TS;
	Xuint32		t_Time1, t_Time2;
	Xuint32		TimeSA, TimeHA;
	XStatus		status;
#endif

void my_test_function(co_stream s_in, co_stream s_out)
{
    int i;
    int hi = 10000000;
	int32   	err;
	
    co_stream_open(s_in, O_RDONLY, INT_TYPE(32));
    co_stream_open(s_out, O_WRONLY, INT_TYPE(32));

#if defined(XPAR_MICROBLAZE_ID)
    printf ("\r\n===================Running on MicroBlaze================\r\n");
    INIT_TIMER();
    StartIF_TS = XTmrCtr_GetValue (&TimerInst, 0);
#endif

    HW_STREAM_WRITE(my_test_function, s_out, hi);
    
#if defined(XPAR_MICROBLAZE_ID)
    FinishIF_TS = XTmrCtr_GetValue (&TimerInst, 0);
    printf ("\r\n--> Timpul de comunicatie com1: ");

    TimeSA = (FinishIF_TS - StartIF_TS)/TIMER_FREQ_MILLI; 
    printf (" %d milliseconds\r\n", TimeSA);
#endif

#if defined(XPAR_MICROBLAZE_ID)
    StartIF_TS = XTmrCtr_GetValue (&TimerInst, 0);
#endif
   	HW_STREAM_READ(my_test_function, s_in, hi, err);

    printf("Rez = %d\n", hi);	
#if defined(XPAR_MICROBLAZE_ID)
    FinishIF_TS = XTmrCtr_GetValue (&TimerInst, 0);
    printf ("\r\n--> Timpul de executie: ");
    TimeSA = (FinishIF_TS - StartIF_TS)/TIMER_FREQ_MILLI; 
    printf ("%d milliseconds\r\n", TimeSA);
#endif
   
    printf("SERIALSTOP");		   

    HW_STREAM_CLOSE(my_test_function, s_in);
    HW_STREAM_CLOSE(my_test_function, s_out);
}


#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include "co.h"

// Software process
extern void my_test_function(co_stream s_in, co_stream s_out);

void my_function(co_stream s_in, co_stream s_out)
{
    co_int32 i, a, b, c, n;
    
    co_stream_open(s_in, O_RDONLY, INT_TYPE(32));
    co_stream_open(s_out, O_WRONLY, INT_TYPE(32));

   	co_stream_read(s_in, &n, sizeof(co_int32));

	a = 0;
	b = 1;
	
	for (i = 1; i<= n; i++)
	{
		c = a + b;
		a = b;
		b = c;
	}

	co_stream_write(s_out, &b, sizeof(co_int32));

	co_stream_close(s_out);
	co_stream_close(s_in);
}

void config_hello(void * arg)
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
    return co_architecture_create("hello", "xilinx_mb_fsl", config_hello,(void *) arg);
}

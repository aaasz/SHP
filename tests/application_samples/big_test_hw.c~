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

void my_main_function(co_stream s_in, co_stream s_out)
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

void function_one(co_stream s_in, co_stream s_out1, co_stream s_out2, co_stream s_out3)
{
    co_int32 i, a, b1, b2, b3, c, n;
    
    co_stream_open(s_in, O_RDONLY, INT_TYPE(32));
    co_stream_open(s_out1, O_WRONLY, INT_TYPE(32));
	co_stream_open(s_out2, O_WRONLY, INT_TYPE(32));
	co_stream_open(s_out3, O_WRONLY, INT_TYPE(32));

   	co_stream_read(s_in, &n, sizeof(co_int32));

	a = 0;
	b1 = 1;
	
	for (i = 1; i<= n; i++)
	{
		c = a + b1;
		a = b1;
		b1 = c;
	}
	b1 = abs(b1) % n;
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


void function_two(co_stream s_in, co_stream s_out)
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

	b = abs(b) % n;

	co_stream_write(s_out, &b, sizeof(co_int32));

	co_stream_close(s_out);
	co_stream_close(s_in);
}


void function_three(co_stream s_in, co_stream s_out)
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

	b = abs(b) % n;

	co_stream_write(s_out, &b, sizeof(co_int32));

	co_stream_close(s_out);
	co_stream_close(s_in);
}


void function_four(co_stream s_in, co_stream s_out)
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

	b = abs(b) % n;

	co_stream_write(s_out, &b, sizeof(co_int32));

	co_stream_close(s_out);
	co_stream_close(s_in);
}

void function_five(co_stream s_in1, co_stream s_in2, co_stream s_in3, co_stream s_out1, co_stream s_out2)
{
    co_int32 i, a, b1, b2, b3, c, n1, n2, n3;
    
    co_stream_open(s_in1, O_RDONLY, INT_TYPE(32));
    co_stream_open(s_in2, O_RDONLY, INT_TYPE(32));
    co_stream_open(s_in3, O_RDONLY, INT_TYPE(32));

    co_stream_open(s_out1, O_WRONLY, INT_TYPE(32));
	co_stream_open(s_out2, O_WRONLY, INT_TYPE(32));


   	co_stream_read(s_in1, &n1, sizeof(co_int32));
   	co_stream_read(s_in2, &n2, sizeof(co_int32));
   	co_stream_read(s_in3, &n3, sizeof(co_int32));

	a = 0;
	b1 = 1;
	
	for (i = 1; i<= n1; i++)
	{
		c = a + b1;
		a = b1;
		b1 = c;
	}

	a = 0;
	b2 = 1;
	
	for (i = 1; i<= n2; i++)
	{
		c = a + b2;
		a = b2;
		b2 = c;
	}

	a = 0;
	b3 = 1;
	
	for (i = 1; i<= n3; i++)
	{
		c = a + b3;
		a = b3;
		b3 = c;
	}
	
	b1 = abs(b1 + b2) % n1;
	b2 = abs(b2 + b3) % n2;


	co_stream_write(s_out1, &b1, sizeof(co_int32));
	co_stream_write(s_out2, &b2, sizeof(co_int32));

	co_stream_close(s_out1);
	co_stream_close(s_out2);

	co_stream_close(s_in1);
	co_stream_close(s_in2);
	co_stream_close(s_in3);
}

void function_six(co_stream s_in, co_stream s_out)
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

	b = abs(b) % n;

	co_stream_write(s_out, &b, sizeof(co_int32));

	co_stream_close(s_out);
	co_stream_close(s_in);
}


void function_seven(co_stream s_in, co_stream s_out)
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

	b = abs(b) % n;

	co_stream_write(s_out, &b, sizeof(co_int32));

	co_stream_close(s_out);
	co_stream_close(s_in);
}


void function_eight(co_stream s_in1, co_stream s_in2, co_stream s_out)
{
    co_int32 i, a, b, c, n1, n2;
    
    co_stream_open(s_in1, O_RDONLY, INT_TYPE(32));
    co_stream_open(s_in2, O_RDONLY, INT_TYPE(32));
    co_stream_open(s_out, O_WRONLY, INT_TYPE(32));

   	co_stream_read(s_in1, &n1, sizeof(co_int32));
   	co_stream_read(s_in2, &n2, sizeof(co_int32));

	a = 0;
	b = 1;
	
	for (i = 1; i<= n1; i++)
	{
		c = a + b;
		a = b;
		b = c;
	}

	b = abs(b + n2) % n1;

	co_stream_write(s_out, &b, sizeof(co_int32));

	co_stream_close(s_out);
	co_stream_close(s_in1);
	co_stream_close(s_in2);

}



void config_function(void * arg)
{
    co_process my_main_function_proc, function_one_proc, function_two_proc,
			   function_three_proc, function_four_proc, function_five_proc,
			   function_six_proc, function_seven_proc, function_eight_proc;

    co_stream stream_M_one = co_stream_create("stream_M_one", INT_TYPE(32), 8);
    co_stream stream_one_two = co_stream_create("stream_one_two", INT_TYPE(32), 8);
    co_stream stream_one_three = co_stream_create("stream_one_three", INT_TYPE(32), 8);
    co_stream stream_one_four = co_stream_create("stream_one_four", INT_TYPE(32), 8);
    co_stream stream_two_five = co_stream_create("stream_two_five", INT_TYPE(32), 8);
    co_stream stream_three_five = co_stream_create("stream_three_five", INT_TYPE(32), 8);
    co_stream stream_four_five = co_stream_create("stream_four_five", INT_TYPE(32), 8);
    co_stream stream_five_six = co_stream_create("stream_five_six", INT_TYPE(32), 8);
	co_stream stream_five_seven = co_stream_create("stream_five_seven", INT_TYPE(32), 8);
	co_stream stream_six_eight = co_stream_create("stream_six_eight", INT_TYPE(32), 8);
	co_stream stream_seven_eight = co_stream_create("stream_seven_eight", INT_TYPE(32), 8);
	co_stream stream_eight_M = co_stream_create("stream_eight_M", INT_TYPE(32), 8);
 
	my_main_function_proc = co_process_create("my_main_function", (co_function)my_main_function,
                                       2,                  
                                       stream_M_one,
									   stream_eight_M);

    function_one_proc = co_process_create("function_one", (co_function)function_one,
                                        4,
										stream_M_one,
										stream_one_two,
										stream_one_three,
										stream_one_four);

    function_two_proc = co_process_create("function_two", (co_function)function_two,
                                        2,
										stream_one_two,
										stream_two_five);

	function_three_proc = co_process_create("function_three", (co_function)function_three,
                                        2,
										stream_one_three,
										stream_three_five);

    function_four_proc = co_process_create("function_four", (co_function)function_four,
                                        2,
										stream_one_four,
										stream_four_five);

    function_five_proc = co_process_create("function_five", (co_function)function_five,
                                        5,
										stream_two_five,
										stream_three_five,
										stream_four_five,
										stream_five_six,
										stream_five_seven);

    function_six_proc = co_process_create("function_six", (co_function)function_six,
                                        2,
										stream_five_six,
										stream_six_eight);

    function_seven_proc = co_process_create("function_seven", (co_function)function_seven,
                                        2,
										stream_five_seven,
										stream_seven_eight);

    function_eight_proc = co_process_create("function_eight", (co_function)function_eight,
                                        3,
										stream_six_eight,
										stream_seven_eight,
										stream_eight_M);

	co_process_config(function_one_proc, co_loc, "PE0");
	co_process_config(function_two_proc, co_loc, "PE0");
	co_process_config(function_three_proc, co_loc, "PE0");
	co_process_config(function_four_proc, co_loc, "PE0");
	co_process_config(function_five_proc, co_loc, "PE0");
	co_process_config(function_six_proc, co_loc, "PE0");
	co_process_config(function_seven_proc, co_loc, "PE0");
	co_process_config(function_eight_proc, co_loc, "PE0");
}

co_architecture co_initialize(void * arg)
{
    return co_architecture_create("hello", "xilinx_mb_fsl", config_function,(void *) arg);
}

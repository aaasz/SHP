////////////////////////////////////////////////////////////
// Copyright (c) 2003, Impulse Accelerated Technologies, Inc.
// All Rights Reserved.
//
// HelloWorld_sw.c: Process to be executed on the target CPU.
//

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
	Xuint32		Time_Com;
	Xuint32		t_Time1, t_Time2;
	Xuint32		TimeSA, TimeHA;
	XStatus		status;
#endif

void test_function_three(co_stream s_in, co_stream s_out)
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

    HW_STREAM_WRITE(test_function_three, s_out, hi);
    
#if defined(XPAR_MICROBLAZE_ID)
    FinishIF_TS = XTmrCtr_GetValue (&TimerInst, 0);
    Time_Com = (FinishIF_TS - StartIF_TS)/TIMER_FREQ_MILLI; 
#endif

#if defined(XPAR_MICROBLAZE_ID)
    StartIF_TS = XTmrCtr_GetValue (&TimerInst, 0);
#endif
   	HW_STREAM_READ(test_function_three, s_in, hi, err);

    printf("Rez = %d\n", hi);	
#if defined(XPAR_MICROBLAZE_ID)

    FinishIF_TS = XTmrCtr_GetValue (&TimerInst, 0);

	/* Print timpul de comunicatie */
	printf("%d\n", 	Time_Com);

	/* Print timpul de executie */
	TimeSA = (FinishIF_TS - StartIF_TS)/TIMER_FREQ_MILLI; 
	printf("%d\n", 	TimeSA);

    printf("SERIALSTOP");		   

#endif
   
    HW_STREAM_CLOSE(test_function_three, s_in);
    HW_STREAM_CLOSE(test_function_three, s_out);
}

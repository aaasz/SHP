////////////////////////////////////////////////////////////
// Copyright (c) 2003, Impulse Accelerated Technologies, Inc.
// All Rights Reserved.
//
// HelloWorld_sw.c: Process to be executed on the target CPU.
//
#include "xparameters.h"
#include "xbasic_types.h"
#include "xutil.h"
#include "xtmrctr.h"

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

#define printf xil_printf

extern void function_six(int n, int *out);

int main()
{
	int out; 

	printf("Microblaze:\n");
    INIT_TIMER();
    StartIF_TS = XTmrCtr_GetValue (&TimerInst, 0);

    function_six(10000000, &out);
	
	printf("Rez = %d\n", out);

    FinishIF_TS = XTmrCtr_GetValue (&TimerInst, 0);
    TimeSA = (FinishIF_TS - StartIF_TS)/TIMER_FREQ_MILLI; 
	/* Timp comunicatie */	
	printf("0\n");

	/* Timp executie */
    printf ("%d\n", TimeSA);

    printf("SERIALSTOP");	
    return 0;
}



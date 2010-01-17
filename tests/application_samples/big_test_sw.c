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

void function_one(int n, int *out1, int *out2, int *out3)
{
    int i, a, b1, b2, b3, c;   	

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

	*out1 = b1;
	*out2 = b2;
	*out3 = b3;
}


void function_two(int n, int *out)
{
    int i, a, b, c;
    
	a = 0;
	b = 1;
	
	for (i = 1; i<= n; i++)
	{
		c = a + b;
		a = b;
		b = c;
	}

	b = abs(b);

	*out = b;
}

void function_three(int n, int *out)
{
    int i, a, b, c;
    
	a = 0;
	b = 1;
	
	for (i = 1; i<= n; i++)
	{
		c = a + b;
		a = b;
		b = c;
	}

	b = abs(b);

	*out = b;
}


void function_four(int n, int *out)
{
    int i, a, b, c;
    
	a = 0;
	b = 1;
	
	for (i = 1; i<= n; i++)
	{
		c = a + b;
		a = b;
		b = c;
	}

	b = abs(b);

	*out = b;
}

void function_five(int n1, int n2, int n3, int *out1, int *out2)
{
    int i, a, b1, b2, b3, c;
    
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


	*out1 = b1;
	*out2 = b2;
}

void function_six(int n, int *out)
{
    int i, a, b, c;
    
	a = 0;
	b = 1;
	
	for (i = 1; i<= n; i++)
	{
		c = a + b;
		a = b;
		b = c;
	}

	*out = abs(b) % n;	

}


void function_seven(int n, int *out)
{
    int i, a, b, c;
    
	a = 0;
	b = 1;
	
	for (i = 1; i<= n; i++)
	{
		c = a + b;
		a = b;
		b = c;
	}

	*out = abs(b) % n;	
}


void function_eight(int n1, int n2, int *out)
{
    int i, a, b, c;
    
	a = 0;
	b = 1;
	
	for (i = 1; i<= n1; i++)
	{
		c = a + b;
		a = b;
		b = c;
	}

	b = abs(b + n2) % n1;

	*out = b;	
}

int main()
{
	int one_out1, one_out2, one_out3;
	int two_out, three_out, four_out;
	int five_out1, five_out2;
	int six_out, seven_out;
	int eight_out;
	
	printf("Microblaze:\n");
    INIT_TIMER();
    StartIF_TS = XTmrCtr_GetValue (&TimerInst, 0);

    function_one(10000000, &one_out1, &one_out2, &one_out3);
	function_two(one_out1, &two_out);
	function_three(one_out2, &three_out);
	function_four(one_out3, &four_out);
	function_five(two_out, three_out, four_out, &five_out1, &five_out2);
	function_six(five_out1, &six_out);
	function_seven(five_out2, &seven_out);
	function_eight(six_out, seven_out, &eight_out);

	printf("Rezultat = %d \n", eight_out);

    FinishIF_TS = XTmrCtr_GetValue (&TimerInst, 0);
    TimeSA = (FinishIF_TS - StartIF_TS)/TIMER_FREQ_MILLI; 

	/* Timp comunicatie */	
	printf("0\n");

	/* Timp executie */
    printf ("%d\n", TimeSA);

    printf("SERIALSTOP");	
    return 0;
}



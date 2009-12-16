///////////////////////////////////////////////////////////////////////////////
// 
// generate_mss.c - generates mss file
//				  - drivers to comunicate with hardware
//
// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	/*

		argv[1] = path to folder
 	 */	

	FILE *mss;

	char path[1024];
	
	strcpy(path, argv[1]);
	strcat(path, "EDK/system.mss");

	mss = fopen(path, "w");
		
	fprintf(mss,"%s","PARAMETER VERSION = 2.2.0\n"
					"\n" 	
					"\n"
					"BEGIN OS\n"
					" PARAMETER OS_NAME = standalone\n"
					" PARAMETER OS_VER = 2.00.a\n"
					" PARAMETER PROC_INSTANCE = microblaze_0\n"
					" PARAMETER STDIN = RS232_DCE\n"
					" PARAMETER STDOUT = RS232_DCE\n"
					"END\n"
					"\n"
					"\n"
					"BEGIN PROCESSOR\n"
					" PARAMETER DRIVER_NAME = cpu\n"
					" PARAMETER DRIVER_VER = 1.11.a\n"
					" PARAMETER HW_INSTANCE = microblaze_0\n"
					" PARAMETER COMPILER = mb-gcc\n"
					" PARAMETER ARCHIVER = mb-ar\n"
					" PARAMETER CORE_CLOCK_FREQ_HZ = 50000000\n"
					"END\n"
					"\n"
					"\n"
					"BEGIN DRIVER\n"
					" PARAMETER DRIVER_NAME = bram\n"
					" PARAMETER DRIVER_VER = 1.00.a\n"
					" PARAMETER HW_INSTANCE = dlmb_cntlr\n"
					"END\n"
					"\n"
					"BEGIN DRIVER\n"
					" PARAMETER DRIVER_NAME = bram\n"
					" PARAMETER DRIVER_VER = 1.00.a\n"
					" PARAMETER HW_INSTANCE = ilmb_cntlr\n"
					"END\n"
					"\n"
					"BEGIN DRIVER\n"
					" PARAMETER DRIVER_NAME = generic\n"
					" PARAMETER DRIVER_VER = 1.00.a\n"
					" PARAMETER HW_INSTANCE = lmb_bram\n"
					"END\n"
					"\n"
					"BEGIN DRIVER\n"
					" PARAMETER DRIVER_NAME = uartlite\n"
					" PARAMETER DRIVER_VER = 1.12.a\n"
					" PARAMETER HW_INSTANCE = RS232_DCE\n"
					"END\n"
					"\n"
					"BEGIN DRIVER\n"
					" PARAMETER DRIVER_NAME = tmrctr\n"
					" PARAMETER DRIVER_VER = 1.10.b\n"
					" PARAMETER HW_INSTANCE = xps_timer_1\n"
					"END\n"
					"\n"
					"BEGIN DRIVER\n"
					" PARAMETER DRIVER_NAME = generic\n"
					" PARAMETER DRIVER_VER = 1.00.a\n"
					" PARAMETER HW_INSTANCE = clock_generator_0\n"
					"END\n"
					"\n"
					"BEGIN DRIVER\n"
					" PARAMETER DRIVER_NAME = uartlite\n"
					" PARAMETER DRIVER_VER = 1.12.a\n"
					" PARAMETER HW_INSTANCE = debug_module\n"
					"END\n"
					"\n"
					"BEGIN DRIVER\n"
					" PARAMETER DRIVER_NAME = generic\n"
					" PARAMETER DRIVER_VER = 1.00.a\n"
					" PARAMETER HW_INSTANCE = proc_sys_reset_0\n"
					"END\n"
					"\n"
					"BEGIN DRIVER\n"
					" PARAMETER DRIVER_NAME = fsl_hello\n" //architecture name - HARDCODED
					" PARAMETER DRIVER_VER = 1.00.a\n"
					" PARAMETER HW_INSTANCE = fsl_hello_0\n"
					"END\n"
					"\n"
	); 
	fclose(mss);   	   
	return 0;
}

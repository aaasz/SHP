///////////////////////////////////////////////////////////////////////////////
// 
// generate_mhs.c - generates mhs file
//				  - two fsl-s		
//   			  - a timer	
//
// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	/*

		argv[1] = path to folder
		argv[2] = -sw or -hw
	if (hw)
	{
		argv[3] = no_master_fsl
		argv[4] = no_slave_fsl
	}
 	*/	

	FILE *mhs;

	char path[1024];
	char buffer[20];
	
	int i;
	int mfsl = atoi(argv[3]);
	int sfsl = atoi(argv[4]);

	strcpy(path, argv[1]);
	strcat(path, "EDK/system.mhs");

	mhs = fopen(path, "w");
		
	fprintf(mhs,"%s","PARAMETER VERSION = 2.1.0\n"
					 "\n"
					 "\n"
					 "PORT fpga_0_RS232_DCE_RX_pin = fpga_0_RS232_DCE_RX, DIR = I\n"
 					 "PORT fpga_0_RS232_DCE_TX_pin = fpga_0_RS232_DCE_TX, DIR = O\n"
 					 "PORT sys_clk_pin = dcm_clk_s, DIR = I, SIGIS = CLK, CLK_FREQ = 50000000\n"
					 "PORT sys_rst_pin = sys_rst_s, DIR = I, RST_POLARITY = 1, SIGIS = RST\n"
					 "\n"
					 "\n"
					 "BEGIN microblaze\n"
					 " PARAMETER INSTANCE = microblaze_0\n"
					 " PARAMETER HW_VER = 7.10.a\n"
					 " PARAMETER C_DEBUG_ENABLED = 1\n"
 					 " PARAMETER C_AREA_OPTIMIZED = 1\n");

	if (!strcmp(argv[2],"hw"))
	{
		sprintf(buffer, "%d", mfsl + sfsl);
		fprintf(mhs,"%s %s\n", " PARAMETER C_FSL_LINKS =", buffer);
	
	}

	fprintf(mhs,"%s"," PARAMETER C_FAMILY = spartan3e\n"
					 " PARAMETER C_INSTANCE = microblaze_0\n"
				     " BUS_INTERFACE DPLB = mb_plb\n"
					 " BUS_INTERFACE IPLB = mb_plb\n"
					 " BUS_INTERFACE DEBUG = microblaze_0_dbg\n"
					 " BUS_INTERFACE DLMB = dlmb\n"
 					 " BUS_INTERFACE ILMB = ilmb\n");

	if (!strcmp(argv[2],"hw"))
	{

		for (i = 0; i < mfsl; i++)
		{
			sprintf(buffer, "%d", i);
			fprintf(mhs,"%s%s%s%s\n", " BUS_INTERFACE MFSL", buffer, " = fsl_v20_",buffer);
			
		}

		for (i = mfsl; i < mfsl + sfsl; i++)
		{
			sprintf(buffer, "%d", i);
			fprintf(mhs,"%s%s%s%s\n", " BUS_INTERFACE SFSL", buffer, " = fsl_v20_",buffer);			
		}

//		fprintf(mhs,"%s", " BUS_INTERFACE MFSL0 = fsl_v20_0\n"
//					 	  " BUS_INTERFACE SFSL1 = fsl_v20_1\n");
	}

	fprintf(mhs,"%s"," PORT MB_RESET = mb_reset\n"
					 "END\n"
					 "\n"
					 "BEGIN plb_v46\n"
 					 " PARAMETER INSTANCE = mb_plb\n"
					 " PARAMETER HW_VER = 1.02.a\n"
					 " PORT PLB_Clk = sys_clk_s\n"
					 " PORT SYS_Rst = sys_bus_reset\n"
					 "END\n"
					 "\n"	
					 "BEGIN lmb_v10\n"
				     " PARAMETER INSTANCE = ilmb\n"
					 " PARAMETER HW_VER = 1.00.a\n"
					 " PORT LMB_Clk = sys_clk_s\n"
					 " PORT SYS_Rst = sys_bus_reset\n"
					 "END\n"
					 "\n"
					 "BEGIN lmb_v10\n"
 					 " PARAMETER INSTANCE = dlmb\n"
					 " PARAMETER HW_VER = 1.00.a\n"
					 " PORT LMB_Clk = sys_clk_s\n"
					 " PORT SYS_Rst = sys_bus_reset\n"
					 "END\n"
					 "\n"
					 "BEGIN lmb_bram_if_cntlr\n"
					 " PARAMETER INSTANCE = dlmb_cntlr\n"
					 " PARAMETER HW_VER = 2.10.a\n"
					 " PARAMETER C_BASEADDR = 0x00000000\n"
					 " PARAMETER C_HIGHADDR = 0x00007fff\n"
				  	 " BUS_INTERFACE SLMB = dlmb\n"
				 	 " BUS_INTERFACE BRAM_PORT = dlmb_port\n"
					 "END\n"
					 "\n"
					 "BEGIN lmb_bram_if_cntlr\n"
					 " PARAMETER INSTANCE = ilmb_cntlr\n"
					 " PARAMETER HW_VER = 2.10.a\n"
					 " PARAMETER C_BASEADDR = 0x00000000\n"
					 " PARAMETER C_HIGHADDR = 0x00007fff\n"
					 " BUS_INTERFACE SLMB = ilmb\n"
					 " BUS_INTERFACE BRAM_PORT = ilmb_port\n"
					 "END\n"
					 "\n"
					 "BEGIN bram_block\n"
					 " PARAMETER INSTANCE = lmb_bram\n"
					 " PARAMETER HW_VER = 1.00.a\n"
				 	 " BUS_INTERFACE PORTA = ilmb_port\n"
					 " BUS_INTERFACE PORTB = dlmb_port\n"
				 	 "END\n"
					 "\n"
					 "BEGIN xps_uartlite\n"
					 " PARAMETER INSTANCE = RS232_DCE\n"
					 " PARAMETER HW_VER = 1.00.a\n"
					 " PARAMETER C_BAUDRATE = 115200\n"
					 " PARAMETER C_DATA_BITS = 8\n"
					 " PARAMETER C_ODD_PARITY = 0\n"
					 " PARAMETER C_USE_PARITY = 0\n"
					 " PARAMETER C_SPLB_CLK_FREQ_HZ = 50000000\n"
				 	 " PARAMETER C_BASEADDR = 0x84000000\n"
					 " PARAMETER C_HIGHADDR = 0x8400ffff\n"
					 " BUS_INTERFACE SPLB = mb_plb\n"
					 " PORT RX = fpga_0_RS232_DCE_RX\n"
					 " PORT TX = fpga_0_RS232_DCE_TX\n"
					 "END\n"
					 "\n"
					 "BEGIN xps_timer\n"
					 " PARAMETER INSTANCE = xps_timer_1\n"
					 " PARAMETER HW_VER = 1.00.a\n"
					 " PARAMETER C_COUNT_WIDTH = 32\n"
					 " PARAMETER C_ONE_TIMER_ONLY = 1\n"
					 " PARAMETER C_BASEADDR = 0x83c00000\n"
					 " PARAMETER C_HIGHADDR = 0x83c0ffff\n"
					 " BUS_INTERFACE SPLB = mb_plb\n"
					 "END\n"
					 "\n"
					 "BEGIN clock_generator\n"
					 " PARAMETER INSTANCE = clock_generator_0\n"
					 " PARAMETER HW_VER = 2.00.a\n"
					 " PARAMETER C_EXT_RESET_HIGH = 1\n"
					 " PARAMETER C_CLKIN_FREQ = 50000000\n"
					 " PARAMETER C_CLKOUT0_FREQ = 50000000\n"
					 " PARAMETER C_CLKOUT0_BUF = TRUE\n"
					 " PARAMETER C_CLKOUT0_PHASE = 0\n"
					 " PARAMETER C_CLKOUT0_GROUP = NONE\n"
					 " PORT CLKOUT0 = sys_clk_s\n"
					 " PORT CLKIN = dcm_clk_s\n"
					 " PORT LOCKED = Dcm_all_locked\n"
					 " PORT RST = net_gnd\n"
					 "END\n"
					 "\n"
					 "BEGIN mdm\n"
					 " PARAMETER INSTANCE = debug_module\n"
					 " PARAMETER HW_VER = 1.00.b\n"
					 " PARAMETER C_MB_DBG_PORTS = 1\n"
				 	 " PARAMETER C_USE_UART = 1\n"
					 " PARAMETER C_UART_WIDTH = 8\n"
					 " PARAMETER C_BASEADDR = 0x84400000\n"
					 " PARAMETER C_HIGHADDR = 0x8440ffff\n"
					 " BUS_INTERFACE SPLB = mb_plb\n"
					 " BUS_INTERFACE MBDEBUG_0 = microblaze_0_dbg\n"
					 " PORT Debug_SYS_Rst = Debug_SYS_Rst\n"
					 "END\n"
					 "\n"
					 "BEGIN proc_sys_reset\n"
					 " PARAMETER INSTANCE = proc_sys_reset_0\n"
					 " PARAMETER HW_VER = 2.00.a\n"
					 " PARAMETER C_EXT_RESET_HIGH = 1\n"
					 " PORT Slowest_sync_clk = sys_clk_s\n"
					 " PORT Dcm_locked = Dcm_all_locked\n"
					 " PORT Ext_Reset_In = sys_rst_s\n"
					 " PORT MB_Reset = mb_reset\n"
					 " PORT Bus_Struct_Reset = sys_bus_reset\n"
					 " PORT MB_Debug_Sys_Rst = Debug_SYS_Rst\n"
					 " PORT Peripheral_Reset = sys_periph_reset\n"
					 "END\n"
				 	 "\n");

	if (!strcmp(argv[2],"hw"))
	{
		fprintf(mhs,"%s", "BEGIN fsl_hello\n" //numele arhitecturii -HARDCODED
					 " PARAMETER INSTANCE = fsl_hello_0\n"
					 " PARAMETER HW_VER = 1.00.a\n");
		
		for (i = mfsl; i < mfsl + sfsl; i++)
		{
			sprintf(buffer, "%d", i);
			fprintf(mhs,"%s%s%s%s\n", " BUS_INTERFACE MFSL", buffer, " = fsl_v20_",buffer);			
		}

		for (i = 0; i < mfsl; i++)
		{
			sprintf(buffer, "%d", i);
			fprintf(mhs,"%s%s%s%s\n", " BUS_INTERFACE SFSL", buffer, " = fsl_v20_",buffer);
			
		}
	
		fprintf(mhs, "%s", " PORT FSL_Clk = sys_clk_s\n"
					 " PORT FSL_Rst = net_gnd\n"
					 "END\n"
					 "\n");

		for (i = 0; i < mfsl + sfsl; i++)
		{
					sprintf(buffer, "%d", i);
		
					 fprintf(mhs, "%s%s%s\n%s", "BEGIN fsl_v20\n",
					 " PARAMETER INSTANCE = fsl_v20_", buffer,
					 " PARAMETER HW_VER = 2.11.a\n"
					 " PORT SYS_Rst = sys_bus_reset\n"
					 " PORT FSL_Clk = sys_clk_s\n"
					 "END\n"
 					 "\n");
		}
 /*					 "BEGIN fsl_v20\n"
					 " PARAMETER INSTANCE = fsl_v20_1\n"
					 " PARAMETER HW_VER = 2.11.a\n"
					 " PORT SYS_Rst = sys_bus_reset\n"
					 " PORT FSL_Clk = sys_clk_s\n"
					 "END\n"
   					 "\n"); 
*/	}
	fclose(mhs);   	   
	return 0;
}

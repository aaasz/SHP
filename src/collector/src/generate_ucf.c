///////////////////////////////////////////////////////////////////////////////
// 
// generate_ucf.c - generates the ucf file
//				  - modify this source to add other constraints	
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

	FILE *ucf;

	char path[1024];
	
	strcpy(path, argv[1]);
	strcat(path, "EDK/data/system.ucf");

	ucf = fopen(path, "w");
		
	fprintf(ucf,"%s","Net sys_clk_pin LOC=c9;\n"
					"Net sys_clk_pin IOSTANDARD = LVCMOS33;\n"
					"Net sys_rst_pin LOC=K17;\n"
					"Net sys_rst_pin IOSTANDARD = LVCMOS33;\n"
					"Net sys_rst_pin PULLDOWN;\n"
					"## System level constraints\n"
					"Net sys_clk_pin TNM_NET = sys_clk_pin;\n"
					"TIMESPEC TS_sys_clk_pin = PERIOD sys_clk_pin 20000 ps;\n"
					"Net sys_rst_pin TIG;\n"
					"\n"
					"## IO Devices constraints\n"
					"\n"
					"#### Module RS232_DCE constraints\n"
					"\n"
					"Net fpga_0_RS232_DCE_RX_pin LOC=R7;\n"
					"Net fpga_0_RS232_DCE_RX_pin IOSTANDARD = LVCMOS33;\n"
					"Net fpga_0_RS232_DCE_TX_pin LOC=M14;\n"
					"Net fpga_0_RS232_DCE_TX_pin IOSTANDARD = LVCMOS33;\n"
					"\n"
	); 
	fclose(ucf);   	   
	return 0;
}

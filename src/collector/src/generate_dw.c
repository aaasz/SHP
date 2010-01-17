///////////////////////////////////////////////////////////////////////////////
// 
// generate_dw.c - generates impact batch command file
//   
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

	FILE *dw;
  
	char path[1024];
	
	strcpy(path, argv[1]);
	strcat(path, "EDK/etc/download.cmd");
	
	printf("%s\n", path);

	dw = fopen(path, "w");
	
	printf("%s\n", path);		

	fprintf(dw,"%s","setMode -bscan\n"
					 "setCable -p auto\n"
					 "identify\n"
			);
	

	strcpy(path, "assignFile -p 1 -file \"");
	strcat(path, argv[1]);
	strcat(path, "EDK/implementation/download.bit\"");

	fprintf(dw, "%s\n", path);
	fprintf(dw, "%s", "assignFile -p 2 -file \"/opt/Xilinx/10.1/ISE/xcf/data/xcf04s.bsd\"\n"
					  "assignFile -p 3 -file \"/opt/Xilinx/10.1/ISE/xbr/data/xc2c64a.bsd\"\n"
					  "program -p 1\n"
					  "quit\n"
			);	

	fclose(dw);   	   
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// 
// generate_xmp.c - generates XPS project file
//   
//
// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_files(FILE* proj, int arg_no, char** argv, char* type)
{
	int i;

	i = arg_no + 1;
	
	if (atoi(argv[arg_no]) == 0)
		return i;

	while (i <= arg_no + atoi(argv[arg_no]))
	{
		fprintf(proj, "%s%s%s\n", type, ": code/", argv[i]);
		i++;	
	}				
	
	return i;
}


int main(int argc, char** argv)
{
	/*

		argv[1] = path to folder
		argv[2] = application name
		argv[3] = -sw or -hw
		
		argv[4] = number of hardware sources	
		argv[5] .. argv[i1] = names of hardware sources
		argv[i1 + 1] = number of hardware headers
		argv[i1+2] .. argv[i2] = names of hardware headers
 		argv[i2+1] = number of software sources	
		argv[i2+2] .. argv[i3] = names of software sources
		argv[i3 + 1] = number of software headers
		argv[i3+2] .. argv[i4] = names of software headers

		Ex. generate_xmp "/home/szekeres/tutorials/HelloWorld_MicroBlaze6/" 
						  "HelloWorld"
						  -hw
						  1
						  "HelloWorld_hw.c"
						  0	
						  2
						  "HelloWorld.c"
						  "HelloWorld_sw.c"
						  0					
 	 */	

	FILE *xmp;
    int arg_no;	    

	char path[1024];
	
	strcpy(path, argv[1]);
	strcat(path, "EDK/system.xmp");

	xmp = fopen(path, "w");
		
	fprintf(xmp,"%s","XmpVersion: 10.1\n"
					  "VerMgmt: 10.1\n"
					  "IntStyle: default\n"
					  "MHS File: system.mhs\n"
					  "MSS File: system.mss\n"
					  "NPL File: projnav/system.ise\n"
					  "Architecture: spartan3e\n"
					  "Device: xc3s500e\n"
					  "Package: fg320\n"
					  "SpeedGrade: -4\n"
					  "UserCmd1:\n" 
					  "UserCmd1Type: 0\n"
					  "UserCmd2:\n" 
					  "UserCmd2Type: 0\n"
					  "TopInst: system_i\n"
					  "GenSimTB: 0\n"
					  "InsertNoPads: 0\n"
					  "WarnForEAArch: 1\n"
					  "HdlLang: VHDL\n"
					  "Simulator: mti\n"
					  "SimModel: BEHAVIORAL\n"
					  "MixLangSim: 1\n"
					  "UcfFile: data/system.ucf\n"
					  "FpgaImpMode: 0\n"
					  "EnableParTimingError: 1\n"
					  "EnableResetOptimization: 0\n"
					  "ShowLicenseDialog: 1\n"
					  "Processor: microblaze_0\n"
					  "BootLoop: 0\n"
					  "XmdStub: 0\n"
		);

	arg_no = 4 + atoi(argv[4]) + 1;
	arg_no = arg_no + atoi(argv[arg_no]) + 1;
	
	fprintf(xmp, "%s%s\n", "SwProj: ", argv[2]);
	fprintf(xmp, "%s", "Processor: microblaze_0\n");
	fprintf(xmp, "%s%s%s\n", "Executable: ", argv[2], "/executable.elf");
	
	if (!strcmp(argv[3],"hw"))
		fprintf(xmp,"%s", "Source: code/co_init.c\n"); //generated by Impulse C Compiler
	
	arg_no = print_files(xmp, arg_no, argv, "Source");
	arg_no = print_files(xmp, arg_no, argv, "Header");
		
	fprintf(xmp, "%s","DefaultInit: EXECUTABLE\n"
					  "InitBram: 1\n"
					  "Active: 1\n"
					  "CompilerOptLevel: 2\n"
					  "GlobPtrOpt: 0\n"
					  "DebugSym: 1\n"
					  "ProfileFlag: 0\n"
					  "ProgStart:\n" 
					  "StackSize: 0x400\n"
					  "HeapSize: 0x400\n"
					  "LinkerScript:\n" 
					  "ProgCCFlags:\n" 
					  "CompileInXps: 1\n"
					  "NonXpsApp: 0\n"								  	
	);
	fclose(xmp);   	   
	return 0;
}

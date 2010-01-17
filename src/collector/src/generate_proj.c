///////////////////////////////////////////////////////////////////////////////
// 
// generate_proj.c - generates CoDeveloper project file
//   
//
// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_files(FILE* proj, int arg_no, char** argv)
{
	int i;

	i = arg_no + 1;

	if (atoi(argv[arg_no]) == 0)
	{
		fprintf(proj, "\"\n");
		return i;
	}

	while (i < arg_no + atoi(argv[arg_no]))
	{
		fprintf(proj, "%s ", argv[i]);
		i++;	
	}				
	fprintf(proj, "%s\"\n", argv[i]);
	
	return i + 1;
}


int main(int argc, char** argv)
{
	/*

		argv[1] = path to folder
		argv[2] = name of the generated file / application
		argv[3] = -sw or -hw	
		argv[4] = no of master fsl
		argv[5] = no of slave fsl
		argv[6] = number of hardware sources	
		argv[7] .. argv[i1] = names of hardware sources
		argv[i1 + 1] = number of hardware headers
		argv[i1+2] .. argv[i2] = names of hardware headers
 		argv[i2+1] = number of software sources	
		argv[i2+2] .. argv[i3] = names of software sources
		argv[i3 + 1] = number of software headers
		argv[i3+2] .. argv[i4] = names of software headers

		Ex. generate_proj "/home/szekeres/tutorials/HelloWorld_MicroBlaze6/" 
						  "HelloWorld"
						  1
						  "HelloWorld_hw.c"
						  0	
						  2
						  "HelloWorld.c"
						  "HelloWorld_sw.c"
						  0					
 	 */	

	FILE *proj;
    int arg_no;	    

	char path[1024];
	
	strcpy(path, argv[1]);
	strcat(path, argv[2]);
	strcat(path, ".icProj");

	proj = fopen(path, "w");
		
	fprintf(proj,"%s","$Version \"2.10.b.1\"\n"
					 	"$Option \"GenerateMakefile=1\"\n"
					  	"$Option \"MakeDebug=1\"\n"
						"$Option \"DefaultMakeFile=HelloWorld.mak\"\n"
						"$Option \"DefaultMakeGenerateTarget=build\"\n"
						"$Option \"DefaultMakeCleanTarget=clean\"\n"
						"$Option \"DefaultMakeExportHWTarget=export_hardware\"\n"
						"$Option \"DefaultMakeExportSWTarget=export_software\"\n"
						"$Option \"HWBuildSrcFiles="
			);

	arg_no = print_files(proj, 6, argv);	

	fprintf(proj, "%s",	"$Option \"HWBuildIncFiles=");
	arg_no = print_files(proj, arg_no, argv);	


	fprintf(proj, "%s",	"$Option \"SWBuildSrcFiles=");
	arg_no = print_files(proj, arg_no, argv);	
	
	fprintf(proj, "%s",	"$Option \"SWBuildIncFiles=");
	arg_no = print_files(proj, arg_no, argv);	
	
	fprintf(proj, "%s", "$Option \"ExeBuildSrcFiles=\"\n"
						"$Option \"ExeBuildIncFiles=\"\n"
						"$Option \"ExeBuildLibFiles=\"\n"
						"$Option \"ExeBuildCCOpts=\"\n"
						"$Option \"LaunchUpdateExe=1\"\n"
						"$Option \"LaunchUpdateHwExe=1\"\n"
						"$Option \"LaunchMonitor=0\"\n"
						"$Option \"SimulateExe=HelloWorld.exe\"\n"
						"$Option \"SimulateHwExe=\"\n"
						"$Option \"SimulateArgs=\"\n"
						"$Option \"SimulateDir=\"\n"
						"$Option \"GenCodeConstProp=0\"\n"
						"$Option \"GenCodeScalarize=0\"\n"
						"$Option \"GenCodeDualClocks=0\"\n"
						"$Option \"GenCodeStdLogic=0\"\n"
						"$Option \"GenCodeCoPort=1\"\n"
						"$Option \"GenCodeFloat=0\"\n"
						"$Option \"GenCodeFloatPipelined=0\"\n"
						"$Option \"GenCodeFloatDouble=0\"\n"
						"$Option \"GenCodeLoopInvariants=0\"\n"
						"$Option \"GenCodeFamily=Xilinx MicroBlaze FSL (VHDL)\"\n"
						"$Option \"GenCodeArchID=xilinx_mb_fsl\"\n"
						"$Option \"GenCodeHWExportDir=EDK\"\n"
						"$Option \"GenCodeHWTargetDir=hw\"\n"
						"$Option \"GenCodeSWExportDir=EDK\"\n"
						"$Option \"GenCodeSWTargetDir=sw\"\n"
						"$Option \"GenCodePFlagsOpt=\"\n"
						"$Option \"UseExternalMakeWin=0\"\n"
						//"$Files\n"
						//"($Application \"HelloWorld\"\n"
						//" ($Sourcefiles\n"
						//"  ($Module \".\\HelloWorld_sw.c\" \"\"\n"
						//"  $Module \".\\HelloWorld_hw.c\" \"\"\n"
						//"  $Module \".\\HelloWorld.c\" \"\"\n"
						//"  )\n"
						//" $Headerfiles\n"
						//" $Projectfiles\n"
						//" $Docfiles\n"
						//"  ($Module \".\\Readme.htm\" \"\"\n"
						//"  )\n"
						//" $Otherfiles\n"
						//" )\n"
						//")\n"										  	
	);
	fclose(proj);   	   
	return 0;
}

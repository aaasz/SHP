/*
 * GraphDataAppender.cpp
 *
 *  Created on: Jan 10, 2010
 *      Author: Silviu
 */


#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "GraphDataAppender.h"


GraphDataAppender::GraphDataAppender(ProcessGraph *pg,std::string test_path) {
	// TODO Auto-generated constructor stub
	
	std::cout << "numVertices " << pg->getNumVertices()<<std::endl;

	for (int i = 0; i < pg->getNumVertices(); i++) {
		ProcessGraphNode *gn = pg->lookupGraphNode(i);
		if (gn->isHardwareImplementable()) {
			int nrFSLIn = 0;
			//int nrFSLOut = 0;


			std::vector<DataStream *> *streams = gn->getStreams();
			for (int j = 0; j < streams->size(); j++) {
				DataStream *ds = (*streams)[j];
				//std::cout<<ds->getVarDecl()->getNameAsString()<<"\n";
				std::cout << "start process: "
						<< ds->getStartProcess()->getName() << " end: "<<
						ds->getEndProcess()->getName()<<"\n";
				if (ds->getStartProcess() == gn){
				//	if (ds->getEndProcess()->isHardwareImplementable())
				//		nrFSLOut++;
				}
				else if (ds->getEndProcess() == gn){
					//if (ds->getStartProcess()->isHardwareImplementable())
						nrFSLIn++;
				}
			}


			std::string name = gn->getName();
			std::string copy_cmd;

			
			copy_cmd = "cp splitterOutput/hw/" + name + ".c " + test_path + "/" + name + "/hw/";
			std::cout << copy_cmd << std::endl;
			//system(copy_cmd.c_str());
				
			copy_cmd = "cp splitterOutput/sw/" + name + ".c " + test_path + "/"
					+ name + "/sw/";
			std::cout << copy_cmd << std::endl;

			//system(copy_cmd.c_str());

			std::string params;
			std::stringstream ss (std::stringstream::in | std::stringstream::out);

			ss << nrFSLIn<<" "<<1;// << " " << nrFSLOut;
			std::string s1,s2;
			ss >> s1;
			ss >> s2;
			//fslIN[9]='\0';
			//fslOUT[9]='\0';

			//sprintf(fslIN,"%d\0",&nrFSLIn);
			//sprintf(fslOUT,"%d\0",&nrFSLOut);


			std::string command = "";
			command = "/home/szekeres/SHP/collector/run.sh \"" + test_path + "/" + name + "/hw/" + "\" \""
					+ name + "\" \"hw\" "
					+s1+" "+s2+" 2 \"" +  "config_" + name + ".c\" \""
					+ name + ".c\" \"stub_function.c\" 0 2 \"test_" + name + ".c\" \"main.c\" 0 > "
					+ test_path + "/" + name + "/hw/output.txt";
			std::cout << command << std::endl;
			//system(command.c_str());
			command = "/home/szekeres/SHP/collector/parse.sh \"" + test_path + "/" + name
					+ "/hw\" > \"" + test_path + "/" + name
					+ "/hw/parameters.txt\"";
			std::cout << command << std::endl;
			//system(command.c_str());

			std::string output_path = test_path + "/" + name
					+ "/hw/parameters.txt";
			std::cout << "output path is : "<<output_path<<"\n";
			VertexData vd;
			
						
			std::ifstream myfile(output_path.c_str());
			unsigned int tmp;
			myfile >> vd.communicationTime;
			myfile >> vd.hadrwareExecutionTime;
			myfile >> vd.LUTS;
			myfile >> tmp;
			pg->setMaxLUTS(tmp);
			myfile >> vd.SLICES;
			myfile >> tmp;
			pg->setMaxSLICES(tmp);
			myfile.close();

			//std::cout << "citit "<<vd.communicationTime << " " << vd.hadrwareExecutionTime << " " << vd.LUTS << " " << vd.SLICES << "\n";
			//std::cout << "citit max luts,slices "<<pg->getMaxLUTS()<<" "<<pg->getMaxSLICES()<<std::endl;

			command = "/home/szekeres/SHP/collector/run.sh \"" + test_path + "/" + name + "/sw/" + "\" \""
					+ name + "\" " + "\"sw\" 0 0 0 0 2 \"test_" + name + ".c\" \""
					+ name + ".c\" \"stub_function.c\" 0 > " + test_path + "/" + name
					+ "/sw/output.txt";
			std::cout << command << std::endl;
			//system(command.c_str());

			command = "/home/szekeres/SHP/collector/parse.sh \"" + test_path + "/" + name
					+ "/sw\" > \"" + test_path + "/" + name
					+ "/sw/parameters.txt\"";
			std::cout << command << std::endl;
			//system(command.c_str());

			output_path = test_path + "/" + name + "/sw/parameters.txt";
			//std::ifstream myfilesw(output_path.c_str());
			
			FILE *f = fopen(output_path.c_str(),"rt");
			if (!f){
				std::cout << "nu am deschis fisierul\n";
				exit(-1);
			}
			VertexData vd1;
			fscanf(f,"%u %u",&vd1.communicationTime,&vd.softwareExecutionTime);
			fscanf(f,"%u",&vd1.LUTS);
			pg->setProcessorLUTS(vd1.LUTS);
			fscanf(f,"%u",&tmp);
			fscanf(f,"%u",&vd1.SLICES);
			pg->setProcessorSLICES(vd1.SLICES);
			fscanf(f,"%u",&tmp);
			fclose(f);	

			//std::cout << "citit "<<vd.softwareExecutionTime << "\n";

			gn->setVertexData(vd);


		} else {
			VertexData vd1;
			vd1.LUTS=0;
			vd1.SLICES=0;
			vd1.hadrwareExecutionTime = INT_MAX;
			vd1.softwareExecutionTime = 0;
			vd1.communicationTime = 0;
			gn->setVertexData(vd1);
		}
	}
	for (int i = 0; i < pg->getNumVertices(); i++){
		ProcessGraphNode *gn = pg->lookupGraphNode(i);
		if (gn->isHardwareImplementable()) {
			VertexData vd = gn->getVertexData();
			vd.LUTS-=pg->getProcessorLUTS();
			vd.SLICES-=pg->getProcessorSLICES();
			gn->setVertexData(vd);
		}
	}
}

GraphDataAppender::~GraphDataAppender() {
	// TODO Auto-generated destructor stub
}

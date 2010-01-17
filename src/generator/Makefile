CLANG_PATH=/usr/local
LLVM_PATH=/usr/local
TOOLNAME = callgraph

LibDir      := $(CLANG_PATH)/lib
LLVMLibDir  := $(LLVM_PATH)/lib

CPP.BaseFlags += -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS
LD.Flags      += -L$(LibDir) -L$(LLVMLibDir)

CPPFLAGS += -I$(CLANG_PATH)/include -I$(LLVM_PATH)/include $(CPP.BaseFlags)
CXXFLAGS = -fno-rtti -g

USEDLIBS = -lclangIndex -lclangFrontend -lclangDriver -lclangSema \
           -lclangAnalysis -lclangAST -lclangParse -lclangLex -lclangBasic \
	   -lLLVMMC -lLLVMBitReader -lLLVMCore -lLLVMSupport \
	   -lLLVMSystem  -ldl -lgaul -lgaul_util -lpthread -lm 
	
OBJECTS = callgraph.o UsedDefsPrinter.o Splitter.o ProgramAnalyzer.o ConfigAnalyzer.o ConfigDeclAnalyzer.o ProcessGraph.o StreamDirectionAnalyzer.o Partitioner.o GraphDataAppender.o HardwareImplementableAnalyzer.o

all:	link		
clean:
	rm -f *.o $(TOOLNAME)
link:	$(OBJECTS)
	g++ -fno-exceptions -fPIC -Woverloaded-virtual  -fno-rtti    -pedantic -Wno-long-long \
	   	-Wall -W -Wno-unused-parameter -Wwrite-strings  -g  -Wl,-R \
	   	-Wl,-export-dynamic \
	   	$(LD.Flags) \
	   	$(OBJECTS) \
	   	$(USEDLIBS) \
	   	-o $(TOOLNAME)
run:
	./$(TOOLNAME) input03.ast

	
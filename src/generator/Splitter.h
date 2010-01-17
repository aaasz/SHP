/*
 * Splitter.h
 *
 *  Created on: Dec 17, 2009
 *      Author: Silviu
 */

#ifndef SPLITTER_H_
#define SPLITTER_H_
#include <string>
#include <vector>
#include <clang/Frontend/TextDiagnosticBuffer.h>
#include <clang/Frontend/ASTUnit.h>
#include "ProcessGraph.h"
class Splitter {
public:
	Splitter(const std::string &InFile);
	void splitFiles(bool analize,std::string subdir);
	bool hasErrors();
	std::string isCoHDefenined(clang::PresumedLoc *pl,llvm::raw_fd_ostream *out, std::string name);
	std::vector<ProcessGraph *> *getConfigs(){
		return &configs;
	}
	~Splitter();
protected:
	clang::TextDiagnosticBuffer DiagClient;
	llvm::OwningPtr<clang::ASTUnit> AST;
	clang::Diagnostic diagnostic;
	std::string ErrMsg;
	bool errors;
	std::vector<std::string> headers;
	std::vector<ProcessGraph *> configs;
private:
	std::vector<std::string> initHeaders();

	std::string defaultIncludes;
};
#endif /* SPLITTER_H_ */

//===-- XCoreAsmPrinter.cpp - XCore LLVM assembly writer ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the XAS-format XCore assembly language.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "asm-printer"
#include "XCore.h"
#include "XCoreInstrInfo.h"
#include "XCoreSubtarget.h"
#include "XCoreMCAsmInfo.h"
#include "XCoreTargetMachine.h"
#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Module.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/DwarfWriter.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetRegistry.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/Mangler.h"
#include "llvm/Support/MathExtras.h"
#include <algorithm>
#include <cctype>
using namespace llvm;

STATISTIC(EmittedInsts, "Number of machine instrs printed");

static cl::opt<unsigned> MaxThreads("xcore-max-threads", cl::Optional,
  cl::desc("Maximum number of threads (for emulation thread-local storage)"),
  cl::Hidden,
  cl::value_desc("number"),
  cl::init(8));

namespace {
  class XCoreAsmPrinter : public AsmPrinter {
    const XCoreSubtarget &Subtarget;
  public:
    explicit XCoreAsmPrinter(formatted_raw_ostream &O, TargetMachine &TM,
                             const MCAsmInfo *T, bool V)
      : AsmPrinter(O, TM, T, V),
      Subtarget(TM.getSubtarget<XCoreSubtarget>()) {}

    virtual const char *getPassName() const {
      return "XCore Assembly Printer";
    }

    void printMemOperand(const MachineInstr *MI, int opNum);
    void printOperand(const MachineInstr *MI, int opNum);
    bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                        unsigned AsmVariant, const char *ExtraCode);

    void emitGlobalDirective(const std::string &name);
    void emitExternDirective(const std::string &name);
    
    void emitArrayBound(const std::string &name, const GlobalVariable *GV);
    virtual void PrintGlobalVariable(const GlobalVariable *GV);

    void emitFunctionStart(MachineFunction &MF);
    void emitFunctionEnd(MachineFunction &MF);

    void printInstruction(const MachineInstr *MI);  // autogenerated.
    static const char *getRegisterName(unsigned RegNo);

    void printMachineInstruction(const MachineInstr *MI);
    bool runOnMachineFunction(MachineFunction &F);
    
    void getAnalysisUsage(AnalysisUsage &AU) const {
      AsmPrinter::getAnalysisUsage(AU);
      AU.setPreservesAll();
      AU.addRequired<MachineModuleInfo>();
      AU.addRequired<DwarfWriter>();
    }
  };
} // end of anonymous namespace

#include "XCoreGenAsmWriter.inc"

void XCoreAsmPrinter::
emitGlobalDirective(const std::string &name)
{
  O << MAI->getGlobalDirective() << name;
  O << "\n";
}

void XCoreAsmPrinter::
emitExternDirective(const std::string &name)
{
  O << "\t.extern\t" << name;
  O << '\n';
}

void XCoreAsmPrinter::
emitArrayBound(const std::string &name, const GlobalVariable *GV)
{
  assert(((GV->hasExternalLinkage() ||
    GV->hasWeakLinkage()) ||
    GV->hasLinkOnceLinkage()) && "Unexpected linkage");
  if (const ArrayType *ATy = dyn_cast<ArrayType>(
    cast<PointerType>(GV->getType())->getElementType()))
  {
    O << MAI->getGlobalDirective() << name << ".globound" << "\n";
    O << MAI->getSetDirective() << name << ".globound" << ","
      << ATy->getNumElements() << "\n";
    if (GV->hasWeakLinkage() || GV->hasLinkOnceLinkage()) {
      // TODO Use COMDAT groups for LinkOnceLinkage
      O << MAI->getWeakDefDirective() << name << ".globound" << "\n";
    }
  }
}

void XCoreAsmPrinter::PrintGlobalVariable(const GlobalVariable *GV) {
  // Check to see if this is a special global used by LLVM, if so, emit it.
  if (!GV->hasInitializer() ||
      EmitSpecialLLVMGlobal(GV))
    return;

  const TargetData *TD = TM.getTargetData();
  
  OutStreamer.SwitchSection(getObjFileLowering().SectionForGlobal(GV, Mang,TM));
  
  std::string name = Mang->getMangledName(GV);
  Constant *C = GV->getInitializer();
  unsigned Align = (unsigned)TD->getPreferredTypeAlignmentShift(C->getType());
  
  // Mark the start of the global
  O << "\t.cc_top " << name << ".data," << name << "\n";

  switch (GV->getLinkage()) {
  case GlobalValue::AppendingLinkage:
    llvm_report_error("AppendingLinkage is not supported by this target!");
  case GlobalValue::LinkOnceAnyLinkage:
  case GlobalValue::LinkOnceODRLinkage:
  case GlobalValue::WeakAnyLinkage:
  case GlobalValue::WeakODRLinkage:
  case GlobalValue::ExternalLinkage:
    emitArrayBound(name, GV);
    emitGlobalDirective(name);
    // TODO Use COMDAT groups for LinkOnceLinkage
    if (GV->hasWeakLinkage() || GV->hasLinkOnceLinkage()) {
      O << MAI->getWeakDefDirective() << name << "\n";
    }
    // FALL THROUGH
  case GlobalValue::InternalLinkage:
  case GlobalValue::PrivateLinkage:
  case GlobalValue::LinkerPrivateLinkage:
    break;
  case GlobalValue::GhostLinkage:
    llvm_unreachable("Should not have any unmaterialized functions!");
  case GlobalValue::DLLImportLinkage:
    llvm_unreachable("DLLImport linkage is not supported by this target!");
  case GlobalValue::DLLExportLinkage:
    llvm_unreachable("DLLExport linkage is not supported by this target!");
  default:
    llvm_unreachable("Unknown linkage type!");
  }

  EmitAlignment(Align, GV, 2);
  
  unsigned Size = TD->getTypeAllocSize(C->getType());
  if (GV->isThreadLocal()) {
    Size *= MaxThreads;
  }
  if (MAI->hasDotTypeDotSizeDirective()) {
    O << "\t.type " << name << ",@object\n";
    O << "\t.size " << name << "," << Size << "\n";
  }
  O << name << ":\n";
  
  EmitGlobalConstant(C);
  if (GV->isThreadLocal()) {
    for (unsigned i = 1; i < MaxThreads; ++i) {
      EmitGlobalConstant(C);
    }
  }
  if (Size < 4) {
    // The ABI requires that unsigned scalar types smaller than 32 bits
    // are are padded to 32 bits.
    EmitZeros(4 - Size);
  }
  
  // Mark the end of the global
  O << "\t.cc_bottom " << name << ".data\n";
}

/// Emit the directives on the start of functions
void XCoreAsmPrinter::emitFunctionStart(MachineFunction &MF) {
  // Print out the label for the function.
  const Function *F = MF.getFunction();

  OutStreamer.SwitchSection(getObjFileLowering().SectionForGlobal(F, Mang, TM));
  
  // Mark the start of the function
  O << "\t.cc_top " << CurrentFnName << ".function," << CurrentFnName << "\n";

  switch (F->getLinkage()) {
  default: llvm_unreachable("Unknown linkage type!");
  case Function::InternalLinkage:  // Symbols default to internal.
  case Function::PrivateLinkage:
  case Function::LinkerPrivateLinkage:
    break;
  case Function::ExternalLinkage:
    emitGlobalDirective(CurrentFnName);
    break;
  case Function::LinkOnceAnyLinkage:
  case Function::LinkOnceODRLinkage:
  case Function::WeakAnyLinkage:
  case Function::WeakODRLinkage:
    // TODO Use COMDAT groups for LinkOnceLinkage
    O << MAI->getGlobalDirective() << CurrentFnName << "\n";
    O << MAI->getWeakDefDirective() << CurrentFnName << "\n";
    break;
  }
  // (1 << 1) byte aligned
  EmitAlignment(MF.getAlignment(), F, 1);
  if (MAI->hasDotTypeDotSizeDirective()) {
    O << "\t.type " << CurrentFnName << ",@function\n";
  }
  O << CurrentFnName << ":\n";
}

/// Emit the directives on the end of functions
void XCoreAsmPrinter::
emitFunctionEnd(MachineFunction &MF) 
{
  // Mark the end of the function
  O << "\t.cc_bottom " << CurrentFnName << ".function\n";
}

/// runOnMachineFunction - This uses the printMachineInstruction()
/// method to print assembly for each instruction.
///
bool XCoreAsmPrinter::runOnMachineFunction(MachineFunction &MF)
{
  this->MF = &MF;

  SetupMachineFunction(MF);

  // Print out constants referenced by the function
  EmitConstantPool(MF.getConstantPool());

  // Print out jump tables referenced by the function
  EmitJumpTableInfo(MF.getJumpTableInfo(), MF);

  // Emit the function start directives
  emitFunctionStart(MF);
  
  // Emit pre-function debug information.
  DW->BeginFunction(&MF);

  // Print out code for the function.
  for (MachineFunction::const_iterator I = MF.begin(), E = MF.end();
       I != E; ++I) {

    // Print a label for the basic block.
    if (I != MF.begin()) {
      EmitBasicBlockStart(I);
    }

    for (MachineBasicBlock::const_iterator II = I->begin(), E = I->end();
         II != E; ++II) {
      // Print the assembly for the instruction.
      printMachineInstruction(II);
    }

    // Each Basic Block is separated by a newline
    O << '\n';
  }

  // Emit function end directives
  emitFunctionEnd(MF);
  
  // Emit post-function debug information.
  DW->EndFunction(&MF);

  // We didn't modify anything.
  return false;
}

void XCoreAsmPrinter::printMemOperand(const MachineInstr *MI, int opNum)
{
  printOperand(MI, opNum);
  
  if (MI->getOperand(opNum+1).isImm()
    && MI->getOperand(opNum+1).getImm() == 0)
    return;
  
  O << "+";
  printOperand(MI, opNum+1);
}

void XCoreAsmPrinter::printOperand(const MachineInstr *MI, int opNum) {
  const MachineOperand &MO = MI->getOperand(opNum);
  switch (MO.getType()) {
  case MachineOperand::MO_Register:
    O << getRegisterName(MO.getReg());
    break;
  case MachineOperand::MO_Immediate:
    O << MO.getImm();
    break;
  case MachineOperand::MO_MachineBasicBlock:
    GetMBBSymbol(MO.getMBB()->getNumber())->print(O, MAI);
    break;
  case MachineOperand::MO_GlobalAddress:
    O << Mang->getMangledName(MO.getGlobal());
    break;
  case MachineOperand::MO_ExternalSymbol:
    O << MO.getSymbolName();
    break;
  case MachineOperand::MO_ConstantPoolIndex:
    O << MAI->getPrivateGlobalPrefix() << "CPI" << getFunctionNumber()
      << '_' << MO.getIndex();
    break;
  case MachineOperand::MO_JumpTableIndex:
    O << MAI->getPrivateGlobalPrefix() << "JTI" << getFunctionNumber()
      << '_' << MO.getIndex();
  case MachineOperand::MO_BlockAddress:
    GetBlockAddressSymbol(MO.getBlockAddress())->print(O, MAI);
    break;
  default:
    llvm_unreachable("not implemented");
  }
}

/// PrintAsmOperand - Print out an operand for an inline asm expression.
///
bool XCoreAsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                                      unsigned AsmVariant, 
                                      const char *ExtraCode) {
  printOperand(MI, OpNo);
  return false;
}

void XCoreAsmPrinter::printMachineInstruction(const MachineInstr *MI) {
  ++EmittedInsts;

  processDebugLoc(MI, true);

  // Check for mov mnemonic
  unsigned src, dst, srcSR, dstSR;
  if (TM.getInstrInfo()->isMoveInstr(*MI, src, dst, srcSR, dstSR)) {
    O << "\tmov " << getRegisterName(dst) << ", ";
    O << getRegisterName(src) << '\n';
    return;
  }
  printInstruction(MI);
  if (VerboseAsm)
    EmitComments(*MI);
  O << '\n';

  processDebugLoc(MI, false);
}

// Force static initialization.
extern "C" void LLVMInitializeXCoreAsmPrinter() { 
  RegisterAsmPrinter<XCoreAsmPrinter> X(TheXCoreTarget);
}

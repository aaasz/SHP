//===-- MipsAsmPrinter.cpp - Mips LLVM assembly writer --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to GAS-format MIPS assembly language.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "mips-asm-printer"

#include "Mips.h"
#include "MipsSubtarget.h"
#include "MipsInstrInfo.h"
#include "MipsTargetMachine.h"
#include "MipsMachineFunction.h"
#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Module.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/DwarfWriter.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetLoweringObjectFile.h" 
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/Mangler.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/MathExtras.h"
#include <cctype>

using namespace llvm;

STATISTIC(EmittedInsts, "Number of machine instrs printed");

namespace {
  class MipsAsmPrinter : public AsmPrinter {
    const MipsSubtarget *Subtarget;
  public:
    explicit MipsAsmPrinter(formatted_raw_ostream &O, TargetMachine &TM, 
                            const MCAsmInfo *T, bool V)
      : AsmPrinter(O, TM, T, V) {
      Subtarget = &TM.getSubtarget<MipsSubtarget>();
    }

    virtual const char *getPassName() const {
      return "Mips Assembly Printer";
    }

    bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo, 
                         unsigned AsmVariant, const char *ExtraCode);
    void printOperand(const MachineInstr *MI, int opNum);
    void printUnsignedImm(const MachineInstr *MI, int opNum);
    void printMemOperand(const MachineInstr *MI, int opNum, 
                         const char *Modifier = 0);
    void printFCCOperand(const MachineInstr *MI, int opNum, 
                         const char *Modifier = 0);
    void PrintGlobalVariable(const GlobalVariable *GVar);
    void printSavedRegsBitmask(MachineFunction &MF);
    void printHex32(unsigned int Value);

    const char *emitCurrentABIString();
    void emitFunctionStart(MachineFunction &MF);
    void emitFunctionEnd(MachineFunction &MF);
    void emitFrameDirective(MachineFunction &MF);

    void printInstruction(const MachineInstr *MI);  // autogenerated.
    static const char *getRegisterName(unsigned RegNo);

    bool runOnMachineFunction(MachineFunction &F);
    void EmitStartOfAsmFile(Module &M);
  };
} // end of anonymous namespace

#include "MipsGenAsmWriter.inc"

//===----------------------------------------------------------------------===//
//
//  Mips Asm Directives
//
//  -- Frame directive "frame Stackpointer, Stacksize, RARegister"
//  Describe the stack frame.
//
//  -- Mask directives "(f)mask  bitmask, offset" 
//  Tells the assembler which registers are saved and where.
//  bitmask - contain a little endian bitset indicating which registers are 
//            saved on function prologue (e.g. with a 0x80000000 mask, the 
//            assembler knows the register 31 (RA) is saved at prologue.
//  offset  - the position before stack pointer subtraction indicating where 
//            the first saved register on prologue is located. (e.g. with a
//
//  Consider the following function prologue:
//
//    .frame  $fp,48,$ra
//    .mask   0xc0000000,-8
//       addiu $sp, $sp, -48
//       sw $ra, 40($sp)
//       sw $fp, 36($sp)
//
//    With a 0xc0000000 mask, the assembler knows the register 31 (RA) and 
//    30 (FP) are saved at prologue. As the save order on prologue is from 
//    left to right, RA is saved first. A -8 offset means that after the 
//    stack pointer subtration, the first register in the mask (RA) will be
//    saved at address 48-8=40.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Mask directives
//===----------------------------------------------------------------------===//

// Create a bitmask with all callee saved registers for CPU or Floating Point 
// registers. For CPU registers consider RA, GP and FP for saving if necessary.
void MipsAsmPrinter::
printSavedRegsBitmask(MachineFunction &MF)
{
  const TargetRegisterInfo &RI = *TM.getRegisterInfo();
  MipsFunctionInfo *MipsFI = MF.getInfo<MipsFunctionInfo>();
             
  // CPU and FPU Saved Registers Bitmasks
  unsigned int CPUBitmask = 0;
  unsigned int FPUBitmask = 0;

  // Set the CPU and FPU Bitmasks
  MachineFrameInfo *MFI = MF.getFrameInfo();
  const std::vector<CalleeSavedInfo> &CSI = MFI->getCalleeSavedInfo();
  for (unsigned i = 0, e = CSI.size(); i != e; ++i) {
    unsigned RegNum = MipsRegisterInfo::getRegisterNumbering(CSI[i].getReg());
    if (CSI[i].getRegClass() == Mips::CPURegsRegisterClass)
      CPUBitmask |= (1 << RegNum);
    else
      FPUBitmask |= (1 << RegNum);
  }

  // Return Address and Frame registers must also be set in CPUBitmask.
  if (RI.hasFP(MF)) 
    CPUBitmask |= (1 << MipsRegisterInfo::
                getRegisterNumbering(RI.getFrameRegister(MF)));
  
  if (MF.getFrameInfo()->hasCalls()) 
    CPUBitmask |= (1 << MipsRegisterInfo::
                getRegisterNumbering(RI.getRARegister()));

  // Print CPUBitmask
  O << "\t.mask \t"; printHex32(CPUBitmask); O << ','
    << MipsFI->getCPUTopSavedRegOff() << '\n';

  // Print FPUBitmask
  O << "\t.fmask\t"; printHex32(FPUBitmask); O << ","
    << MipsFI->getFPUTopSavedRegOff() << '\n';
}

// Print a 32 bit hex number with all numbers.
void MipsAsmPrinter::
printHex32(unsigned int Value) 
{
  O << "0x";
  for (int i = 7; i >= 0; i--) 
    O << utohexstr( (Value & (0xF << (i*4))) >> (i*4) );
}

//===----------------------------------------------------------------------===//
// Frame and Set directives
//===----------------------------------------------------------------------===//

/// Frame Directive
void MipsAsmPrinter::emitFrameDirective(MachineFunction &MF) {
  const TargetRegisterInfo &RI = *TM.getRegisterInfo();

  unsigned stackReg  = RI.getFrameRegister(MF);
  unsigned returnReg = RI.getRARegister();
  unsigned stackSize = MF.getFrameInfo()->getStackSize();


  O << "\t.frame\t" << '$' << LowercaseString(getRegisterName(stackReg))
                    << ',' << stackSize << ','
                    << '$' << LowercaseString(getRegisterName(returnReg))
                    << '\n';
}

/// Emit Set directives.
const char *MipsAsmPrinter::emitCurrentABIString() {  
  switch(Subtarget->getTargetABI()) {
    case MipsSubtarget::O32:  return "abi32";  
    case MipsSubtarget::O64:  return "abiO64";
    case MipsSubtarget::N32:  return "abiN32";
    case MipsSubtarget::N64:  return "abi64";
    case MipsSubtarget::EABI: return "eabi32"; // TODO: handle eabi64
    default: break;
  }

  llvm_unreachable("Unknown Mips ABI");
  return NULL;
}  

/// Emit the directives used by GAS on the start of functions
void MipsAsmPrinter::emitFunctionStart(MachineFunction &MF) {
  // Print out the label for the function.
  const Function *F = MF.getFunction();
  OutStreamer.SwitchSection(getObjFileLowering().SectionForGlobal(F, Mang, TM));

  // 2 bits aligned
  EmitAlignment(MF.getAlignment(), F);

  O << "\t.globl\t"  << CurrentFnName << '\n';
  O << "\t.ent\t"    << CurrentFnName << '\n';

  printVisibility(CurrentFnName, F->getVisibility());

  if ((MAI->hasDotTypeDotSizeDirective()) && Subtarget->isLinux())
    O << "\t.type\t"   << CurrentFnName << ", @function\n";

  O << CurrentFnName << ":\n";

  emitFrameDirective(MF);
  printSavedRegsBitmask(MF);

  O << '\n';
}

/// Emit the directives used by GAS on the end of functions
void MipsAsmPrinter::emitFunctionEnd(MachineFunction &MF) {
  // There are instruction for this macros, but they must
  // always be at the function end, and we can't emit and
  // break with BB logic. 
  O << "\t.set\tmacro\n"; 
  O << "\t.set\treorder\n"; 

  O << "\t.end\t" << CurrentFnName << '\n';
  if (MAI->hasDotTypeDotSizeDirective() && !Subtarget->isLinux())
    O << "\t.size\t" << CurrentFnName << ", .-" << CurrentFnName << '\n';
}

/// runOnMachineFunction - This uses the printMachineInstruction()
/// method to print assembly for each instruction.
bool MipsAsmPrinter::runOnMachineFunction(MachineFunction &MF) {
  this->MF = &MF;

  SetupMachineFunction(MF);

  // Print out constants referenced by the function
  EmitConstantPool(MF.getConstantPool());

  // Print out jump tables referenced by the function
  EmitJumpTableInfo(MF.getJumpTableInfo(), MF);

  O << "\n\n";

  // Emit the function start directives
  emitFunctionStart(MF);

  // Print out code for the function.
  for (MachineFunction::const_iterator I = MF.begin(), E = MF.end();
       I != E; ++I) {

    // Print a label for the basic block.
    if (I != MF.begin()) {
      EmitBasicBlockStart(I);
    }

    for (MachineBasicBlock::const_iterator II = I->begin(), E = I->end();
         II != E; ++II) {
      processDebugLoc(II, true);

      // Print the assembly for the instruction.
      printInstruction(II);
      
      if (VerboseAsm)
        EmitComments(*II);
      O << '\n';

      processDebugLoc(II, false);      
      ++EmittedInsts;
    }

    // Each Basic Block is separated by a newline
    O << '\n';
  }

  // Emit function end directives
  emitFunctionEnd(MF);

  // We didn't modify anything.
  return false;
}

// Print out an operand for an inline asm expression.
bool MipsAsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo, 
                                     unsigned AsmVariant,const char *ExtraCode){
  // Does this asm operand have a single letter operand modifier?
  if (ExtraCode && ExtraCode[0]) 
    return true; // Unknown modifier.

  printOperand(MI, OpNo);
  return false;
}

void MipsAsmPrinter::printOperand(const MachineInstr *MI, int opNum) {
  const MachineOperand &MO = MI->getOperand(opNum);
  bool closeP = false;

  if (MO.getTargetFlags())
    closeP = true;

  switch(MO.getTargetFlags()) {
  case MipsII::MO_GPREL:    O << "%gp_rel("; break;
  case MipsII::MO_GOT_CALL: O << "%call16("; break;
  case MipsII::MO_GOT:
    if (MI->getOpcode() == Mips::LW)
      O << "%got(";
    else
      O << "%lo(";
    break;
  case MipsII::MO_ABS_HILO:
    if (MI->getOpcode() == Mips::LUi)
      O << "%hi(";
    else
      O << "%lo(";     
    break;
  }

  switch (MO.getType()) {
    case MachineOperand::MO_Register:
      O << '$' << LowercaseString(getRegisterName(MO.getReg()));
      break;

    case MachineOperand::MO_Immediate:
      O << (short int)MO.getImm();
      break;

    case MachineOperand::MO_MachineBasicBlock:
      GetMBBSymbol(MO.getMBB()->getNumber())->print(O, MAI);
      return;

    case MachineOperand::MO_GlobalAddress:
      O << Mang->getMangledName(MO.getGlobal());
      break;

    case MachineOperand::MO_ExternalSymbol:
      O << MO.getSymbolName();
      break;

    case MachineOperand::MO_JumpTableIndex:
      O << MAI->getPrivateGlobalPrefix() << "JTI" << getFunctionNumber()
      << '_' << MO.getIndex();
      break;

    case MachineOperand::MO_ConstantPoolIndex:
      O << MAI->getPrivateGlobalPrefix() << "CPI"
        << getFunctionNumber() << "_" << MO.getIndex();
      if (MO.getOffset())
        O << "+" << MO.getOffset();
      break;
  
    default:
      llvm_unreachable("<unknown operand type>");
  }

  if (closeP) O << ")";
}

void MipsAsmPrinter::printUnsignedImm(const MachineInstr *MI, int opNum) {
  const MachineOperand &MO = MI->getOperand(opNum);
  if (MO.getType() == MachineOperand::MO_Immediate)
    O << (unsigned short int)MO.getImm();
  else 
    printOperand(MI, opNum);
}

void MipsAsmPrinter::
printMemOperand(const MachineInstr *MI, int opNum, const char *Modifier) {
  // when using stack locations for not load/store instructions
  // print the same way as all normal 3 operand instructions.
  if (Modifier && !strcmp(Modifier, "stackloc")) {
    printOperand(MI, opNum+1);
    O << ", ";
    printOperand(MI, opNum);
    return;
  }

  // Load/Store memory operands -- imm($reg) 
  // If PIC target the target is loaded as the 
  // pattern lw $25,%call16($28)
  printOperand(MI, opNum);
  O << "(";
  printOperand(MI, opNum+1);
  O << ")";
}

void MipsAsmPrinter::
printFCCOperand(const MachineInstr *MI, int opNum, const char *Modifier) {
  const MachineOperand& MO = MI->getOperand(opNum);
  O << Mips::MipsFCCToString((Mips::CondCode)MO.getImm()); 
}

void MipsAsmPrinter::EmitStartOfAsmFile(Module &M) {
  // FIXME: Use SwitchSection.
  
  // Tell the assembler which ABI we are using
  O << "\t.section .mdebug." << emitCurrentABIString() << '\n';

  // TODO: handle O64 ABI
  if (Subtarget->isABI_EABI())
    O << "\t.section .gcc_compiled_long" << 
      (Subtarget->isGP32bit() ? "32" : "64") << '\n';

  // return to previous section
  O << "\t.previous" << '\n'; 
}

void MipsAsmPrinter::PrintGlobalVariable(const GlobalVariable *GVar) {
  const TargetData *TD = TM.getTargetData();

  if (!GVar->hasInitializer())
    return;   // External global require no code

  // Check to see if this is a special global used by LLVM, if so, emit it.
  if (EmitSpecialLLVMGlobal(GVar))
    return;

  O << "\n\n";
  std::string name = Mang->getMangledName(GVar);
  Constant *C = GVar->getInitializer();
  const Type *CTy = C->getType();
  unsigned Size = TD->getTypeAllocSize(CTy);
  const ConstantArray *CVA = dyn_cast<ConstantArray>(C);
  bool printSizeAndType = true;

  // A data structure or array is aligned in memory to the largest
  // alignment boundary required by any data type inside it (this matches
  // the Preferred Type Alignment). For integral types, the alignment is
  // the type size.
  unsigned Align;
  if (CTy->getTypeID() == Type::IntegerTyID ||
      CTy->getTypeID() == Type::VoidTyID) {
    assert(!(Size & (Size-1)) && "Alignment is not a power of two!");
    Align = Log2_32(Size);
  } else
    Align = TD->getPreferredTypeAlignmentShift(CTy);

  printVisibility(name, GVar->getVisibility());

  OutStreamer.SwitchSection(getObjFileLowering().SectionForGlobal(GVar, Mang,
                                                                  TM));

  if (C->isNullValue() && !GVar->hasSection()) {
    if (!GVar->isThreadLocal() &&
        (GVar->hasLocalLinkage() || GVar->isWeakForLinker())) {
      if (Size == 0) Size = 1;   // .comm Foo, 0 is undefined, avoid it.

      if (GVar->hasLocalLinkage())
        O << "\t.local\t" << name << '\n';

      O << MAI->getCOMMDirective() << name << ',' << Size;
      if (MAI->getCOMMDirectiveTakesAlignment())
        O << ',' << (1 << Align);

      O << '\n';
      return;
    }
  }
  switch (GVar->getLinkage()) {
   case GlobalValue::LinkOnceAnyLinkage:
   case GlobalValue::LinkOnceODRLinkage:
   case GlobalValue::CommonLinkage:
   case GlobalValue::WeakAnyLinkage:
   case GlobalValue::WeakODRLinkage:
    // FIXME: Verify correct for weak.
    // Nonnull linkonce -> weak
    O << "\t.weak " << name << '\n';
    break;
   case GlobalValue::AppendingLinkage:
    // FIXME: appending linkage variables should go into a section of their name
    // or something.  For now, just emit them as external.
   case GlobalValue::ExternalLinkage:
    // If external or appending, declare as a global symbol
    O << MAI->getGlobalDirective() << name << '\n';
    // Fall Through
   case GlobalValue::PrivateLinkage:
   case GlobalValue::LinkerPrivateLinkage:
   case GlobalValue::InternalLinkage:
    if (CVA && CVA->isCString())
      printSizeAndType = false;
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

  EmitAlignment(Align, GVar);

  if (MAI->hasDotTypeDotSizeDirective() && printSizeAndType) {
    O << "\t.type " << name << ",@object\n";
    O << "\t.size " << name << ',' << Size << '\n';
  }

  O << name << ":\n";
  EmitGlobalConstant(C);
}


// Force static initialization.
extern "C" void LLVMInitializeMipsAsmPrinter() { 
  RegisterAsmPrinter<MipsAsmPrinter> X(TheMipsTarget);
  RegisterAsmPrinter<MipsAsmPrinter> Y(TheMipselTarget);
}

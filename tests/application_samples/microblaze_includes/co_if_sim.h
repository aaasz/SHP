/*********************************************************************
* Copyright (c) 2003, Impulse Accelerated Technologies, Inc.
* All Rights Reserved.
*
* co_if_sim.h: Conditional-compilation macros used to exclude/include
*   code depending on whether we're doing desktop simulation or not.
*
* There is no need to #include this file directly -- it is referenced
* by co.h, the standard Impulse C header file.
*
*********************************************************************/

#undef IF_SIM
#undef IF_NSIM

// Wrap the IF_SIM macro around a block of code to ensure that it is only 
// compiled for desktop simulation, not for synthesis or for real
// execution on the target platform processor.
//
// Conversely, wrap the IF_NSIM macro around a block of code to ensure that 
// it is only compiled for synthesis or for execution on the target
// processor, not in desktop simulation.

#if defined(IMPULSE_C_SYNTHESIS) || defined(IMPULSE_C_TARGET)
#define IF_SIM(s)
#define IF_NSIM(s) s

#else
#define IF_SIM(s) s
#define IF_NSIM(s)

#endif

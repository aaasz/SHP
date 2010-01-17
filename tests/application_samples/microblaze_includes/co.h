/*********************************************************************
* Copyright (c) 2003-2004, Impulse Accelerated Technologies, Inc.
* All Rights Reserved.
*
* co.h: Impulse C interface for the Xilinx MicroBlaze/FSL platform.
*
* Definitions of Impulse C functions, including:
*   - process creation and execution
*   - bit operations
*   - streams
*   - signals
*   - registers
*   - shared memory
*
*********************************************************************/

#ifndef CO_H
#define CO_H

#ifdef __uClinux__
#define XIo_In32(InputPtr)  (*(volatile uint32 *)(InputPtr))
#define XIo_Out32(OutputPtr, Value) \
    { (*(volatile uint32 *)(OutputPtr) = Value);  }
#define co_msg_print(msg) printf("%s\n",msg)
#else
#include "xio.h"
#include "mb_interface.h"
#define co_msg_print(msg) xil_printf("%s\n",msg)
#endif

#include "fsl_if.h"

#define IMPULSE_C_TARGET MICROBLAZE
#define IMPULSE_C_BUS FSL
#define REGISTER_WIDTH 32

#include "co_if_sim.h"
#include "co_types.h"

extern co_process co_process_create(char * name, co_function run, int argc, ...);
extern void co_execute(co_architecture arch);
#define cosim_architecture_config(arch, attribute, value)

extern co_type co_type_create(co_sort sort, unsigned int width);
#define INT_TYPE(width)		co_type_create(co_int_sort,width)
#define UINT_TYPE(width) 	co_type_create(co_uint_sort,width)
#define CHAR_TYPE			co_type_create(co_int_sort,8)
#define FLOAT_TYPE			co_type_create(co_float_sort,32)
#define DOUBLE_TYPE			co_type_create(co_float_sort,64)

#define co_bit_extract(value, start_bit, num_bits) (int32)(((value) >> (start_bit)) & (~(0xffffffffU << (num_bits))))
#define co_bit_extract_u(value, start_bit, num_bits) (uint32)((start_bit) >= 32 ? 0 : ((value) >> (start_bit)) & (~(0xffffffffU << (num_bits))))
#define co_bit_insert(destination, dest_start_bit, num_bits, source) \
	(int32)( \
	    ( (destination) & ( 0xffffffffU << ( (dest_start_bit) + (num_bits) ) ) ) | \
        ( ( (source) << (dest_start_bit) ) & ~( ( 0xffffffffU << ( (dest_start_bit) + (num_bits) ) ) | ~( 0xffffffffU << (dest_start_bit) ) ) ) | \
        ( (destination) & ~( 0xffffffffU << (dest_start_bit) ) ) \
	)
#define co_bit_insert_u(destination, dest_start_bit, num_bits, source) (uint32)co_bit_insert((int32)(destination), (dest_start_bit), (num_bits), (int32)(source))

extern co_stream co_stream_create(char * name, co_type datatype, int bufferdepth);
extern void co_stream_attach(co_stream stream, int io_addr, co_stream_direction dir);
extern int co_stream_free(co_stream stream);

#define co_stream_open(stream,mode,type)
#define co_stream_eos(stream) 0

#define HW_STREAM_OPEN(proc,stream,mode,type)

#define HW_STREAM_READ(proc,stream,var,evar) \
  asm("get %0, " p_ ## proc ## _ ## stream () "; nop; mfs %1,rmsr" : "=d" (var), "=d" (evar));\
  evar=((evar&0x10)!=0)

#define HW_STREAM_READ_NB(proc,stream,var,evar) \
  evar=0;\
  asm("nget %0, " p_ ## proc ## _ ## stream () "; nop; addic %1,%2,0" : "=d" (var), "=d" (evar) : "d" (evar))

#define HW_STREAM_WRITE(proc,stream,var) \
  asm("put %0, " p_ ## proc ## _ ## stream () : : "d" (var))

#define HW_STREAM_CLOSE(proc,stream) \
  if (stream->direction==HW_INPUT) asm("cput r0, " p_ ## proc ## _ ## stream ())

extern co_signal co_signal_create(char * name);
extern void co_signal_attach(co_signal stream, int io_addr, co_stream_direction dir);
extern co_error co_signal_post(co_signal signal, int32 value);
extern co_error co_signal_wait(co_signal signal, int32 *vp);

extern co_register co_register_create(const char * name, co_type type);
extern void co_register_attach(co_register reg, int io_addr, co_stream_direction dir);
extern int32 co_register_get(co_register reg);
extern void co_register_put(co_register reg, int32 value);
extern co_error co_register_read(co_register reg, void *buffer, unsigned int size);
extern co_error co_register_write(co_register reg, const void *buffer, unsigned int size);

extern co_memory co_memory_create(char *name, char *loc, unsigned int size, void *(*alloc)());
extern co_error co_memory_writeblock(co_memory mem, unsigned int offset, void *buf, unsigned int buffersize);
extern co_error co_memory_readblock(co_memory mem, unsigned int offset, void *buf, unsigned int buffersize);
extern void *co_memory_ptr(co_memory mem);

#ifndef IMPULSE_C_SYNTHESIS
#define co_par_break()
#endif

#include "co_math.h"

#endif  /* CO_H */


/*********************************************************************
* Copyright (c) 2003, Impulse Accelerated Technologies, Inc.
* All Rights Reserved.
*
* co_types.h: Type definitions for the Impulse C API.
*
*********************************************************************/

#ifndef CO_TYPES_H
#define CO_TYPES_H

#ifndef NULL
#define NULL 0
#endif

typedef enum {co_err_none = 0, co_err_eos, co_err_invalid_arg, co_err_already_open, co_err_not_open, co_err_unavail, co_err_unknown} co_error;

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;

typedef void * co_architecture;

typedef void (*co_function)();

typedef struct co_process_s {
	char * name;
	co_function run;
	int argc;
	void ** args;
	struct co_process_s *next;
} co_process_t;
typedef co_process_t* co_process;

typedef enum {co_int_sort = 1, co_uint_sort, co_float_sort} co_sort;

typedef struct co_type_s {
	co_sort sort;
	unsigned int width;	
} co_type_t;
typedef co_type_t* co_type;

typedef enum { HW_INPUT, HW_OUTPUT, CO_NO_DIRECTION } co_stream_direction; 

typedef struct co_stream_s {
	char * name;
	co_type datatype;
	int buffer_depth;
	co_stream_direction direction;
	int io_addr;
} co_stream_t;
typedef co_stream_t* co_stream;

typedef struct co_signal_s {
	char * name;
	co_stream_direction direction;
	int io_addr;
} co_signal_t;
typedef co_signal_t* co_signal;

typedef struct co_register_s {
	char * name;
	co_stream_direction direction;
	int io_addr;
} co_register_t;
typedef co_register_t* co_register;

typedef struct co_memory_s {
	char * name;
        void * data;
        unsigned int size;
} co_memory_t;
typedef co_memory_t* co_memory;

typedef void* co_parameter;

#endif  // CO_TYPES_H

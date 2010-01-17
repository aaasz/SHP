CFLAGS = -Wall
CC = gcc
INCLUDES =
LIBS = 
BIN_PATH = ../../bin/collector


EXECUTABLES = ${BIN_PATH}/generate_proj ${BIN_PATH}/generate_xmp \
			  ${BIN_PATH}/generate_dw ${BIN_PATH}/generate_mhs   \
			  ${BIN_PATH}/generate_mss ${BIN_PATH}/generate_ucf

all: ${EXECUTABLES}

clean:
	rm -f ${BIN_PATH}/* 

bin/%:src/%.c
	${CC} ${CFLAGS} -I${INCLUDES} -c $< ${LIBS} -o $@
	

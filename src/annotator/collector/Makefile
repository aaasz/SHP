CFLAGS = -Wall
CC = gcc
INCLUDES =
LIBS = 
EXECUTABLES = bin/generate_proj bin/generate_xmp \
			  bin/generate_dw bin/generate_mhs   \
			  bin/generate_mss bin/generate_ucf

all: ${EXECUTABLES}

clean:
	rm -f bin/* 

bin/%:src/%.c
	${CC} ${CFLAGS} -I${INCLUDES} -c $< ${LIBS} -o $@
	

AVCLIB=./avce00/avc.a
AVCFILES=avce00/avc_*.c avce00/cpl*.c avce00/ex_*.c


LIB_OBJS= avc_e00read.o avc_e00write.o avc_rawbin.o avc_bin.o avc_binwr.o \
	  avc_e00gen.o avc_e00parse.o avc_misc.o \
	  cpl_error.o cpl_conv.o cpl_vsisimple.o cpl_string.o cpl_dir.o

CFLAGS=-I/usr/lib/R/include -I./avce00

all: RArcInfo.c $(LIB_OBJS)
	gcc $(CFLAGS) -c -Wall -O3 -fPIC RArcInfo.c 
	gcc -shared -Wl,-soname,RArcInfo.so -o RArcInfo.so RArcInfo.o $(LIB_OBJS)

$(LIB_OBJS):
	gcc $(CFLAGS) -c -Wall -O3 -fPIC $(AVCFILES) 

avclib: $(LIB_OBJS)
	gcc -shared -Wl,-soname,libavc.1.2.so -o libavc.1.2.so 	$(LIB_OBJS)

clean:
	rm *.o *.so

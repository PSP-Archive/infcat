TARGET = InfCat
OBJS =\
	../../core/icDef.o \
	../../core/icTextReader.o \
	../../core/icSectionValue.o \
	../../psp/moduleinfo.o \
	main.o

#USE_PSPSDK_LIBC = 1

PSP_FW_VERSION = 371

INCDIR = ../../core
CFLAGS = -O6 -G0 -mno-check-zero-division -Wall

CXXFLAGS = $(CFLAGS) -fno-exceptions
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS = -lcat -lpng -lz -lpspgum -lpspgu -lpsppower -lpsprtc -lstdc++ -lm

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = char def read - InfinityCat Test

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak


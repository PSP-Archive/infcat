TARGET = Cat_Input
OBJS =\
	moduleinfo.o \
	main.o

#USE_PSPSDK_LIBC = 1

PSP_FW_VERSION = 371

INCDIR = .
CFLAGS = -O6 -G0 -Wall

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS = -lcat -lpspgum -lpspgu -lpsppower -lpsprtc -lm

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Cat_Input - libCat test

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak


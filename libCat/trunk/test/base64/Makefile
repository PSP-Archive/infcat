TARGET = base64
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
LIBS = -lcat -lpsppower -lpspgum -lpspgu -lpsppower -lpsprtc

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = base64 - libCat test

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak


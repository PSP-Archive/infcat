# libCat
#
# -build
# make
#
# -install
# make install
#
# -uninstall
# make uninstall
#

# define
#  USE_CAT_IMAGELOADER_PNG
#  USE_CAT_IMAGELOADER_PCX

TARGET_LIB = libCat.a
PSP_FW_VERSION = 371

OBJS=\
	source/Cat_GetButtonSwapInfo.o \
	source/Cat_PspCallback.o \
	source/Cat_Network.o \
	source/Cat_Resolver.o \
	source/Cat_CharacterEntityReferenceConvert.o \
	source/Cat_URLConvert.o \
	source/Cat_UTFConvert.o \
	source/Cat_SJISConvert.o \
	source/Cat_GB2312Convert.o \
	source/Cat_Big5Convert.o \
	source/Cat_EUCJPConvert.o \
	source/Cat_Base64.o \
	source/Cat_MD5.o \
	source/Cat_Palette.o \
	source/Cat_Texture.o \
	source/Cat_ImageLoader.o \
	source/Cat_ImageLoaderPNG.o \
	source/Cat_ImageLoaderPCX.o \
	source/Cat_Render.o \
	source/Cat_Stream.o \
	source/Cat_StreamFile.o \
	source/Cat_StreamMemory.o \
	source/Cat_StreamPad.o \
	source/Cat_Input.o \
	source/Cat_CritSec.o

INSTALL_FILE=\
	include/Cat_GetButtonSwapInfo.h \
	include/Cat_PspCallback.h \
	include/Cat_Network.h \
	include/Cat_Resolver.h \
	include/Cat_CharacterEntityReferenceConvert.h \
	include/Cat_URLConvert.h \
	include/Cat_UTFConvert.h \
	include/Cat_SJISConvert.h \
	include/Cat_GB2312Convert.h \
	include/Cat_Big5Convert.h \
	include/Cat_EUCJPConvert.h \
	include/Cat_Base64.h \
	include/Cat_MD5.h \
	include/Cat_Palette.h \
	include/Cat_Texture.h \
	include/Cat_ImageLoader.h \
	include/Cat_Render.h \
	include/Cat_Stream.h \
	include/Cat_StreamFile.h \
	include/Cat_StreamMemory.h \
	include/Cat_StreamPad.h \
	include/Cat_Input.h \
	include/Cat_CritSec.h

CFLAGS = -O6 -G0 -mno-check-zero-division -Wall -DUSE_CAT_IMAGELOADER_PNG -DUSE_CAT_IMAGELOADER_PCX
INCDIR = ./include

PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR=$(shell psp-config --psp-prefix)

include $(PSPSDK)/lib/build.mak

install: $(TARGET_LIB)
	@echo "Installing $(TARGET_LIB) into $(PSPDIR)"
	@mkdir -p $(PSPDIR)/include $(PSPDIR)/lib
	@cp $(INSTALL_FILE) $(PSPDIR)/include
	@cp $(TARGET_LIB)   $(PSPDIR)/lib
	@echo "Done"

uninstall:
	@echo "Uninstalling $(TARGET_LIB) from $(PSPDIR)"
	@rm -f $(PSPDIR)/include/Cat_GetButtonSwapInfo.h
	@rm -f $(PSPDIR)/include/Cat_PspCallback.h
	@rm -f $(PSPDIR)/include/Cat_Network.h
	@rm -f $(PSPDIR)/include/Cat_Resolver.h
	@rm -f $(PSPDIR)/include/Cat_CharacterEntityReferenceConvert.h
	@rm -f $(PSPDIR)/include/Cat_URLConvert.h
	@rm -f $(PSPDIR)/include/Cat_UTFConvert.h
	@rm -f $(PSPDIR)/include/Cat_SJISConvert.h
	@rm -f $(PSPDIR)/include/Cat_GB2312Convert.h
	@rm -f $(PSPDIR)/include/Cat_Big5Convert.h
	@rm -f $(PSPDIR)/include/Cat_EUCJPConvert.h
	@rm -f $(PSPDIR)/include/Cat_Base64.h
	@rm -f $(PSPDIR)/include/Cat_MD5.h
	@rm -f $(PSPDIR)/include/Cat_Palette.h
	@rm -f $(PSPDIR)/include/Cat_Texture.h
	@rm -f $(PSPDIR)/include/Cat_ImageLoader.h
	@rm -f $(PSPDIR)/include/Cat_Render.h
	@rm -f $(PSPDIR)/include/Cat_Stream.h
	@rm -f $(PSPDIR)/include/Cat_StreamFile.h
	@rm -f $(PSPDIR)/include/Cat_StreamMemory.h
	@rm -f $(PSPDIR)/include/Cat_StreamPad.h
	@rm -f $(PSPDIR)/include/Cat_Input.h
	@rm -f $(PSPDIR)/include/Cat_CritSec.h
	@rm -f $(PSPDIR)/lib/$(TARGET_LIB)
	@echo "Done"

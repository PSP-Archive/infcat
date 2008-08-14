#
# test.sffを表示してみるテスト
#
# 実行ファイルと同じフォルダに
# 表示したいsffファイルをtest.sffとリネームし入れてください。
# sffファイルは、別途ご用意ください。
#
# イメージ切り替え 左右の方向キー
#

TARGET = InfCat
OBJS =\
	../../core/icTexture.o \
	../../core/icTexturePool.o \
	../../core/icSffLoader.o \
	../../core/icAct.o \
	../../psp/moduleinfo.o \
	icGame.o \
	icGameSffViewer.o \
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
PSP_EBOOT_TITLE = SffViewer - InfinityCat Test

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak


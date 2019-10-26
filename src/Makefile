#Makefile for canny

#编译器为g++
CC = g++
#目标名
TARGET = Vision

INCLUDE = -I../include

#vpath %.h ../include
CFLAGS = -g -o  

#动态库的引用
LIBVAR = -lMvCameraControl -lX11 -lMVGigEVisionSDK

LIBPATH = -L../lib
LPATHRUN = -Wl,-rpath=../lib
#定义变量pkg表示opencv的包
PKG = `pkg-config opencv --libs --cflags`
#CPPFLAGS = -E -D

RM = rm -rf

SRCFILE = $(wildcard *.cpp)
DEFFILE = $(patsubst %.cpp, %.o, $(SRCFILE))


$(TARGET):$(SRCFILE)
	$(CC) $(CFLAGS) $(TARGET) $(SRCFILE) $(INCLUDE) $(LPATHRUN) $(LIBPATH) $(LIBVAR) $(PKG) 
#%.o : %.c
##	$(CC) $(CFLAGS) $< $@ 
.PHONY:clean

clean:
	-$(RM) $(TARGET) $(DEFFILE)
	-$(RM) MvSdkLog

.PHONY:add-library
add-library:
	sudo cp ../lib/* /usr/local/lib

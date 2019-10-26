#Makefile for canny
#编译器为g++
CC = g++
#目标名
TARGET = Vision

INCLUDE = -I ../include

CFLAGS = -o -g -Wall -fPIC $(INCLUDE)

#动态库的引用
LIBVAR = $(wildcard ../lib/*.so)

LIBPATH = -L ../lib
#定义变量pkg表示opencv的包
PKG = `pkg-config opencv --libs --cflags`
#CPPFLAGS = -E -D

RM = rm -rf

SRCFILE = $(wildcard *.cpp)
DEFFILE = $(patsubst %.cpp, %.o, $(SRCFILE))


$(TARGET):$(DEFFILE)
	$(CC) -o $(TARGET) $^ $(PKG) $(CFLAGS) $(LIBPATH)
%.o : %.c
	$(CC) $(CFLAGS) $< $@ 
clean:
	-$(RM) $(TARGET) $(DEFFILE)

#Makefile for canny
#--------------------------变量定义-----------------------------------------------
#编译器为g++
CC = g++
#目标名
TARGET = Vision

#头文件包含路径
INCLUDE = -I../include

#vpath %.h ../include
CFLAGS = -g -o  
#CPPFLAGS = -E -D

#动态库的引用
LIBVAR = -lMvCameraControl 
LIBPATH = -L../lib
LPATHRUN = -Wl,-rpath=../lib
#定义变量pkg表示opencv的包
PKG = `pkg-config opencv --libs --cflags`


#删除指令变量参数
RM = rm -rf

SRCFILE = $(wildcard *.cpp)
DEFFILE = $(patsubst %.cpp, %.o, $(SRCFILE))
$(warning "All the cpp file is $(SRCFILE)")
#--------------------------命令操作----------------------------------------------
#$^ 代表所有依赖文件
#$@ 代表目标文件
#$< 代表依赖第一个文件
$(TARGET):$(DEFFILE)
	$(CC) $(CFLAGS) $(TARGET) $(DEFFILE) $(INCLUDE) $(LPATHRUN) $(LIBPATH) $(LIBVAR) $(PKG) 
%.o:%.cpp
	$(CC) -c $< -o $@ $(INCLUDE) $(LPATHRUN) $(PKG)
#---------------------------伪指令---------------------------------------------
#clean操作，删除生成的文件和文件夹
#如运行过需执行sudo操作
.PHONY:clean
clean:
	$(RM) $(TARGET) $(DEFFILE)
	$(RM) MvSdkLog
#伪指令add-library
#将动态库安装到系统路径中。
.PHONY:add-library
add-library:
	sudo cp ../lib/* /usr/local/lib

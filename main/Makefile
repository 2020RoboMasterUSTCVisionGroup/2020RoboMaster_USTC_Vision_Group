#Makefile for canny
#--------------------------变量定义-----------------------------------------------
#编译器为g++
CC = g++
#目标名
TARGET = vision

#头文件包含路径
AROMOR_FINDER_INCLUDE = -I../codes/ArmorFinder/include
CAMERA_INCLUDE = -I../codes/Camera/include

#vpath %.h ../include
CFLAGS = -g -o  
#CPPFLAGS = -E -D

#动态库的引用
CAMERA_LIBVAR = -lMvCameraControl 
CAMERA_LIBPATH = -L../codes/Camera/lib
LPATHRUN = -Wl,-rpath=../codes/Camera/lib
#定义变量pkg表示opencv的包
PKG = `pkg-config opencv --libs --cflags`


#删除指令变量参数
RM = rm -rf

ARMOR_FINDER_SRCFILE = $(wildcard ../codes/ArmorFinder/src/*.cpp)
ARMOR_FINDER_DEFFILE = $(patsubst %.cpp, %.o, $(ARMOR_FINDER_SRCFILE))
#摄像头源文件和目标文件
CAMERA_SRCFILE = $(wildcard ../codes/Camera/src/*.cpp)
CAMERA_DEFFILE = $(patsubst %.cpp, %.o, $(CAMERA_SRCFILE))
#--------------------------命令操作----------------------------------------------
#$^ 代表所有依赖文件
#$@ 代表目标文件
#$< 代表依赖第一个文件
$(TARGET):$(ARMOR_FINDER_DEFFILE) $(CAMERA_DEFFILE)
#	$(CC) $(DEFFILE) $(CFLAGS) $(TARGET) $(INCLUDE) $(LPATHRUN) $(LIBPATH) $(LIBVAR) $(PKG) 
	$(CC) $(CAMERA_DEFFILE) $(ARMOR_FINDER_DEFFILE) main.cpp $(CFLAGS) $(TARGET) $(CAMERA_LIBVAR) $(PKG) $(CAMERA_INCLUDE)
#%.o:%.cpp
#	$(CC) -c $< -o $@ $(AROMOR_FINDER_INCLUDE) $(LPATHRUN) $(PKG)
$(ARMOR_FINDER_DEFFILE):$(ARMOR_FINDER_SRCFILE)
	$(CC) -c $^ -o $@ $(AROMOR_FINDER_INCLUDE) $(PKG)
$(CAMERA_DEFFILE):$(CAMERA_SRCFILE)
	$(CC) -c $^ -o $@ $(CAMERA_INCLUDE) $(PKG) $(CAMERA_INCLUDE) $(CAMERA_LIBVAR) $(LPATHRUN)

#---------------------------伪指令---------------------------------------------
#clean操作，删除生成的文件和文件夹
#如运行过需执行sudo操作
.PHONY:clean
clean:
	$(RM) $(TARGET) $(ARMOR_FINDER_DEFFILE) $(CAMERA_DEFFILE)
	#$(RM) -rf MvSdkLog
#伪指令add-library
#将动态库安装到系统路径中。
.PHONY:add-library
add-libraries:
	sudo cp ../codes/Camera/lib/* /usr/local/lib

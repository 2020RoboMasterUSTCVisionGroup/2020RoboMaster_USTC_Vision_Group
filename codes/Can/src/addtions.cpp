#include <cstring>
#include <iostream>
#include <unistd.h>
#include<cans.h>
#include "addtions.h"
using namespace std;
extern McuData mcu_data;
void canReceive(Can *pCan) {
    char buffer[8];
    int k=0; //计时变量
    while (true) {
        if(k==50)
        {
            int j;
            k=0;
            unsigned char resend[]="handshake"; //k=50,说明1秒内没有从单片机接收到正确的数据
            for(j=0;j<3;j++)                   //则重新发送初始信号，再次建立通信
            pCan->canTansfer(resend);
        }
        memset(buffer, 0, sizeof(buffer));  //初始化buffer数组

        int nbytes = pCan->ReadData((uint8_t *) buffer);
        if (nbytes==8)  //接收到8个字节的数据，通信正确
        {
            k=0;
            memcpy(&mcu_data, buffer, sizeof(mcu_data)); //接收到的数据复制到结构体中
        }
        else
        {
            k++;
        }
        usleep(20000);//延时等待20毫秒
    }
}

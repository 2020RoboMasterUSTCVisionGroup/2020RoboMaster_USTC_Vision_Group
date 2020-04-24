#ifndef CAN
#define CAN

#include "cans.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int Can::canTansfer(unsigned char data[])
{
	int s1;
	struct sockaddr_can addr1;
	struct ifreq ifr1;
	struct can_frame frame1;
	if ((s1 = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)//创建套接字
	{
	    perror("Create socket failed");
	    exit(-1);
	}
	
	strcpy(ifr1.ifr_name, "can0"); //设置can接口
	printf("can port is %s\n",ifr1.ifr_name);
	ioctl(s1, SIOCGIFINDEX, &ifr1);
    addr1.can_family = AF_CAN;
	addr1.can_ifindex = ifr1.ifr_ifindex;
    
	if(bind(s1, (struct sockaddr *)&addr1, sizeof(addr1)) < 0) //can接口和套接字关联
	{
		perror("Bind can device failed\n");
		close(s1);
		exit(-2);
	}
   	frame1.can_id = 0x1F;  //发送数据设置
    frame1.can_dlc = 8;
    printf("%s ID=%#x data length=%d\n", ifr1.ifr_name, frame1.can_id, frame1.can_dlc);
    /* prepare data for sending: 0x11,0x22...0x88 */
    	for (int i=0; i<8; i++)
         {
        	frame1.data[i] = data[i];
        	printf("%#x ", frame1.data[i]);
         }

        printf("Sent out\n");
        /* Sending data */
        if(write(s1, &frame1, sizeof(frame1)) < 0) //发送
		{
            perror("Send failed");
            close(s1);
		}
	 close(s1);
}

int Can::ReadData(unsigned char *buffer) {
	int s0, nbytes;
	struct sockaddr_can addr0;
	struct ifreq ifr0;
	struct can_frame frame0;
	struct can_filter rfilter[1];
	if ((s0 = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)//创建套接字
	{
	    perror("Create socket failed");
	    exit(-1);
	}
	
	strcpy(ifr0.ifr_name, "can0"); //设置can接口
	printf("can port is %s\n",ifr0.ifr_name);
	ioctl(s0, SIOCGIFINDEX, &ifr0);//绑定can0设备
    addr0.can_family = AF_CAN;
	addr0.can_ifindex = ifr0.ifr_ifindex;
    
	if(bind(s0, (struct sockaddr *)&addr0, sizeof(addr0)) < 0) //can接口和套接字关联
	{
		perror("Bind can device failed\n");
		close(s0);
		exit(-2);
	}
    rfilter[0].can_id = 0x1F; //设置滤波器，仅接收can id 0x1F的数据
	rfilter[0].can_mask = CAN_SFF_MASK;
	if(setsockopt(s0, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter)) < 0)
	    {
	    	perror("set receiving filter error\n");
	    	close(s0);
	    	exit(-3);
	    }
    nbytes = read(s0, &frame0, sizeof(frame0)); //接收
	if((frame0.can_id==0x1F)&&(nbytes>0))
		{
	    	printf("%s ID=%#x data length=%d\n", ifr0.ifr_name, frame0.can_id, frame0.can_dlc);
	        	for (int i=0; i < frame0.can_dlc; i++)
	        		printf("%#x ", frame0.data[i]);
	        	printf("\n");
				memcpy(buffer,frame0.data,nbytes);
	        }
	else{
		nbytes=0;
		}
		return nbytes;
	    
}
#endif
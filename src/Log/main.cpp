#include "log.h"
#include<iostream>
int main(){
     TIMER_START(x);
    for(int i=0;i<1000;i++){
        int j;
        j++;
    }
    std::cout<<"finish!"<<std::endl;
    TIMER_STOP(x);
    TIME_COST_MS("test");
    return 0;
}
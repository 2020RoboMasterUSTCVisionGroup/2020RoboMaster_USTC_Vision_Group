#include "../include/log.h"
int main(){
    INFO_START("s1 start",s1);
    for(int i=0;i<1000;i++){
        int j;
        j++;
    }
    std::cout<<"1finish!"<<std::endl;
    INFO_END("s1 end",s1);
    INFO_START("s2",s2);
        for(int i=0;i<100000;i++){
        int j;
        j++;
    }
    std::cout<<"2finish!"<<std::endl;
   INFO_END("s2 end",s2);
    return 0;
}
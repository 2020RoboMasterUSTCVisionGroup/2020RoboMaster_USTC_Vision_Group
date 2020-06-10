#include "../include/shootTable.h"
int main(){
    ShootTable t;
    Result r=t.getResult(18,7);
    cout<<"ang:"<<r.angle<<endl;
    cout<<"end_velocity:"<<r.end_velocity<<endl;
    cout<<"fly_time:"<<r.fly_time<<endl;
    cout<<"relative_height:"<<r.relative_height<<endl;
}
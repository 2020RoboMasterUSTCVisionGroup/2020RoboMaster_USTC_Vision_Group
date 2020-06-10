#include <unordered_map>
#include<iostream>
using namespace std;
struct Key
{
    int  velocity;
    int range;
    bool operator == (const Key &t) const {
        return  velocity==t.velocity && range==t.range;
    }
};
struct KeyHash {
    std::size_t operator () (const Key &t) const {
        return  t.velocity * 100 + t.range;
    }
};
struct Result{
    float angle;
    float relative_height;
    float end_velocity;
    float fly_time;
};
class ShootTable{
    private:
        unordered_map<Key,Result,KeyHash> table;
        Key key;
        Result res;

    public:
        ShootTable(){
            //v=6 m/s**************************************************************
            key.velocity=6;
            key.range=3;
            res.angle=22.4;
            res.relative_height=0.8832;
            res.end_velocity=6.3114;
            res.fly_time=0.5411;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));
            //v=8 m/s**************************************************************
            key.velocity=8;
            key.range=3;
            res.angle=9.7;
            res.relative_height=2.4810;
            res.end_velocity=8.2306;
            res.fly_time=0.3806;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));

            key.velocity=8;
            key.range=6;
            res.angle=30.3;
            res.relative_height=-2.8573;
            res.end_velocity=8.2289;
            res.fly_time=0.8689;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));
            //v=10 m/s**************************************************************
            key.velocity=10;
            key.range=3;
            res.angle=4.7;
            res.relative_height=1.8361;
            res.end_velocity=10.1826;
            res.fly_time=0.3012;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));

            key.velocity=10;
            key.range=6;
            res.angle=15.9;
            res.relative_height=-3.4405;
            res.end_velocity=10.1760;
            res.fly_time=0.6246;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));


            key.velocity=10;
            key.range=9;
            res.angle=29.2;
            res.relative_height=-1.5941;
            res.end_velocity=10.1632;
            res.fly_time=1.0331;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));
            //v=15 m/s**************************************************************
            key.velocity=15;
            key.range=3;
            res.angle=-0.1;
            res.relative_height=-1.5726;
            res.end_velocity=15.1167;
            res.fly_time=0.2001;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));

            key.velocity=15;
            key.range=6;
            res.angle=5.7;
            res.relative_height=4.9545;
            res.end_velocity=15.0956;
            res.fly_time=0.4024;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));

            key.velocity=15;
            key.range=9;
            res.angle=10.3;
            res.relative_height=6.5596;
            res.end_velocity=15.0755;
            res.fly_time=0.6108;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));
            //v=16 m/s**************************************************************
            key.velocity=16;
            key.range=3;
            res.angle=-0.5;
            res.relative_height=1.2453;
            res.end_velocity=16.1060;
            res.fly_time=0.1876;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));

            key.velocity=16;
            key.range=6;
            res.angle=4.7;
            res.relative_height=-2.1634;
            res.end_velocity=16.0902;
            res.fly_time=0.3766;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));

            key.velocity=16;
            key.range=9;
            res.angle=8.8;
            res.relative_height=-0.1292;
            res.end_velocity=16.0689;
            res.fly_time=0.5701;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));
            //v=18 m/s**************************************************************
            key.velocity=18;
            key.range=3;
            res.angle=-1.2;
            res.relative_height=0.7238;
            res.end_velocity=18.0911;
            res.fly_time=0.1668;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));

            key.velocity=18;
            key.range=6;
            res.angle=3.3;
            res.relative_height=-1.6961;
            res.end_velocity=18.0727;
            res.fly_time=0.3342;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));

            key.velocity=18;
            key.range=9;
            res.angle=6.6;
            res.relative_height=-4.5513;
            res.end_velocity=18.0522;
            res.fly_time=0.5041;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));
            //v=28 m/s**************************************************************
            key.velocity=28;
            key.range=3;
            res.angle=-2.7;
            res.relative_height=2.0805;
            res.end_velocity=28.0431;
            res.fly_time=0.1073;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));

            key.velocity=28;
            key.range=6;
            res.angle=0.2;
            res.relative_height=-4.6652;
            res.end_velocity=28.0168;
            res.fly_time=0.2145;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));

            key.velocity=28;
            key.range=9;
            res.angle=2.0;
            res.relative_height=5.6018;
            res.end_velocity=27.9819;
            res.fly_time=0.3221;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));
            //v=30 m/s**************************************************************
            key.velocity=30;
            key.range=3;
            res.angle=-2.9;
            res.relative_height=-1.2464;
            res.end_velocity=30.0378;
            res.fly_time=0.1002;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));

            key.velocity=30;
            key.range=6;
            res.angle=0;
            res.relative_height=3.4674;
            res.end_velocity=30.0058;
            res.fly_time=0.2002;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));

            key.velocity=30;
            key.range=9;
            res.angle=1.5;
            res.relative_height=-7.1164;
            res.end_velocity=29.9762;
            res.fly_time=0.3005;
            table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));
        }
    
        Result getResult(int v,int r){
            key.velocity=v;
            key.range=r;

            if(table.count(key)==0){
                cout<<"can not find this key"<<endl;
                res.angle=-1;
                res.relative_height=-1;
                res.end_velocity=-1;
                res.fly_time=-1;
                return res;
            }

            return table[key];
            
        }

};
#include <unordered_map>
#include<iostream>
#include <fstream>
#include <sstream>
#include <string>
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
            res.angle=-1;
            res.relative_height=-1;
            res.end_velocity=-1;
            res.fly_time=-1;
            readTable();
        }
        void readTable(){
            ifstream inFile("../include/shoot.csv", ios::in);
            if (!inFile){
                cout << "can not find table！" << endl;
                exit(1);
            }
            int i = 0;
            string line;
            string field;
            string fields[6];
            while (getline(inFile, line)){
                i=0;
                istringstream sin(line); 
                while (getline(sin, field, ',')){
                    if(i>5){
                        cout << "out of index" << endl;
                        return;
                    }
                    fields[i]=field;
                    i++;
		        }
                key.velocity=stoi(fields[0]);
                key.range=stoi(fields[1]);
                res.angle= stof(fields[2]);
                res.relative_height=stof(fields[3]);
                res.fly_time=stof(fields[4]);
                res.end_velocity=stof(fields[5]);              
                table.insert(unordered_map<Key,Result,KeyHash>::value_type (key,res));
            }
            inFile.close();
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
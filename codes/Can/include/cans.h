#ifndef _CAN_H_
#define _CAN_H_

class Can {
public:
    Can(){};
    ~Can(){};
    int canTansfer(unsigned char data[]);
    int ReadData(unsigned char* buffer);
};

#endif /* _CAN_H_ */
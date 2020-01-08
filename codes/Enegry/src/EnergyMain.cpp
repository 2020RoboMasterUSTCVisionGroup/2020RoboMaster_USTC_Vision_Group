#include "energy.h"

using namespace cv;
using namespace std;

int energy(cv::Mat &src)     //能量机关部分的主函数
{   
    uint8_t color=1;         //设定颜色是红色
    Energy energy(color);
    energy.run(src);
    return 0;
}

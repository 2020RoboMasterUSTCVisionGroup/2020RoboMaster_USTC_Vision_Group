#include "energy.h"

using namespace cv;
using namespace std;

int main()
{   
    uint8_t color=1;
    Energy energy(color);
    Mat src=imread("energy.png");
    energy.run(src);
    return 0;
}

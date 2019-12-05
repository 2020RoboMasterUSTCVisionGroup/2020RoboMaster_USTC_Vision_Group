#include "amorFinder.h"
int main(){
    AmorFinder finder;
    ArmorBoxes boxes;
    Mat src = imread("./test.png");
    finder.setImage("./test.png");
    finder.preProcessV2(20);
    LightBlobs blobs = finder.findLightBlobV2();
    finder.matchArmorBoxes(src,blobs,boxes);
    finder.showArmorBoxes("res",src,boxes);
    waitKey(0);
    return 0;
}
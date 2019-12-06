#include "amorFinder.h"
int main(){
    AmorFinder finder;
    ArmorBoxes boxes;
    Mat src = imread("./test2.jpg");
    finder.setImage("./test2.jpg");
    finder.preProcessV2(40);
    LightBlobs blobs = finder.findLightBlobV2();
    finder.matchArmorBoxes(src,blobs,boxes);
    finder.showArmorBoxes("res",src,boxes);
    waitKey(0);
    return 0;
}
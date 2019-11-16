#include"amorFinder.h"
int main(){
    AmorFinder finder;
    finder.setImage("./test.png");
    finder.preProcess(200);
    LightBlobs light_Blobs = finder.findLightBlob();
    finder.showLightBlobs("result",light_Blobs);
}
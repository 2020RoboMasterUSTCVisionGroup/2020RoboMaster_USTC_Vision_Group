#include "AmorFinder/amorFinder.h"
#include "Log/log.h"
int main(){
    TIMER_START(x);
    AmorFinder finder;
    finder.setImage("./test.png");
    finder.preProcess(200);
    LightBlobs light_Blobs = finder.findLightBlob();
    finder.showLightBlobs("result",light_Blobs);
    TIMER_STOP(x);
    TIME_COST_MS("AmorFinder");
}
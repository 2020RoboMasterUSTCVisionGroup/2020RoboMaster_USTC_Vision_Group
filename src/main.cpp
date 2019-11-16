#include "AmorFinder/amorFinder.h"
#include "Log/log.h"
int main(){
    //time collect start
    TIMER_START(x);

    //construct AmorFinder instance,try to find lightbolbs
    AmorFinder finder;
    finder.setImage("./test.png");
    finder.preProcess(200);
    LightBlobs light_Blobs = finder.findLightBlob();
    finder.showLightBlobs("result",light_Blobs);

    //time collect finish,show the time
    TIMER_STOP(x);
    TIME_COST_MS("AmorFinder");
}
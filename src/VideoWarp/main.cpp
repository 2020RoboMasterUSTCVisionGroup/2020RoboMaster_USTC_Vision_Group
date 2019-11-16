#include "video.h"
int main(){
    Mat frame;
    VideoWarp *video;
    video = new VideoWarp();
    video->videoInit("./test.mp4");
    while(true){
        frame = video->getNext();
        imshow("test",frame);
        waitKey(30);
        if(waitKey(30)>=0) break;
    }
    return 0;
}
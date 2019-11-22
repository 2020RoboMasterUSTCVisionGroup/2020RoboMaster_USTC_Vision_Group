#include "inc/amorFinder.h"
/**
 * @author:tfren
 * @func:main()
 * @para:None
 * @desc:
 *       ArmorFinder测试及说明
 *       ArmorFinder实例化后，依次调用三个方法:
 *       (1) setImage():
 *           该方法用于读取一张(一帧)图片
 *           可指定路径读取：
 *           setImage("path")
 *           可直接接受Mat类型：
 *           Mat src；
 *           setImage(src)
 *       (2)preProcess(),preProcessV2()
 *          对读入的图像预处理
 *          preProcess()阈值建议200
 *          preProcessV2()阈值建议25
 *       (3)findLightBlob(),findLightBlobV2()
 *          无参数，返回灯条类，完成(1)(2)后直接调用
 *       可直接运行当前目录下的makefile查看测试结果
 *       目前均采用V2版本
 */
int main(){
    AmorFinder *finder;
    finder = new AmorFinder();
    finder->setImage("./test.png");
    finder->preProcessV2(25);
    LightBlobs light_blobs = finder->findLightBlobV2();
    finder->showLightBlobs("result",light_blobs);
    delete finder;
    return 0; 
}
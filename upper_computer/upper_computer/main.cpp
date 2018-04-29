#include <iostream>
#include <cstdlib>
#include <string>
#include <opencv2/opencv.hpp>
#include "imageRecognition.h"

using namespace std;
using namespace cv;

int main() {
    while (1) {
        //获取截图
        getScreenshot();
        //读取图像
        Mat srcImage = imread("D:/课件/单片机/curriculum-design/image/screenshot.png");
        Mat tmplImage = imread("D:/课件/单片机/curriculum-design/bottle.png");
        //查找瓶子
        Rect bottleScope;
        Point bottleLoc = getBottleLoc(srcImage, tmplImage, bottleScope);
        //查找平台
        Point platformLoc = getPlatformLoc(srcImage, bottleScope);
        //计算距离
        double distance = getDistance(bottleLoc, platformLoc);
        //输出显示
        draw(srcImage, bottleScope, bottleLoc, platformLoc);
        //点击
        char command[200];
        sprintf_s(command, 200, "adb shell input swipe 540 1600 540 1600 %d", static_cast<int>(distance*1.35));
        cout << command << endl;
        system(command);
        waitKey(1000);
    }
    waitKey();
    return 0;
}
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
        Point matchLoc;
        Point bottleBottomLoc = getBottleBottomLoc(srcImage, tmplImage, matchLoc);
        //查找平台
        Point platformLoc = getPlatformLoc(srcImage, matchLoc, tmplImage.rows, tmplImage.cols);
        //计算距离
        double distance = getDistance(bottleBottomLoc, platformLoc);
        //输出显示
        draw(srcImage, matchLoc, tmplImage.cols, tmplImage.rows, bottleBottomLoc, platformLoc);
        //点击
        char command[200];
        sprintf(command, "adb shell input swipe 540 1600 540 1600 %d", static_cast<int>(distance*1.35));
        cout << command << endl;
        system(command);
        _sleep(1500);
    }
    waitKey();
    return 0;
}
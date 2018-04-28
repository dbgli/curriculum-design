#include <iostream>
#include <cstdlib>
#include <string>
#include <opencv2/opencv.hpp>
#include "imageRecognition.h"

using namespace std;
using namespace cv;

int main() {
    while (1) {
        //��ȡ��ͼ
        getScreenshot();
        //��ȡͼ��
        Mat srcImage = imread("D:/�μ�/��Ƭ��/curriculum-design/image/screenshot.png");
        Mat tmplImage = imread("D:/�μ�/��Ƭ��/curriculum-design/bottle.png");
        //����ƿ��
        Point matchLoc, bottleBottomLoc;
        findBottle(srcImage, tmplImage, matchLoc, bottleBottomLoc);
        //����ƽ̨
        Point platformLoc;
        findPlatform(srcImage, matchLoc, tmplImage.rows, tmplImage.cols, platformLoc);
        //�������
        double distance;
        getDistance(bottleBottomLoc, platformLoc, distance);
        //�����ʾ
        draw(srcImage, matchLoc, tmplImage.cols, tmplImage.rows, bottleBottomLoc, platformLoc);
        //���
        char command[200];
        sprintf(command, "adb shell input swipe 540 1600 540 1600 %d", static_cast<int>(distance*1.35));
        cout << command << endl;
        system(command);
        _sleep(1500);
    }
    waitKey();
    return 0;
}
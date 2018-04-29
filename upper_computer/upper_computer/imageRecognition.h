#pragma once
#ifndef IMAGERECOGNITION_H
#define IMAGERECOGNITION_H
#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool getScreenshot() {
    system("D:/课件/单片机/curriculum-design/getScreenshot.bat");
    return 0;
}

Point getBottleLoc(Mat &srcImage, Mat &tmplImage, Rect &bottleScope) {
    //创建结果图像
    int retImageRows = srcImage.rows - tmplImage.rows + 1;
    int retImageCols = srcImage.cols - tmplImage.cols + 1;
    Mat retImage(retImageRows, retImageCols, CV_32FC1);
    //模板匹配并归一化数据
    matchTemplate(srcImage, tmplImage, retImage, TM_CCOEFF_NORMED);
    normalize(retImage, retImage, 0, 1, NORM_MINMAX, -1, Mat());
    //查找最佳匹配
    double minVal, maxVal;
    Point minLoc, maxLoc, matchLoc;
    minMaxLoc(retImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    matchLoc = maxLoc;
    bottleScope = Rect(matchLoc, tmplImage.size());
    Point bottleLoc = Point(bottleScope.x + bottleScope.width / 2, bottleScope.y + bottleScope.height - 20);
    return bottleLoc;
}

Point getPlatformLoc(Mat dstImage, Rect bottleScope) {
    //转换成灰度图
    cvtColor(dstImage, dstImage, COLOR_BGR2GRAY);
    //均值模糊
    blur(dstImage, dstImage, Size(3, 3));
    //查找边缘
    Canny(dstImage, dstImage, 3, 9, 3);
    //去除瓶子轮廓
    dstImage(bottleScope) = { Scalar(0) };
    //返回平台上顶点下50像素处
    Point platformLoc;
    bool flag = 0;
    for (int i = 400; i != bottleScope.y + bottleScope.height; ++i) {
        if (flag == 1) break;   //避免多余的判断
        uchar *pRow = dstImage.ptr<uchar>(i);
        for (int j = 0; j != dstImage.cols; ++j) {
            if (pRow[j]) {
                platformLoc = Point(j, i + 50);
                flag = 1;
                break;
            }
        }
    }
    return platformLoc;
}

double getDistance(Point bottleBottomLoc, Point platformLoc) {
    double distance = sqrt(pow(bottleBottomLoc.x - platformLoc.x, 2) + pow(bottleBottomLoc.y - platformLoc.y, 2));
    return distance;
}

bool draw(Mat &srcImage, Rect bottleScope, Point bottleLoc, Point platformLoc) {
    rectangle(srcImage, bottleScope, Scalar(0, 255, 255), 6, 8, 0);
    line(srcImage, bottleLoc, platformLoc, Scalar(255, 0, 0), 10, 8);
    circle(srcImage, bottleLoc, 10, Scalar(0, 0, 255), -1, 8);
    circle(srcImage, platformLoc, 10, Scalar(0, 0, 255), -1, 8);
    namedWindow("结果", WINDOW_NORMAL);
    resizeWindow("结果", 360, 640);
    imshow("结果", srcImage);
    static unsigned int n = 0;
    string srcImageSaveFile = "D:/课件/单片机/curriculum-design/image/screenshot_" + to_string(++n) + ".jpg";
    imwrite(srcImageSaveFile, srcImage);
    return 0;
}

#endif
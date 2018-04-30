#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool getScreenshot() {
    //设置存放路径
    string pcSaveFile = "D:/课件/单片机/curriculum-design/image/screenshot.png";
    string mobileSaveFile = "/storage/emulated/0/Pictures/Screenshots/screenshot.png";
    //调用ADB截图上传
    char command[200];
    sprintf_s(command, 200, "adb shell screencap -p %s", mobileSaveFile.c_str());
    system(command);
    sprintf_s(command, 200, "adb pull %s %s", mobileSaveFile.c_str(), pcSaveFile.c_str());
    system(command);
    sprintf_s(command, 200, "adb shell rm %s", mobileSaveFile.c_str());
    system(command);
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
    //返回瓶子的范围和底部位置
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
    //去除瓶子轮廓以防瓶子比平台高
    dstImage(bottleScope + Point(-1, -1) + Size(2, 2)) = { Scalar(0) };
    //返回平台上顶点下50像素处
    Point platformLoc;
    bool flag = 0;
    for (int i = 400; i != bottleScope.y + bottleScope.height; ++i) {
        if (flag == 1) break;   //减少多余的判断
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

bool draw(Mat &srcImage, Rect bottleScope, Point bottleLoc, Point platformLoc) {
    //绘制
    rectangle(srcImage, bottleScope, Scalar(0, 255, 255), 8, CV_AA);
    line(srcImage, bottleLoc, platformLoc, Scalar(255, 0, 0), 10, CV_AA);
    circle(srcImage, bottleLoc, 10, Scalar(0, 0, 255), -1, CV_AA);
    circle(srcImage, platformLoc, 10, Scalar(0, 0, 255), -1, CV_AA);
    //显示
    namedWindow("结果", WINDOW_NORMAL);
    resizeWindow("结果", 360, 640);
    imshow("结果", srcImage);
    waitKey(30);
    //保存
    static unsigned int n = 0;
    char srcImageSaveFile[100];
    sprintf_s(srcImageSaveFile, 100, "D:/课件/单片机/curriculum-design/image/screenshot_%d.jpg", ++n);
    imwrite(srcImageSaveFile, srcImage);
    return 0;
}
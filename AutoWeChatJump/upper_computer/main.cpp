#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool getScreenshot();
Point getBottleLoc(Mat &srcImage, Mat &tmplImage, Rect &bottleScope);
Point getPlatformLoc(Mat dstImage, Rect bottleScope);
bool draw(Mat &srcImage, Rect bottleScope, Point bottleLoc, Point platformLoc);
double getDistance(Point bottleLoc, Point platformLoc);
int distance2time(double distance);
bool longPress(int time, bool flag = 0);

int main() {
    unsigned int n = 0;
    while (1) {
        //获取截图
        getScreenshot();
        cout << "第 " << ++n << " 次" << endl;
        //读取图像
        Mat srcImage = imread("D:/课件/单片机/curriculum-design/image/screenshot.png");
        Mat tmplImage = imread("D:/课件/单片机/curriculum-design/bottle.png");
        //查找瓶子
        Rect bottleScope;
        Point bottleLoc = getBottleLoc(srcImage, tmplImage, bottleScope);
        cout << "瓶子位置：" << bottleLoc << endl;
        //查找平台
        Point platformLoc = getPlatformLoc(srcImage, bottleScope);
        cout << "平台位置：" << platformLoc << endl;
        //输出显示
        draw(srcImage, bottleScope, bottleLoc, platformLoc);
        //计算距离
        double distance = getDistance(bottleLoc, platformLoc);
        cout << "    距离：" << distance << " px" << endl;
        //计算时间
        int time = distance2time(distance);
        cout << "按压时间：" << time << " ms" << endl;
        //长按
        longPress(time);
        waitKey(1000);
    }
    waitKey();
    return 0;
}
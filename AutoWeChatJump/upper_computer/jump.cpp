#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define random(a, b) (rand() % (b - a + 1) + a)   //取得[a,b]的随机整数

double getDistance(Point bottleLoc, Point platformLoc) {
    double distance = sqrt(pow(bottleLoc.x - platformLoc.x, 2) + pow(bottleLoc.y - platformLoc.y, 2));
    return distance;
}

int distance2time(double distance) {
    int time = static_cast<int>(distance * 1.35);
    return time;
}

bool longPress(int time, bool flag) {
    if (flag) {
        //TODO
    }
    else {
        //添加随机值防作弊检测
        int x = 540 + random(-100, 100);
        int y = 1600 + random(-100, 100);
        cout << "按压位置：[" << x << ", " << y << "]" << endl;
        char command[100];
        sprintf_s(command, 100, "adb shell input swipe %d %d %d %d %d", x, y, x, y, time);
        system(command);
    }
    return 0;
}
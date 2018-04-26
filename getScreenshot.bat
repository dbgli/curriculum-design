@echo off
::获取日期
::set temp=%date:~0,10%
::set myDate=%temp:/=%
::获取时间
::set temp=%time: =0%
::set temp=%temp::=%
::set myTime=%temp:.=%
::设置路径
set pcSaveFile=D:/课件/单片机/curriculum-design/image/screenshot.png
set mobileSaveFile=/storage/emulated/0/Pictures/Screenshots/screenshot.png
::调用ADB截图上传
adb shell screencap -p %mobileSaveFile%
adb pull %mobileSaveFile% %pcSaveFile%
adb shell rm %mobileSaveFile%
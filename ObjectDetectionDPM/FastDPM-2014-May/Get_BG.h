#ifndef _GETBG_H_
#define _GETBG_H_

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <Windows.h>
using namespace std;
using namespace cv;

/***********************************************
生成背景有关函数和参数
***********************************************/
/*
这里的宽度和高度需要提前设定！！
*/





int Get_BG(string folder, char * outputbmp);
/*
需要首先配置opencv环境
示例用法：
Get_BG("H:\\ResearchCenter\\changshutuxiang\\left1\\","output.bmp");
输入参数：
folder:    包含图像的输入文件夹 文件夹后需要双斜线
outputbmp: 输出图像的名称 输出文件名格式为bmp
*/




/***********************************************
去除背景有关函数和参数
***********************************************/
int Remove_BG(const char * inputfolder, char * BGimage, const char * outputfolder, double sample);
/*
需要首先配置opencv环境
示例用法：
Remove_BG("H:\\ResearchCenter\\changshutuxiang\\left1\\","output.bmp", "H:\\ResearchCenter\\changshutuxiang\\output\\", 2.0);
inputfolder:    包含图像的输入文件夹 文件夹后需要双斜线
BGimage:        输出图像的名称 输出文件名格式为bmp
outputfolder:   输出图像的文件夹 文件夹后需要双斜线
sample:         图像上采样的比例参数
*/

Mat Remove_BG_OnePic(const char * inputfolder, IplImage* BGimage, double sample);
Mat Remove_BG_OnePic_mat(Mat imgNames, IplImage * backgroundorg, double sample);

#define BG_THR 200



#endif


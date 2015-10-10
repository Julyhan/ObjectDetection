#ifndef _RE_H_
#define _RE_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#define THRESHOLD 0.1

using namespace std;

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

struct data
{
	double x01;
	double y01;
	double x02;
	double y02;
	int flag; // 检测标签
	string imgname;
};


/***************************************************
将连续3帧间中断的结果补充起来
根据最近的结果线性插值得到
将需要添加的结果输出
输入
FileName   输入txt文件名
OutputFile 输出txt文件名
***************************************************/
int Result_Increasing( const char * FileName, const char * OutputFile );



/***************************************************
将连续5帧间噪声检测去除
根据连续的比较和判断得到
将需要删除的结果输出
输入
FileName   输入txt文件名
OutputFile 输出txt文件名
***************************************************/
int Result_Removing( const char * FileName, const char * OutputFile);



#endif
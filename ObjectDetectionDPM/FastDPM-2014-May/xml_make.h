#ifndef _XML_MAKE_
#define _XML_MAKE_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
typedef struct SaveInfo
{
	//const char *mediaFile; /*视频文件名*/
	std::string mediaFile; /*视频文件名*/
	int type;		/*保存结果类型，0-人脸，1-车，2-行人*/
	int frameSeq; 	/*图像帧序号*/
	int left;		/*目标左上x 坐标*/
	int top;		/*目标左上y 坐标*/
	int right;		/*目标右下X 坐标*/
	int bottom;		/*目标右下y 坐标*/
	int id;			/*跟踪目标*/
	float score;

}SaveInfo;

void xml_DetectionResults(string directoryName, vector<SaveInfo> detection_results, string xml_name, int evaluateTyte);
bool cmp(SaveInfo a, SaveInfo b);
int xml_make(string directoryName, const char* fileName, string xml_name);



#endif


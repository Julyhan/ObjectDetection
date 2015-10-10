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


void xml_DetectionResults(string directoryName,vector<SaveInfo> detection_results,string xml_name,int evaluateTyte)
{
	ofstream outputfile(xml_name.c_str());
	outputfile << "<?xml version=\"1.0\" encoding=\"gb2312\" ?>" << endl;
	outputfile << "<Message Version=\"1.0\">" << endl;
	outputfile << "    " << "<Info evaluateType=\"" << evaluateTyte << "\" mediaFile=\"" << directoryName <<"\" />" << endl;
	outputfile << "    " << "<Items>" << endl;
	vector<SaveInfo>::iterator it0;

	string lastname = detection_results.at(0).mediaFile;
	outputfile << "        " << "<Item imageName=\"" << detection_results.at(0).mediaFile << "\">" << endl;
	for (it0 = detection_results.begin();it0 != detection_results.end(); it0++)
	{
		if ((*it0).mediaFile!=lastname)
		{
			outputfile << "        " << "</Item>" << endl;
			cout << "<Item imageName=\"" << (*it0).mediaFile << "\">" << endl;
			outputfile << "        " << "<Item imageName=\"" << (*it0).mediaFile << "\">" << endl;
			outputfile << "            " << "<Label type=\"" << (*it0).type << "\" l=\"" << (*it0).left <<"\" t=\"" << (*it0).top <<"\" r=\"" 
				<< (*it0).right <<"\" b=\"" << (*it0).bottom <<"\" />" << endl;
			lastname = (*it0).mediaFile;
		}
		else
		{
			outputfile << "            " << "<Label type=\"" << (*it0).type << "\" l=\"" << (*it0).left <<"\" t=\"" << (*it0).top <<"\" r=\"" 
				<< (*it0).right <<"\" b=\"" << (*it0).bottom <<"\" />" << endl;
			lastname = (*it0).mediaFile;

		}
	}
    outputfile << "        " << "</Item>" << endl;    
	outputfile << "    " << "</Items>" << endl;
	outputfile << "</Message>" << endl;
	outputfile.close();
}
bool cmp(SaveInfo a, SaveInfo b)
{
	return a.mediaFile < b.mediaFile;
}





int main(int argc, char* argv[])
{
	//if (argc < 4)
	//{
	//	cout << "ERROR! SHORT OF ARGUMENT!" << endl;
	//	cout <<" INPUT AS BELOW:"<< endl;
	//	cout <<"[EXENAME]xmlmake" << endl << "[FOLDER NAME]dongnanmen_1_1280x720_30_2" << endl << "[INPUT TXT]dongnanmen_1_1280x720_30_2_20150718_delete.txt" << endl << "[OUPUT XML]dongnanmen_1_1280x720_30_2_20150718_delete.xml"<< endl;
	//	system("pause");
	//	return 1;
	//}

 //   string directoryName = argv[1];//"dongnanmen_1_1280x720_30_2";
	//char* fileName = argv[2];//"dongnanmen_1_1280x720_30_2_20150718_delete.txt";
	//string xml_name = argv[3];//dongnanmen_1_1280x720_30_2_20150718_delete.xml


	string directoryName = "dongnanmen_1_1280x720_30_2";
	char* fileName = "dongnanmen_1_1280x720_30_2_20150718_delete.txt";
	string xml_name = "dongnanmen.xml";

	int directoryInt = 0;
	if (directoryName == "hongsilounorth_13_1920x1080_30_1")
		directoryInt = 1;
	else if (directoryName == "hongsilouwest_14_1920x1080_30_1")
		directoryInt = 2;
	else if (directoryName == "weiminghubeieast_11_1920x1080_30_1")
		directoryInt = 3;
	else if (directoryName ==  "weiminghubeiwest_12_1920x1080_30_1")
		directoryInt = 4;
	else if (directoryName == "dongcemen_6_1280x720_30_2")
		directoryInt = 5;
	else if (directoryName == "dongnanmen_1_1280x720_30_2")
		directoryInt = 6;
	else if (directoryName == "weiminghudong_7_1280x720_30_2")
		directoryInt = 7;
	else if (directoryName == "yaoganqian_5_1280x720_30_2")
		directoryInt = 8;

	vector<SaveInfo> infoResults;
	char lc[100];
	ifstream file(fileName);
	string imgIndex;
	int numObj;
	int typenumber;
	double left1, top1, right1, bottom1;
	file >> imgIndex;
	while (!file.eof())
	{
		file  >> numObj;
		cout << "numObj: " << numObj << "imgIndex " << imgIndex << endl;
		for(int video=0;video<numObj;video++)
		{
			file >> left1 >> top1 >> right1 >> bottom1 >> typenumber;
			SaveInfo temp;
			temp.type=typenumber;
			temp.left=(int)left1 ;
			temp.right=(int)right1 ;
			temp.bottom=(int)bottom1;
			temp.top=(int)top1;
			temp.mediaFile=imgIndex;
			double area = (right1 - left1) * (bottom1 - top1);
			switch (directoryInt)
			{
			case 1:
				if (
				temp.top >= 35 
				&& temp.top >=(0.7389*temp.right - 865.4083)
				&& (!(temp.top> 350 && (area < 17000) ))
				&& (!((temp.top < 150 && (area / (0.394426533822960  * temp.top * temp.top  +	   17.6031699564880 * temp.top +  3971.36895494939)) > 1.8)))
				)
				{
					infoResults.push_back(temp);
					break;
				}
			case 2:
				if ((temp.top < 150 && (area / (0.352927773980407  * top1 * top1  +	   117.612212349054 * top1   -4451.57040328092)) > 1.4)
					|| (temp.type == 2 && temp.top> 150 && (area / (0.275599085788160  * top1 * top1  +	   206.652255286731 * top1   -12335.5906824269)) > 1.3)
					|| (temp.type == 404 && temp.top >500)
					||
					(area < 20000 && temp.top >600)
					)
					
				{
					continue;
				}
				else
				{
					infoResults.push_back(temp);
					break;
				}
			case 3:
				if 	(
					(area<=7920 && temp.top>=380) 
					 ||
					temp.right <= 479 
					||
					temp.top <=60
					||
					(temp.top< 500 && (area / (0.247385252026339  * top1 * top1  +	   27.0171792609519 * top1  +  8655.44790127104)) > 1.2)
					||
					(temp.top> 550 && temp.top< 850 && area <14000)
					||
					(temp.left > 1507 && temp.bottom < 415)
					||
					(temp.left > 1313 && temp.bottom < 271)
					||
					(temp.top < (0.7594 * temp.right - 856.901))
					||
					(temp.top < 0)
					)
				{
					continue;
				}
				else
				{
					infoResults.push_back(temp);
					break;
				};
			case 4:
				if (
					(temp.type == 2 && temp.top <500 && area >  1.2 *(   temp.top * temp.top *  0.100996755305837	+ temp.top * 206.186646390742 + 20502.9695693555))
					||
					(temp.type == 2 && temp.top > 350 && temp.left > 1000 && area < 30000)
					)
				{
					continue;
				}
				else
				{
					infoResults.push_back(temp);
					break;
				}
			//<<<<<<<<<<<<========================= 多类
			case 5:
				if ((!(temp.right>=1211 && temp.bottom > 643)) && (temp.top >= 42) && (temp.top >= -0.1 * temp.left + 95) && (!(temp.top <= 110 && area >= 10000)))
				{
					infoResults.push_back(temp);
					break;
				}
			case 6:
				if (
					((temp.left <= 68 && temp.top <= 171)) //区域约束
					|| ((temp.left <= 312 && temp.top <= 139)) //区域约束
					|| ((temp.right <= 221 && temp.bottom <= 202))//区域约束
					|| ((temp.right<=544 && temp.top >= 470)) //区域约束
					|| ((temp.left <= 325 && temp.bottom <= 154)) //区域约束
					|| ((temp.type == 1 && temp.top<=166 && temp.top >=120 && temp.right >=1230))//去掉静止车
					|| ((temp.left <= 72 && temp.top<=244 && temp.top >=208)) //左边错检
					|| ((temp.type == 1 && temp.top <= 66)) //车的上边界
					|| ((temp.type == 2 && temp.top <= 110)) //人的上边界
					|| (temp.type == 2 && (area / (0.0199678855005707  * top1 * top1  +	   54.5726823329214 * top1   -2428.62438488418)) > 1.2)//top约束
					)
				{
					continue;
				}
				else
				{
					infoResults.push_back(temp);
					break;
				}
			case 7:
				if (
				(temp.top > - 0.9953 * temp.left + 324.51) && (temp.top > -0.8793 * temp.right - 674.88) 
				&& (!(temp.right > 647 && temp.top < 188)) && (!( temp.top >= 130 && temp.bottom <= 364 && temp.left>= 600 && temp.right <= 701  ))
				&& temp.top >= 70
				)
				{
					infoResults.push_back(temp);
					break;
				}
			case 8:
				if (((temp.right>=1211 && temp.bottom > 643)) 
					|| (temp.left < 0) 
					|| ((temp.type == 1 && temp.top <= 45)) 
					|| ((temp.type == 2 && temp.top <= 65))
					||
					(temp.top <500 && area >  1.2 *(   temp.top * temp.top *  0.191442928353313	+ temp.top * 37.3776368005645 -1090.07443285875))
					||
					(temp.top > 350 && temp.top <600 && area <  0.8 *(   temp.top * temp.top *  0.0249903929811526	+ temp.top * 8.18465978238122 -1038.71633376331))
					)
				{
					continue;
				}
				else
				{
					infoResults.push_back(temp);
					break;
				}
			default:
				cout << "Foler Name Error!!!!" << endl;
			}		
		}	
		
		file >> imgIndex;
	}
	file.close();
	sort(infoResults.begin(),infoResults.end(),cmp);  
	xml_DetectionResults(directoryName,infoResults,xml_name,1);
	

	return 0;
}
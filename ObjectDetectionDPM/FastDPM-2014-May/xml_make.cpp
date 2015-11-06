#include "xml_make.h"
#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)

void xml_DetectionResults(string directoryName, vector<SaveInfo> detection_results, string xml_name, int evaluateTyte)
{
	ofstream outputfile(xml_name.c_str());
	outputfile << "<?xml version=\"1.0\" encoding=\"gb2312\" ?>" << endl;
	outputfile << "<Message Version=\"1.0\">" << endl;
	outputfile << "    " << "<Info evaluateType=\"" << evaluateTyte << "\" mediaFile=\"" << directoryName << "\" />" << endl;
	outputfile << "    " << "<Items>" << endl;
	vector<SaveInfo>::iterator it0;

	string lastname = detection_results.at(0).mediaFile;
	outputfile << "        " << "<Item imageName=\"" << detection_results.at(0).mediaFile << "\">" << endl;
	for (it0 = detection_results.begin(); it0 != detection_results.end(); it0++)
	{
		if ((*it0).mediaFile != lastname)
		{
			outputfile << "        " << "</Item>" << endl;
			cout << "<Item imageName=\"" << (*it0).mediaFile << "\">" << endl;
			outputfile << "        " << "<Item imageName=\"" << (*it0).mediaFile << "\">" << endl;
			outputfile << "            " << "<Label type=\"" << (*it0).type << "\" l=\"" << (*it0).left << "\" t=\"" << (*it0).top << "\" r=\""
				<< (*it0).right << "\" b=\"" << (*it0).bottom << "\" />" << endl;
			lastname = (*it0).mediaFile;
		}
		else
		{
			outputfile << "            " << "<Label type=\"" << (*it0).type << "\" l=\"" << (*it0).left << "\" t=\"" << (*it0).top << "\" r=\""
				<< (*it0).right << "\" b=\"" << (*it0).bottom << "\" />" << endl;
			lastname = (*it0).mediaFile;

		}
	}
	outputfile << "        " << "</Item>" << endl;
	outputfile << "    " << "</Items>" << endl;
	outputfile << "</Message>" << endl;
	outputfile.close();
}

bool cmp_type(SaveInfo a, SaveInfo b)
{
	return a.type < b.type;
}



vector<SaveInfo> deletecompare(vector<SaveInfo> OnePicData)
{
	if (OnePicData.size() <= 1)
		return OnePicData;
	sort(OnePicData.begin(), OnePicData.end(), cmp_type);
	int carsize = 0;
	while (OnePicData.at(carsize).type == 1 )
	{
		carsize++;
		if (carsize == OnePicData.size())
			break;
	}
	
	vector<SaveInfo>::iterator it0;
	vector<SaveInfo> temp;
	int length = OnePicData.size();
	int personsize = length - carsize;
	int personsize1 = personsize;
	int personbegin = carsize;
	if (carsize == 1)
	{
		for (int iter2 = 0; iter2 < carsize; iter2++)
			temp.push_back(OnePicData.at(iter2));
	}
	if (carsize >= 2)
	{
		int beginnumber = carsize;
		int i = 0;
		int ind = 0;
		int square1, square2, squareshare, squaremin;
		while (i <= carsize - 1)
		{
			square1 = (OnePicData.at(i).right - OnePicData.at(i).left) *
				(OnePicData.at(i).bottom - OnePicData.at(i).top);
			int j = i + 1;
			while (j <= carsize - 1)
			{
				square2 = (OnePicData.at(j).right - OnePicData.at(j).left) *
					(OnePicData.at(j).bottom - OnePicData.at(j).top);
				squareshare = max(min(OnePicData.at(i).right, OnePicData.at(j).right) - max(OnePicData.at(i).left, OnePicData.at(j).left), 0)
					* max(min(OnePicData.at(i).bottom, OnePicData.at(j).bottom) - max(OnePicData.at(i).top, OnePicData.at(j).top), 0);

				squaremin = min(square1, square2);
				
				if (squareshare / double(squaremin) > 0.7) 
				{
					for (int iter1 = i; iter1 < carsize - 1; iter1++)
					{
						OnePicData.at(iter1) = OnePicData.at(iter1 + 1);
					}
					carsize = carsize - 1;
				}
				else
				{
					j = j + 1;
				}
			}
			i = i + 1;
		}
		while (beginnumber > carsize)
		{
			beginnumber = carsize;
			i = 0;
			while (i <carsize)
			{
				square1 = (OnePicData.at(i).right - OnePicData.at(i).left) *
					(OnePicData.at(i).bottom - OnePicData.at(i).top);
				int j = i + 1;
				while (j <= carsize - 1)
				{
					square2 = (OnePicData.at(j).right - OnePicData.at(j).left) *
						(OnePicData.at(j).bottom - OnePicData.at(j).top);
					squareshare = max(min(OnePicData.at(i).right, OnePicData.at(j).right) - max(OnePicData.at(i).left, OnePicData.at(j).left), 0)
						* max(min(OnePicData.at(i).bottom, OnePicData.at(j).bottom) - max(OnePicData.at(i).top, OnePicData.at(j).top), 0);
					squaremin = min(square1, square2);
					if (squareshare / double(squaremin) > 0.7)
					{
						for (int iter1 = i; iter1 < carsize - 1; iter1++)
						{
							OnePicData.at(iter1) = OnePicData.at(iter1 + 1);
						}
						carsize = carsize - 1;
					}
					else
					{
						j = j + 1;
					}
				}
				i = i + 1;
			}
		}
		for (int iter2 = 0; iter2 < carsize; iter2++)
			temp.push_back(OnePicData.at(iter2));	
	}
	if (personsize == 1)
	{
		for (int iter2 = personbegin; iter2 < personbegin + personsize; iter2++)
			temp.push_back(OnePicData.at(iter2));
	}
	if (personsize >= 2)
	{
		int beginnumber = personsize;
		int i = personbegin;
		int ind = 0;
		int square1, square2, squareshare, squaremin;
		while (i <= length - 1)
		{
			square1 = (OnePicData.at(i).right - OnePicData.at(i).left) *
				(OnePicData.at(i).bottom - OnePicData.at(i).top);
			int j = i + 1;
			while (j <= length - 1)
			{
				square2 = (OnePicData.at(j).right - OnePicData.at(j).left) *
					(OnePicData.at(j).bottom - OnePicData.at(j).top);
				squareshare = max(min(OnePicData.at(i).right, OnePicData.at(j).right) - max(OnePicData.at(i).left, OnePicData.at(j).left), 0)
					* max(min(OnePicData.at(i).bottom, OnePicData.at(j).bottom) - max(OnePicData.at(i).top, OnePicData.at(j).top), 0);
				squaremin = min(square1, square2);
				if (squareshare / double(squaremin) > 0.8)
				{
					for (int iter1 = i; iter1 < carsize - 1; iter1++)
					{
						OnePicData.at(iter1) = OnePicData.at(iter1 + 1);
					}
					personsize = personsize - 1;
					length = length - 1;
				}
				else
				{
					j = j + 1;
				}
			}
			i = i + 1;
		}
		while (beginnumber > personsize)
		{
			beginnumber = personsize;
			i = personbegin;
			while (i <length)
			{
				square1 = (OnePicData.at(i).right - OnePicData.at(i).left) *
					(OnePicData.at(i).bottom - OnePicData.at(i).top);
				int j = i + 1;
				while (j <= length - 1)
				{
					square2 = (OnePicData.at(j).right - OnePicData.at(j).left) *
						(OnePicData.at(j).bottom - OnePicData.at(j).top);
					squareshare = max(min(OnePicData.at(i).right, OnePicData.at(j).right) - max(OnePicData.at(i).left, OnePicData.at(j).left), 0)
						* max(min(OnePicData.at(i).bottom, OnePicData.at(j).bottom) - max(OnePicData.at(i).top, OnePicData.at(j).top), 0);
					squaremin = min(square1, square2);
					if (squareshare / double(squaremin) > 0.8)
					{
						for (int iter1 = i; iter1 < personsize - 1; iter1++)
						{
							OnePicData.at(iter1) = OnePicData.at(iter1 + 1);
						}
						personsize = personsize - 1;
						length = length - 1;
					}
					else
					{
						j = j + 1;
					}
				}
				i = i + 1;
			}
		}
		for (int iter2 = personbegin; iter2 < personbegin + personsize; iter2++)
			temp.push_back(OnePicData.at(iter2));
	}
	return temp;
}


void xml_DetectionResults_chongfu(string directoryName, vector<SaveInfo> detection_results, string xml_name, int evaluateTyte)
{
	ofstream outputfile(xml_name.c_str());
	outputfile << "<?xml version=\"1.0\" encoding=\"gb2312\" ?>" << endl;
	outputfile << "<Message Version=\"1.0\">" << endl;
	outputfile << "    " << "<Info evaluateType=\"" << evaluateTyte << "\" mediaFile=\"" << directoryName << "\" />" << endl;
	outputfile << "    " << "<Items>" << endl;
	vector<SaveInfo>::iterator it0;
	vector<SaveInfo> OnePicData;
	string lastname = detection_results.at(0).mediaFile;
	outputfile << "        " << "<Item imageName=\"" << detection_results.at(0).mediaFile << "\">" << endl;
	bool firstFlag = false;
	for (it0 = detection_results.begin(); it0 != detection_results.end(); it0++)
	{
		if ((*it0).mediaFile != lastname)
		{
			if ((*it0).mediaFile == "0024356.jpg")
				vector<SaveInfo> Result = deletecompare(OnePicData);
			vector<SaveInfo> Result = deletecompare(OnePicData);
			for (vector<SaveInfo>::iterator it1 = Result.begin(); it1 != Result.end(); it1++)
			{
				outputfile << "            " << "<Label type=\"" << (*it1).type << "\" l=\"" << (*it1).left << "\" t=\"" << (*it1).top << "\" r=\""
					<< (*it1).right << "\" b=\"" << (*it1).bottom << "\" />" << endl;
			}
			OnePicData.clear();
			outputfile << "        " << "</Item>" << endl;
			cout << "<Item imageName=\"" << (*it0).mediaFile << "\">" << endl;
			outputfile << "        " << "<Item imageName=\"" << (*it0).mediaFile << "\">" << endl;

			OnePicData.push_back(*it0);

			lastname = (*it0).mediaFile;
		}
		else
		{
			OnePicData.push_back(*it0);
			lastname = (*it0).mediaFile;

		}

	}
	vector<SaveInfo> Result = deletecompare(OnePicData);
	for (vector<SaveInfo>::iterator it1 = Result.begin(); it1 != Result.end(); it1++)
	{
		outputfile << "            " << "<Label type=\"" << (*it1).type << "\" l=\"" << (*it1).left << "\" t=\"" << (*it1).top << "\" r=\""
			<< (*it1).right << "\" b=\"" << (*it1).bottom << "\" />" << endl;
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


int xml_make(string directoryName, const char* fileName, string xml_name)
{
	int directoryInt = 0;
	if (directoryName == "hongsilounorth_13_1920x1080_30_1")
		directoryInt = 1;
	else if (directoryName == "hongsilouwest_14_1920x1080_30_R1")
		directoryInt = 2;
	else if (directoryName == "weiminghubeieast_11_1920x1080_30_R1")
		directoryInt = 3;
	else if (directoryName == "weiminghubeiwest_12_1920x1080_30_R1")
		directoryInt = 4;
	else if (directoryName == "dongcemen_6_1280x720_30_R1")
		directoryInt = 5;
	else if (directoryName == "dongnanmen_1_1280x720_30_R1")
		directoryInt = 6;
	else if (directoryName == "weiminghudong_7_1280x720_30_R1")
		directoryInt = 7;
	else if (directoryName == "yaoganqian_5_1280x720_30_R1")
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
		file >> numObj;
		for (int video = 0; video<numObj; video++)
		{
			file >> left1 >> top1 >> right1 >> bottom1 >> typenumber;
			if (typenumber == 404)
			{
				cout << "wrong answer!" << endl;
				continue;
			}
			SaveInfo temp;
			temp.type = typenumber;
			temp.left = (int)left1;
			temp.right = (int)right1;
			temp.bottom = (int)bottom1;
			temp.top = (int)top1;
			temp.mediaFile = imgIndex;
			double area = (right1 - left1) * (bottom1 - top1);
			switch (directoryInt)
			{
			case 1:
				if (
					temp.top >= 35
					&& temp.top >= (0.7389*temp.right - 865.4083)
					&& (!(temp.top> 350 && (area < 17000)))
					&& (!((temp.top < 150 && (area / (0.394426533822960  * temp.top * temp.top + 17.6031699564880 * temp.top + 3971.36895494939)) > 1.8)))
					)
				{
					infoResults.push_back(temp);
					break;
				}
			case 2:
				if ((temp.top < 150 && (area / (0.352927773980407  * top1 * top1 + 117.612212349054 * top1 - 4451.57040328092)) > 1.4)
					|| (temp.type == 2 && temp.top> 150 && (area / (0.275599085788160  * top1 * top1 + 206.652255286731 * top1 - 12335.5906824269)) > 1.3)
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
				if (
					(area <= 7920 && temp.top >= 380)
					||
					temp.right <= 479
					||
					temp.top <= 60
					||
					(temp.top< 500 && (area / (0.247385252026339  * top1 * top1 + 27.0171792609519 * top1 + 8655.44790127104)) > 1.2)
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
					(temp.type == 2 && temp.top <500 && area >  1.2 *(temp.top * temp.top *  0.100996755305837 + temp.top * 206.186646390742 + 20502.9695693555))
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
				if (
					(!(temp.right >= 1211 && temp.bottom > 643)) 
					&& (temp.top >= 42) 
					//&& (temp.top >= -0.1 * temp.left + 95) 
					&& (!(temp.top <= 110 && area >= 10000))
					)
				{
					infoResults.push_back(temp);
					break;
				}
				else
				{
					cout << "delete!~~~~" << endl;
					break;
				}
			case 6:
				if (
					((temp.left <= 68 && temp.top <= 171)) //区域约束
					|| ((temp.left <= 312 && temp.top <= 139)) //区域约束
					|| ((temp.right <= 221 && temp.bottom <= 202))//区域约束
					|| ((temp.right <= 544 && temp.top >= 470)) //区域约束
					|| ((temp.left <= 325 && temp.bottom <= 154)) //区域约束
					|| ((temp.type == 1 && temp.top <= 166 && temp.top >= 120 && temp.right >= 1230))//去掉静止车
					|| ((temp.left <= 72 && temp.top <= 244 && temp.top >= 208)) //左边错检
					|| ((temp.type == 1 && temp.top <= 66)) //车的上边界
					|| ((temp.type == 2 && temp.top <= 110)) //人的上边界
					|| (temp.type == 2 && (area / (0.0199678855005707  * top1 * top1 + 54.5726823329214 * top1 - 2428.62438488418)) > 1.2)//top约束
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
				//	(temp.top > -0.9953 * temp.left + 324.51) && (temp.top > -0.8793 * temp.right - 674.88)
					//&& (!(temp.right > 647 && temp.top < 188)) && (!(temp.top >= 130 && temp.bottom <= 364 && temp.left >= 600 && temp.right <= 701))
					//&& 
					temp.top >= 70
					)
				{
					infoResults.push_back(temp);
					break;
				}
			case 8:
				if (((temp.right >= 1211 && temp.bottom > 643))
					|| (temp.left < 0)
					|| ((temp.type == 1 && temp.top <= 45))
					|| ((temp.type == 2 && temp.top <= 65))
					||
					(temp.type == 2 && temp.top <500 && area >  1.2 *(temp.top * temp.top *  0.191442928353313 + temp.top * 37.3776368005645 - 1090.07443285875))
					||
					(temp.type == 2 && temp.top > 350 && temp.top <600 && area <  0.8 *(temp.top * temp.top *  0.0249903929811526 + temp.top * 8.18465978238122 - 1038.71633376331))
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
	sort(infoResults.begin(), infoResults.end(), cmp);
	if (directoryInt == 7 || directoryInt == 6 || directoryInt == 8 || directoryInt == 5)
	{
		cout << "Test!" << endl;
		xml_DetectionResults_chongfu(directoryName, infoResults, xml_name, 1);
	}
	else
		xml_DetectionResults(directoryName, infoResults, xml_name, 1);


	return 0;
}
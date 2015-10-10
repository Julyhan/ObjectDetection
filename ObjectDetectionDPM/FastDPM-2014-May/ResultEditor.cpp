#include "ResultEditor.h"
#include <algorithm>

double ab(double n)
{
	if(n >= 0) 
		return n ;
	else 
		return -n ; 
}

double AABB(double x01, double y01, double x02, double y02, double x11, double y11, double x12, double y12)
{
	double zx = ab(x01+x02-x11-x12) ; //两个矩形重心在x轴上的距离的两倍
	double x = ab(x01-x02)+ab(x11-x12) ; //两矩形在x方向的边长的和
	double zy = ab(y01+y02-y11-y12) ;   //重心在y轴上距离的两倍
	double y = ab(y01-y02)+ab(y11-y12) ; //y方向边长的和
	if(zx < x && zy < y)
	{
		double area = (min(x02,x12)-max(x01,x11))*(min(y02,y12)-max(y01,y11));
		return area/( ab(x01-x02)*ab(y01-y02) + ab(x11-x12)*ab(y11-y12) - area);
	}
	else 
		return 0;
}

void RemoveData(vector<data > frame0, vector<data > frame1, vector<data > frame2, vector<data > frame3, vector<data > frame4, vector<data > &removed, vector<data > &removedAll)
{
	vector<data >::iterator it0,it1,it2,it3,it4;
	for( it2=frame2.begin(); it2!=frame2.end();it2++ )
	{
		if( (*it2).flag == 2 )
		{
			bool shouldberm = true;
			for( it0=frame0.begin(); it0!=frame0.end();it0++ )
			{
				if( (*it0).flag == (*it2).flag )
					if( AABB((*it0).x01,(*it0).y01,(*it0).x02,(*it0).y02,(*it2).x01,(*it2).y01,(*it2).x02,(*it2).y02) > THRESHOLD )
						shouldberm = false;
			}
			for( it1=frame1.begin(); it1!=frame1.end();it1++ )
			{
				if( (*it1).flag == (*it2).flag )
					if( AABB((*it1).x01,(*it1).y01,(*it1).x02,(*it1).y02,(*it2).x01,(*it2).y01,(*it2).x02,(*it2).y02) > THRESHOLD )
						shouldberm = false;
			}
			for( it3=frame3.begin(); it3!=frame3.end();it3++ )
			{
				if( (*it3).flag == (*it2).flag )
					if( AABB((*it3).x01,(*it3).y01,(*it3).x02,(*it3).y02,(*it2).x01,(*it2).y01,(*it2).x02,(*it2).y02) > THRESHOLD )
						shouldberm = false;
			}
			for( it4=frame4.begin(); it4!=frame4.end();it4++ )
			{
				if( (*it4).flag == (*it2).flag )
					if( AABB((*it4).x01,(*it4).y01,(*it4).x02,(*it4).y02,(*it2).x01,(*it2).y01,(*it2).x02,(*it2).y02) > THRESHOLD )
						shouldberm = false;
			}
			if(shouldberm)
			{
				removed.push_back((*it2));
				removedAll.push_back((*it2));
			}
		}
	}
}

int Result_Removing( const char * FileName, const char * OutputFile)
{
	//char* fileName = "dongnanmen_1_1280x720_30_2_20150718.txt";
	ifstream file(FileName);

	string imgIndex;
	int numObj;
	int typenumber;

	vector<data > frame[5];
	int curindex = 0;
	data tempdata;
	// 获取填充数据
	vector<data > removed;
	vector<data > removedAll;

	for (int i = 0; i < 4; i++)
	{
		frame[curindex].clear();
		file >> tempdata.imgname;
		numObj = -1;
		file  >> numObj;
		if (numObj >= 0)
		{
			for (int video = 0; video<numObj; video++)
			{
				file >> tempdata.x01 >> tempdata.y01 >> tempdata.x02 >> tempdata.y02 >> typenumber; //>> level >> 
				frame[curindex].push_back(tempdata);
			}
			curindex = (curindex + 1) % 5;
		}
	}

	ofstream outputfile(OutputFile);
	vector<data >::iterator it0,it1;

	int count = 0;
	while (!file.eof())
	{
		cout<< count++ << endl;
		frame[curindex].clear();
		removed.clear();
		file >> tempdata.imgname;
		file  >> numObj;
		for(int video=0;video<numObj;video++)
		{
			file >> tempdata.x01 >> tempdata.y01 >> tempdata.x02 >> tempdata.y02 >> tempdata.flag; //>> level >> 
			frame[curindex].push_back(tempdata);
		}
		RemoveData(frame[(curindex+1)%5],frame[(curindex+2)%5],frame[(curindex+3)%5],frame[(curindex+4)%5],frame[curindex],removed,removedAll);
		curindex = (curindex+1)%5;
	}

	file.close();

	// 新的数据
	vector<data > writedata;
	file.open(FileName);
	while (!file.eof())
	{
		// clear the input buffer
		writedata.clear();

		// fill the buffer
		frame[curindex].clear();
		file >> tempdata.imgname;
		numObj = -1;
		file >> numObj;
		if (numObj >= 0)
		{
			for (int video = 0; video<numObj; video++)
			{
				file >> tempdata.x01 >> tempdata.y01 >> tempdata.x02 >> tempdata.y02 >> tempdata.flag; //>> level >> 
				frame[curindex].push_back(tempdata);
			}

			// processing
			bool NoOneNeedBeRemoved = true;
			for (it0 = frame[curindex].begin(); it0 != frame[curindex].end(); it0++)
			{
				NoOneNeedBeRemoved = true;
				for (it1 = removedAll.begin(); it1 != removedAll.end(); it1++)
				{
					if ((*it0).imgname == (*it1).imgname && (*it0).x01 == (*it1).x01 && (*it0).y01 == (*it1).y01 && (*it0).x02 == (*it1).x02 && (*it0).y02 == (*it1).y02 && (*it0).flag == (*it1).flag)
					{
						NoOneNeedBeRemoved = false;
						break;
					}
				}
				if (NoOneNeedBeRemoved)
					writedata.push_back(*it0);
				//outputfile << (*it0).x01 << '\t'<< (*it0).y01 << '\t'<< (*it0).x02 << '\t'<< (*it0).y02 << '\t'<< (*it0).flag << '\n';
				else
					removedAll.erase(it1);
				//outputfile << (*it0).x01 << '\t'<< (*it0).y01 << '\t'<< (*it0).x02 << '\t'<< (*it0).y02 << '\t'<< 404 << '\t';			
			}

			// output the buffer
			bool first = true;
			for (it0 = writedata.begin(); it0 != writedata.end(); it0++)
			{
				if (first == true)
				{
					first = false;
					outputfile << (*it0).imgname << '\t' << writedata.size() << '\n';
				}
				outputfile << (*it0).x01 << '\t' << (*it0).y01 << '\t' << (*it0).x02 << '\t' << (*it0).y02 << '\t' << (*it0).flag << '\n';
			}
		}
	}

	file.close();
	outputfile.close();

	return 0;
}

void GetMissedData(vector<data > frame0, vector<data > frame1, vector<data > frame2, vector<data > &missed)
{
	vector<data >::iterator it0,it1,it2;
	for( it0=frame0.begin(); it0!=frame0.end();it0++ )
	{
		// 先判断第二帧上没有
		bool not = true;
		for( it1=frame1.begin(); it1!=frame1.end();it1++ )
		{
			if( (*it0).flag == (*it1).flag )
				if( AABB((*it0).x01,(*it0).y01,(*it0).x02,(*it0).y02,(*it1).x01,(*it1).y01,(*it1).x02,(*it1).y02) > THRESHOLD )
					not = false;
		}

		if( not == true )
		{
			vector<data >::iterator tempit;
			// 第三帧上寻找最匹配
			double maxmatch = 0;
			for( it2=frame2.begin(); it2!=frame2.end();it2++ )
			{
				if( (*it0).flag == (*it2).flag )
				{
					double tempmatch = AABB((*it0).x01,(*it0).y01,(*it0).x02,(*it0).y02,(*it2).x01,(*it2).y01,(*it2).x02,(*it2).y02);
					if( tempmatch > maxmatch )
					{
						maxmatch = tempmatch;
						tempit = it2;
					}
				}
			}
			if (maxmatch > THRESHOLD && frame1.size() > 0)
			{
				data tempdata;
				tempdata.imgname = frame1[0].imgname;
				tempdata.flag = (*tempit).flag;
				tempdata.x01 = ((*tempit).x01+(*it0).x01)/2;
				tempdata.x02 = ((*tempit).x02+(*it0).x02)/2;
				tempdata.y01 = ((*tempit).y01+(*it0).y01)/2;
				tempdata.y02 = ((*tempit).y02+(*it0).y02)/2;
				missed.push_back(tempdata);
			}
		}
	}
}


int Result_Increasing( const char * FileName, const char * OutputFile )
{
	{
		ifstream file(FileName);

		string imgIndex;
		int numObj;
		int typenumber;
		int curindex = -1;

		vector<data> alldetect;
		data tempdata;

		vector<vector<data>> frame;
		vector<data> missed;

		ofstream outputfile(OutputFile);
		while (!file.eof())
		{
			file >> tempdata.imgname;
			numObj = -1;
			file >> numObj;
			if (numObj >= 0)
			{
				for (size_t i = 0; i < numObj; i++)
				{
					file >> tempdata.x01 >> tempdata.y01 >> tempdata.x02 >> tempdata.y02 >> typenumber;
					alldetect.push_back(tempdata);
				}
			}
		}
		sort(alldetect.begin(), alldetect.end(), [](data test1, data test2){return test1.imgname < test2.imgname; });

		if (!alldetect.empty())
		{
			size_t idx = 0;
			string last_name = "";
			while (idx < alldetect.size())
			{
				if (alldetect[idx].imgname == last_name)
				{
					frame[curindex].push_back(alldetect[idx]);
				}
				else
				{
					curindex++;
					last_name = alldetect[idx].imgname; 
					frame.push_back(missed);
					frame[curindex].push_back(alldetect[idx]);
				}
				idx++;
			}
		}//END if (!alldetect.empty())

		vector<data >::iterator it0;
		curindex = 0;
		int count = frame.size() - 2;
		while (curindex < count)///////
		{
			missed.clear();
			auto name = frame[(curindex + 2)][0].imgname;
			GetMissedData(frame[(curindex)],frame[(curindex+1)],frame[curindex+2],missed);
			// output
			if (missed.size() > 0)
			{
				for (size_t i = 0; i < missed.size(); i++)
				{
					frame[(curindex + 1)].push_back(missed[i]);
				}
				
			}
			curindex++;
		}


		for (size_t i = 0; i < frame.size(); i++)
		{
			outputfile << frame[i][0].imgname << '\n' << frame[i].size() << '\n';
			for (size_t j = 0; j < frame[i].size(); j++)
			{
				tempdata = frame[i][j];
				outputfile << tempdata.x01 << '\t' << tempdata.y01 << '\t' << tempdata.x02 << '\t' << tempdata.y02 << '\t' << typenumber << '\n';
			}
		}

		file.close();
		outputfile.close();
	}



	return 0;
}

#pragma warning(disable:4819)
#pragma warning(disable:4996)

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include "windows.h"
#include "time.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include "cv.h"
#include "highgui.h"
using namespace cv;

#include "Encrypt.h"
#include "Util.h"

Util::Util(void)
{
}

Util::~Util(void)
{
}

void Util::enumFiles(const char *lpPath, const char *lpExtension, std::vector<string>& vFiles)
{	
	char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	strcpy(szFind,lpPath);
	strcat(szFind,"\\*");
	strcat(szFind, lpExtension);
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);

	//SCPP_ASSERT(INVALID_HANDLE_VALUE != hFind, "Can not find path: " << szFind);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}

	char szFound[MAX_PATH];
	memset(szFound, 0, MAX_PATH);
	szFound[0] = '\0';

	while(TRUE)
	{
		if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			memset(szFound, 0, MAX_PATH);
			szFound[0] = '\0';
			strcpy(szFound, lpPath);
			strcat(szFound, "\\");
			strcat(szFound, FindFileData.cFileName);
			//pString = new string(szFound);
			string strPath(szFound);
			vFiles.push_back(strPath);
		}
		if(!FindNextFile(hFind,&FindFileData))    break;
	}
	FindClose(hFind);
}

void Util::enumFolders(const char* lpPath, vector<string>& vFolders)
{
	if (NULL == lpPath)
	{
		return;
	}

	char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	strcpy(szFind,lpPath);
	strcat(szFind,"\\*.*");
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)    return;

	const string dirPath = string(lpPath);
	char szFound[MAX_PATH];
	memset(szFound, 0, MAX_PATH);
	szFound[0] = '\0';

	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!='.')
			{
				memset(szFound, 0, MAX_PATH);
				szFound[0] = '\0';
				strcpy(szFound, lpPath);
				strcat(szFound, "\\");
				strcat(szFound, FindFileData.cFileName);
				string path(szFound);
				//if (dirPath == path)
				//{
				//	continue;
				//}
				vFolders.push_back(szFound);
				enumFolders(path.c_str(), vFolders);
			}	
		}
		if(!FindNextFile(hFind,&FindFileData))    break;
	}
	FindClose(hFind);
} 

bool Util::encrypt(const std::string srcPath, const std::string dstPath, const int FILE_SIZE)
{
	ifstream Readfile(srcPath.c_str());
	if (!Readfile.is_open())
	{
		cerr << "Can not open file: " << srcPath << endl;
		return false;
	}
	//
	ofstream Savefile(dstPath.c_str());
	if (!Savefile.is_open())
	{
		cerr << "Can not open file: " << dstPath << endl;
		Readfile.close();
		return false;
	}

	char *source = new char[FILE_SIZE];
	if (NULL == source)
	{
	}
	
	char ch;
	
	memset(source, 0, FILE_SIZE);
	int sou_i=0;
	char pass[] = "12345678";
	char *result1 = NULL;
	char *result2 = NULL;
	while(!Readfile.eof())
	{
		Readfile.get(ch);
		source[sou_i]=ch;
		sou_i+=1;
	}
	result1 = Encrypt(source, pass);
	Savefile<<result1;
	sou_i=0;
	//result2 = Decrypt(result1, pass);
	//printf("result2=%s\n", result2);
	delete []result1;
	//delete []result2;
	Savefile.close();
	Readfile.close();

	delete []source;
	source = NULL;

	return true;
}

int Util::randomVector(const int minValue, const int maxValue, vector<int>& target)
{
	if (minValue >= maxValue)
	{
		return -1;
	}
	target.clear();
	target.resize(maxValue - minValue);
	vector<int> randomSet(maxValue - minValue);
	srand((int)time(NULL));
	for (int i = minValue; i < maxValue; ++i)
	{
		target[i-minValue] = i;
		randomSet[i-minValue] = rand() % (maxValue - minValue) + minValue;
	}

	int temp = -1;
	for (int i = minValue; i < maxValue; ++i)
	{
		temp = target[i - minValue];
		target[i - minValue] = target[randomSet[i - minValue] - minValue];
		target[randomSet[i-minValue] - minValue] = temp;
	}

	return 0;
}

 
std::vector<std::string> Util::split(std::string str,std::string pattern) 
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str+=pattern;							//扩展字符串以方便操作
	int size=str.size(); 
	for(int i=0; i<size; i++)
	{
		pos=str.find(pattern,i);
		if(pos<size)
		{
			std::string s=str.substr(i,pos-i);
			result.push_back(s);
			i=pos+pattern.size()-1;
		}
	}
	return result;
}
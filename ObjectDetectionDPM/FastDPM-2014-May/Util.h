#pragma once

#include <string>
#include <vector>

class __declspec(dllexport) Util
{
public:
	Util(void);
	~Util(void);

	static void enumFolders(const char* lpPath, std::vector<std::string>& vFolders);
	static void enumFiles(const char *lpPath, const char *lpExtension, std::vector<string>& vFiles);
	static bool encrypt(const std::string srcPath, const std::string dstPath, const int FILE_SIZE);
	static int randomVector(const int minValue, const int maxValue, vector<int>& target);
	static std::vector<std::string> split(std::string str,std::string pattern);
};

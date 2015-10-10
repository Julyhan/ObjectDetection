#pragma warning(disable:4819)
#pragma warning(disable:4996)

#include <iostream>
#include <string>
using namespace std;

#include "Common.h"
#include "SaveInfo.h"

CSaveInfo::CSaveInfo(const SaveInfo& saveInfo)
{
	mediaFile_ = saveInfo.mediaFile;

	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];

	memset(buffer, 0, BUFFER_SIZE);
	sprintf(buffer, "%d", saveInfo.type);
	type_ = string(buffer);

	memset(buffer, 0, BUFFER_SIZE);
	sprintf(buffer, "%d", saveInfo.frameSeq);
	frameSeq_ = string(buffer);

	memset(buffer, 0, BUFFER_SIZE);
	sprintf(buffer, "%d", saveInfo.left);
	left_ = string(buffer);

	memset(buffer, 0, BUFFER_SIZE);
	sprintf(buffer, "%d", saveInfo.top);
	top_ = string(buffer);

	memset(buffer, 0, BUFFER_SIZE);
	sprintf(buffer, "%d", saveInfo.right);
	right_ = string(buffer);

	memset(buffer, 0, BUFFER_SIZE);
	sprintf(buffer, "%d", saveInfo.bottom);
	bottom_ = string(buffer);

	memset(buffer, 0, BUFFER_SIZE);
	sprintf(buffer, "%d", saveInfo.id);
	id_ = string(buffer);
}

CSaveInfo::~CSaveInfo(void)
{
}

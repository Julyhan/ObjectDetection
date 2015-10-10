#ifndef Defines_H
#define Defines_H


///@brief
///评测类型定义
enum EnumEvaluateType
{
	EnumEvaluateType_Null = 0,


	/*单摄像头多类对象检测评测*/
	EnumEvaluateType_SingleObjects = 1,

	/*单摄像头指定对象跟踪*/
	EnumEvaluateType_SingleObjectTrack = 2,

	/*跨摄像头指定行人跟踪*/
	EnumEvaluateType_MultiPedestrian = 3,

	/*人脸识别*/
	EnumEvaluateType_FaceRecog = 4,

	/*异常行为识别*/
	EnumEvaluateType_AnomalyDetection = 5,

	/*跨摄像头指定车辆跟踪*/
	EnumEvaluateType_MultiVehicle = 8,

	/*单摄像头行人评测*/
	EnumEvaluateType_SinglePedestrian = 9,

	/*人脸检测*/
	EnumEvaluateType_FaceDetection = 10,


};


///@brief
///目标类型定义
enum EnumObjectType
{
	/*人脸*/
	EnumObjectType_Face = 0,

	/*车*/
	EnumObjectType_Vehicle = 1,

	/*行人*/
	EnumObjectType_Pedestrian = 2,

};


typedef struct ObjectStruct
{
	int objectType;
	int frameNum;
	int left;
	int top;
	int right;
	int bottom;
	float score;
	bool isOk;

}ObjectStruct;


typedef struct FaceRecogStruct
{
	int pic;
	int id;
	float score;
	bool isOk;

}FaceRecogStruct;

typedef struct __AnomalyDetectionStruct
{
	int abn;
	int startFrame;
	int endFrame;
	std::map<int, std::vector<int>> abnSegOfFrame;
} AnomalyDetStruct;

#endif



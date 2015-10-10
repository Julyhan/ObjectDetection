#include "FastDPM.h"
#include "Get_BG.h"
#include <conio.h>
#include <fstream>
#include <direct.h>
#include "xml_make.h"
#include "ResultEditor.h"

using namespace cv;

vector<string>  yuStdDirFiles( string DirName, vector<string> FileExtensions );
string trim_file_name( string FileName, int FLAG );

bool SHOWIMAGE = false;
int detectPic(const char* bgPic_str, double largetimes, string img_dir, string save_txt, string modelname, int labelname, float detectthresh)
{
	/* I. Get images */
	string extensions[] = { ".jpg" };
	vector<string>	img_extensions(extensions, extensions + 1);
	vector<string>	imgnames = yuStdDirFiles(img_dir, img_extensions);

	/* II. Perform Part_Model based detection */
	FastDPM	PM(modelname);
	IplImage* bgPic = cvLoadImage(bgPic_str);

	bool	flag = false;
	for (unsigned i = 0; i<imgnames.size(); i++){
		auto length = img_dir.length();
		string	img_name1 = imgnames[i].substr(length + 1);
		string	img_name = imgnames[i];
		ofstream fid(save_txt, ios::app);
		fid << img_name1 << endl;
		fid.close();

		Mat img_uint8 = Remove_BG_OnePic(img_name.c_str(), bgPic, largetimes);
		if (img_uint8.empty()){
			cout << "Cannot get image " << img_name << endl;
			getchar();
			return -2;
		}
		cout << "Processing " << trim_file_name(img_name, 0) << endl;

		Mat	img = PM.prepareImg(img_uint8);
		/* double largetimes, int labelname, string save_txt, float score_thresh*/
		PM.detect(largetimes, labelname, save_txt, img, detectthresh, false, SHOWIMAGE);
		cout << "------------------------------------------------------------" << endl;
	}
	cvReleaseImage(&bgPic);
	return 0;
}
int detect_yaoganqian(double largetimes, string img_dir, string save_txt, string modelname, int labelname, float detectthresh)
{
	/* I. Get images */
	string extensions[] = { ".jpg" };
	vector<string>	img_extensions(extensions, extensions + 1);
	vector<string>	imgnames = yuStdDirFiles(img_dir, img_extensions);

	/* II. Perform Part_Model based detection */
	FastDPM	PM(modelname);

	bool	flag = false;
	for (unsigned i = 0; i<imgnames.size(); i++){
		auto length = img_dir.length();
		string	img_name1 = imgnames[i].substr(length + 1);
		string	img_name = imgnames[i];
		ofstream fid(save_txt, ios::app);
		fid << img_name1 << endl;
		fid.close();
		Mat img_uint8 = imread(img_name.c_str(), 1);
		if (img_uint8.empty()){
			cout << "Cannot get image " << img_name << endl;
			getchar();
			return -2;
		}
		cout << "Processing " << trim_file_name(img_name, 0) << endl;

		Mat img2 = img_uint8.rowRange(0, 249);
		img2 = img2.colRange(int(1280/2.5*1.5), 1279);
		resize(img2, img2, Size(int(512 * largetimes), int(250 * largetimes)));
		Mat	img = PM.prepareImg(img2);
		PM.detect(largetimes, labelname, save_txt, img, detectthresh, false, SHOWIMAGE, "yaoganqian_yuanche.jpg");
		cout << "------------------------------------------------------------" << endl;
	}
	return 0;
}

int detect_weiminghudong(const char* bgPic_str, double largetimes, string img_dir, string save_txt, string modelname, int labelname, float detectthresh)
{
	/* I. Get images */
	string extensions[] = { ".jpg" };
	vector<string>	img_extensions(extensions, extensions + 1);
	vector<string>	imgnames = yuStdDirFiles(img_dir, img_extensions);

	/* II. Perform Part_Model based detection */
	FastDPM	PM(modelname);
	IplImage* bgPic = cvLoadImage(bgPic_str);

	bool	flag = false;
	for (unsigned i = 0; i<imgnames.size(); i++){
		auto length = img_dir.length();
		string	img_name1 = imgnames[i].substr(length + 1);
		string	img_name = imgnames[i];
		ofstream fid(save_txt, ios::app);
		fid << img_name1 << endl;
		fid.close();

		Mat img2 = imread(img_name.c_str(), 1); 
		img2 = img2.rowRange(0, 249);
		img2 = img2.colRange(0, 499);
		if (img2.empty()){
			cout << "Cannot get image " << img_name << endl;
			getchar();
			return -2;
		}
		cout << "Processing " << trim_file_name(img_name, 0) << endl;

		Mat img_uint8 = Remove_BG_OnePic_mat(img2, bgPic, largetimes);


		resize(img_uint8, img_uint8, Size(int(500 * largetimes), int(250 * largetimes)));

		Mat	img = PM.prepareImg(img_uint8);
		PM.detect(largetimes, labelname, save_txt, img, detectthresh, false, SHOWIMAGE,"weiminghudong_part.bmp");
		cout << "------------------------------------------------------------" << endl;
	}
	cvReleaseImage(&bgPic);
	return 0;
}


int detect_dongcemen(double largetimes, string img_dir, string save_txt, string modelname, int labelname, float detectthresh)
{
	/* I. Get images */
	string extensions[] = { ".jpg" };
	vector<string>	img_extensions(extensions, extensions + 1);
	vector<string>	imgnames = yuStdDirFiles(img_dir, img_extensions);

	/* II. Perform Part_Model based detection */
	FastDPM	PM(modelname);

	bool	flag = false;
	for (unsigned i = 0; i<imgnames.size(); i++){
		auto length = img_dir.length();
		string	img_name1 = imgnames[i].substr(length + 1);
		string	img_name = imgnames[i];
		ofstream fid(save_txt, ios::app);
		fid << img_name1 << endl;
		fid.close();
		Mat img_uint8 = imread(img_name.c_str(), 1);
		if (img_uint8.empty()){
			cout << "Cannot get image " << img_name << endl;
			getchar();
			return -2;
		}
		cout << "Processing " << trim_file_name(img_name, 0) << endl;

		Mat img2 = img_uint8.rowRange(0, 154);
		img2 = img2.colRange(930, 1279);
		resize(img2, img2, Size(int(350 * largetimes), int(155 * largetimes)));
		Mat	img = PM.prepareImg(img2);
		PM.detect(largetimes, labelname, save_txt, img, detectthresh, false, SHOWIMAGE, "dongcemen_yuanche.jpg");
		cout << "------------------------------------------------------------" << endl;
	}
	return 0;
}
int detect_PIC_resize(double largetimes, string img_dir, string save_txt, string modelname, int labelname, float detectthresh)
{
	/* I. Get images */
	string extensions[] = { ".jpg" };
	vector<string>	img_extensions(extensions, extensions + 1);
	vector<string>	imgnames = yuStdDirFiles(img_dir, img_extensions);

	/* II. Perform Part_Model based detection */
	FastDPM	PM(modelname);

	bool	flag = false;
	for (unsigned i = 0; i<imgnames.size(); i++){
		auto length = img_dir.length();
		string	img_name1 = imgnames[i].substr(length + 1);
		string	img_name = imgnames[i];
		ofstream fid(save_txt, ios::app);
		fid << img_name1 << endl;
		fid.close();
		Mat img_uint8 = imread(img_name.c_str(), 1);
		if (img_uint8.empty()){
			cout << "Cannot get image " << img_name << endl;
			getchar();
			return -2;
		}
		cout << "Processing " << trim_file_name(img_name, 0) << endl;

		Mat img2;
		resize(img_uint8, img2, Size(int(img_uint8.cols * largetimes), int(img_uint8.rows * largetimes)));
		Mat	img = PM.prepareImg(img2);
		PM.detect(largetimes, labelname, save_txt, img, detectthresh, false, SHOWIMAGE);
		cout << "------------------------------------------------------------" << endl;
		//if (PM.detections.empty())
		//	continue;
		//flag = true;
		//char key = waitKey(1);
		//if (key == 27)
		//	break;

	}
	return 0;
}

int	main()
{
	
	string base_folder_name_1 = "C:\\Users\\Danny\\Pictures\\testdata";
	string base_folder_name_2 = "C:\\Users\\Danny\\Pictures\\testdata";

	//cout << "Please input the single object folder: " << endl;
	//cin >> base_folder_name_1;

	//cout << "Please input the mutli object folder: " << endl;
	//cin >> base_folder_name_2;

	// single
	string hongsilounorth_foldername = "hongsilounorth_13_1920x1080_30_1";
	string hongsilouwest_foldername = "hongsilouwest_14_1920x1080_30_1";
	string weiminghubeieast_foldername = "weiminghubeieast_11_1920x1080_30_R1";
	string weiminghubeiwest_foldername = "weiminghubeiwest_12_1920x1080_30_R1";
	string hongsilounorth = base_folder_name_1 + "\\" + hongsilounorth_foldername;
	string hongsilouwest = base_folder_name_1 + "\\" + hongsilouwest_foldername;
	string weiminghubeieast = base_folder_name_1 + "\\" + weiminghubeieast_foldername;
	string weiminghubeiwest = base_folder_name_1 + "\\" + weiminghubeiwest_foldername;

	//double
	string dongcemen_foldername = "dongcemen_6_1280x720_30_R1";
	string dongnanmen_foldername = "dongnanmen_1_1280x720_30_R1";
	string weiminghudong_foldername = "weiminghudong_7_1280x720_30_R1";
	string yaoganqian_foldername = "yaoganqian_5_1280x720_30_R1";
	string dongcemen = base_folder_name_2 + "\\" + dongcemen_foldername;
	string dongnanmen = base_folder_name_2 + "\\" + dongnanmen_foldername;
	string weiminghudong = base_folder_name_2 + "\\" + weiminghudong_foldername;
	string yaoganqian = base_folder_name_2 + "\\" + yaoganqian_foldername;
	
	//================  hongsilounorth  ================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//Hansiyang Finish it at 20:08 2015/8/22.
	if (1)
	{
		//该数据库不需要去背景，不需要放大，检测即良好
		string dir_name = hongsilounorth_foldername;
		double sample_rate = 1;
		string	img_dir = hongsilounorth;  


		string  save_txt = "1_detection_" + dir_name + ".txt";
		string modelname = "inriaperson_final.txt";


		detect_PIC_resize(sample_rate, img_dir.c_str(), save_txt, modelname, 2, -0.4f);
		cout << "=======detect is over========" << endl;

		string txt_inrease = "1_increase_" + dir_name + ".txt";
		Result_Increasing(save_txt.c_str(), txt_inrease.c_str());
		cout << "=======increase is over========" << endl;

		string txt_delete = "1_delete_" + dir_name + ".txt";
		Result_Removing(txt_inrease.c_str(), txt_delete.c_str());
		cout << "=======delete is over========" << endl;


		string foldername = dir_name;
		string inputtxt = txt_delete;
		string outputxml = dir_name + ".xml";
		xml_make(foldername, txt_delete.c_str(), outputxml);
		cout << "=======xml is over========" << endl;
	}
	

	//================  hongsilouwest  ================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if (0)
	{
		string dir_name = hongsilouwest_foldername;
		double sample_rate = 1.5;
		string	img_dir = hongsilouwest;  

		Get_BG(img_dir, "hongsilouwest.bmp");
		cout << "=======Get_BG is over========" << endl;

		string  save_txt = "1_detection_" + dir_name + ".txt";
		string modelname = "inriaperson_final.txt";

		//sample_rate改为2试一下
		detectPic("hongsilouwest.bmp", sample_rate, img_dir.c_str(), save_txt, modelname, 2, -0.4f);
		cout << "=======detect is over========" << endl;

		string txt_inrease = "1_increase_" + dir_name + ".txt";
		Result_Increasing(save_txt.c_str(), txt_inrease.c_str());
		cout << "=======increate is over========" << endl;

		string txt_delete = "1_delete_" + dir_name + ".txt";
		Result_Removing(txt_inrease.c_str(), txt_delete.c_str());
		cout << "=======increate is over========" << endl;

		string foldername = dir_name;
		string outputxml = dir_name + ".xml";
		xml_make(foldername, txt_delete.c_str(), outputxml);
		cout << "=======xml is over========" << endl;
	}

	//================  weiminghubeieast  ================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if (0)
	{
		string dir_name = weiminghubeieast_foldername;
		double sample_rate = 1.5;
		string	img_dir = weiminghubeieast; 
		cout << img_dir << endl;

		Get_BG(img_dir, "weiminghubeieast.bmp");
		cout << "=======Get_BG is over========" << endl;

		string  save_txt = "1_detection_" + dir_name + ".txt";
		string modelname = "inriaperson_final.txt";



		detectPic("weiminghubeieast.bmp", sample_rate, img_dir.c_str(), save_txt, modelname, 2, -0.4f);
		cout << "=======detect is over========" << endl;

		string txt_inrease = "1_increase_" + dir_name + ".txt";
		Result_Increasing(save_txt.c_str(), txt_inrease.c_str());
		cout << "=======increate is over========" << endl;

		string txt_delete = "1_delete_" + dir_name + ".txt";
		Result_Removing(txt_inrease.c_str(), txt_delete.c_str());
		cout << "=======increate is over========" << endl;


		string foldername = dir_name;
		string outputxml = dir_name + ".xml";
		xml_make(foldername, txt_delete.c_str(), outputxml);
		cout << "=======xml is over========" << endl;
	}

	//================  weiminghubeiwest  ================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if (0)
	{
		string dir_name = weiminghubeiwest_foldername;
		double sample_rate = 1.5;
		string	img_dir = weiminghubeiwest;  
		cout << img_dir << endl;

		Get_BG(img_dir, "weiminghubeiwest.bmp");
		cout << "=======Get_BG is over========" << endl;

		string  save_txt = "1_detection_" + dir_name + ".txt";
		string modelname = "inriaperson_final.txt";


		if (0)
			detect_PIC_resize(sample_rate, img_dir.c_str(), save_txt, modelname, 2, -0.4f);
		else
			detectPic("weiminghubeiwest.bmp", sample_rate, img_dir.c_str(), save_txt, modelname, 2, -0.4f);
		cout << "=======detect is over========" << endl;

		string txt_inrease = "1_increase_" + dir_name + ".txt";
		Result_Increasing(save_txt.c_str(), txt_inrease.c_str());
		cout << "=======increate is over========" << endl;

		string txt_delete = "1_delete_" + dir_name + ".txt";
		Result_Removing(txt_inrease.c_str(), txt_delete.c_str());
		cout << "=======increate is over========" << endl;


		string foldername = dir_name;
		string outputxml = dir_name + ".xml";
		xml_make(foldername, txt_delete.c_str(), outputxml);
		cout << "=======xml is over========" << endl;
	}

	//================  dongcemen  ================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if (0)
	{
		string dir_name = dongcemen_foldername;
		double sample_rate = 2.0;
		string	img_dir = dongcemen;  
		cout << img_dir << endl;

		string  save_txt = "1_detection_" + dir_name + ".txt";
		string modelname = "inriaperson_final.txt";

		detect_PIC_resize(sample_rate, img_dir.c_str(), save_txt, modelname, 2, -0.4f);

		modelname = "car_final_dongcemen_1.txt";
		detect_PIC_resize(1, img_dir.c_str(), save_txt, modelname, 1, -0.8f);
		modelname = "car_final_2010.txt";
		detect_dongcemen(0.8, img_dir.c_str(), save_txt, modelname, 1, -0.7f);

		cout << "=======detect is over========" << endl;

		string txt_inrease = "1_increase_" + dir_name + ".txt";
		Result_Increasing(save_txt.c_str(), txt_inrease.c_str());
		cout << "=======increate is over========" << endl;

		string txt_delete = "1_delete_" + dir_name + ".txt";
		Result_Removing(txt_inrease.c_str(), txt_delete.c_str());
		cout << "=======delete is over========" << endl;

		string foldername = dir_name;
		string outputxml = dir_name + ".xml";
		xml_make(foldername, txt_delete.c_str(), outputxml);
		cout << "=======xml is over========" << endl;
	}

	//================  dongnanmen  ================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//TODO
	if (0)
	{
		string dir_name = dongnanmen_foldername;
		double sample_rate = 2;
		string	img_dir = dongnanmen;  
		cout << img_dir << endl;

		string  save_txt = "1_detection_" + dir_name + ".txt";
		string modelname = "inriaperson_final.txt";

			//PAY ATTENTION::::::MAYBE 1.8 is still big.
		detect_PIC_resize(1.8, img_dir.c_str(), save_txt, modelname, 2, -0.5f);

		modelname = "car_final_weiminghudong_1.txt";
		detect_PIC_resize(1, img_dir.c_str(), save_txt, modelname, 1, -0.9f);
		modelname = "model_car2010.txt";
		detect_PIC_resize(1, img_dir.c_str(), save_txt, modelname, 1, -0.5f);
		cout << "=======detect is over========" << endl;

		string txt_inrease = "1_increase_" + dir_name + ".txt";
		Result_Increasing(save_txt.c_str(), txt_inrease.c_str());
		cout << "=======increate is over========" << endl;

		string txt_delete = "1_delete_" + dir_name + ".txt";
		Result_Removing(txt_inrease.c_str(), txt_delete.c_str());
		cout << "=======delete is over========" << endl;


		string foldername = dir_name;
		string outputxml = dir_name + ".xml";
		xml_make(foldername, txt_delete.c_str(), outputxml);
		cout << "=======xml is over========" << endl;
	}

	//================  weiminghudong  ================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//Hansiyang Finish it at 15:14 2015/8/22. PAY ATTENTION：：左上角放大
	if (0)
	{
		string dir_name = weiminghudong_foldername;
		double sample_rate = 2;
		string	img_dir = weiminghudong;  
		cout << img_dir << endl;

		Get_BG(img_dir, "weiminghudong.bmp");
		cout << "=======Get_BG is over========" << endl;

		string  save_txt = "1_detection_" + dir_name + ".txt";
		string modelname = "inriaperson_final.txt";
		if (0)
			detect_PIC_resize(sample_rate, img_dir.c_str(), save_txt, modelname, 2, -0.12f);
		else
			detectPic("weiminghudong.bmp", sample_rate, img_dir.c_str(), save_txt, modelname, 2, -0.5f);
		IplImage* Iweiminghubg = cvLoadImage("weiminghudong.bmp", 1);
		Mat weiminghudong_BG = Mat(Iweiminghubg, 1);
		weiminghudong_BG = weiminghudong_BG.rowRange(0, 249);
		weiminghudong_BG = weiminghudong_BG.colRange(0, 499);
		Iweiminghubg = &IplImage(weiminghudong_BG);
		cvSaveImage("weiminghudong_part.bmp", Iweiminghubg);

		detect_weiminghudong("weiminghudong_part.bmp", 3, img_dir.c_str(), save_txt, modelname, 2, -1.4f);


		modelname = "car_final_weiminghudong_1.txt";
		detect_PIC_resize(1.5, img_dir.c_str(), save_txt, modelname, 1, -0.9f);

		cout << "=======detect is over========" << endl;

		string txt_inrease = "1_increase_" + dir_name + ".txt";
		Result_Increasing(save_txt.c_str(), txt_inrease.c_str());
		cout << "=======increate is over========" << endl;

		string txt_delete = "1_delete_" + dir_name + ".txt";
		Result_Removing(txt_inrease.c_str(), txt_delete.c_str());
		cout << "=======delete is over========" << endl;


		string foldername = dir_name;
		string outputxml = dir_name + ".xml";
		xml_make(foldername, txt_delete.c_str(), outputxml);
		cout << "=======xml is over========" << endl;
	}

	//================  yaoganqian  ================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//Hansiyang Finish it at 15:05 2015/8/22. 
	if (0)
	{
		string dir_name = yaoganqian_foldername;
		double sample_rate = 2.5;//PAY ATTENTION : 2.5 costs more time which we can't afford it.
		string	img_dir = yaoganqian;  
		cout << img_dir << endl;

		string  save_txt = "1_detection_" + dir_name + ".txt";
		string modelname = "inriaperson_final.txt";
		detect_PIC_resize(sample_rate, img_dir.c_str(), save_txt, modelname, 2, -0.4f);
		modelname = "car_final_weiminghudong_1.txt";
		detect_yaoganqian(2, img_dir.c_str(), save_txt, modelname, 1, -0.9f);
		modelname = "model_car2010.txt";
		detect_PIC_resize(1, img_dir.c_str(), save_txt, modelname, 1, -0.8f);

		cout << "=======detect is over========" << endl;

		string txt_inrease = "1_increase_" + dir_name + ".txt";
		Result_Increasing(save_txt.c_str(), txt_inrease.c_str());
		cout << "=======increate is over========" << endl;

		string txt_delete = "1_delete_" + dir_name + ".txt";
		Result_Removing(txt_inrease.c_str(), txt_delete.c_str());
		cout << "=======delete is over========" << endl;


		string foldername = dir_name;
		string outputxml = dir_name + ".xml";
		xml_make(foldername, txt_delete.c_str(), outputxml);
		cout << "=======xml is over========" << endl;
	}
	
	return	0;
}
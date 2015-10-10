#include "Get_BG.h"

using namespace cv;
using namespace std;

vector<string>  yuStdDirFiles(string DirName, vector<string> FileExtensions);
vector<string>  yuStdDirFilesWithoutPath(string DirName, vector<string> FileExtensions);

void readImageSequenceFiles(char* imgFilePath,vector <string> &imgNames)
{	
	imgNames.clear();

	char tmpDirSpec[MAX_PATH+1];
	sprintf (tmpDirSpec, "%s/*", imgFilePath);

	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFile(tmpDirSpec , &f);
	if(h != INVALID_HANDLE_VALUE)
	{
		FindNextFile(h, &f);	//read ..
		FindNextFile(h, &f);	//read .
		do
		{
			imgNames.push_back(f.cFileName);
		} while(FindNextFile(h, &f));

	}
	FindClose(h);	
}

int FindMax( int * his)
{
	int max,maxi;
	max = -1;
	maxi = 0;
	for (int i = 0; i < 256; i++)
	{
		if(his[i]>max)
		{
			max = his[i];
			maxi = i;
		}
	}
	return maxi;
}

int Get_BG(string folder, char * outputbmp)
{
	string extensions[] = { ".jpg" };
	vector<string>	img_extensions(extensions, extensions + 1);
	cout << "folder:" << folder << endl;
	vector<string>	imgNames = yuStdDirFiles(folder, img_extensions);

	int frameindex = 0;
	IplImage *image,*images;
	IplImage *newimage;
	image = cvLoadImage(imgNames[0].data());
	newimage = cvCreateImage(cvSize(image->width,image->height),image->depth,image->nChannels);
	int inten;
	int patch = image->width*3;

	int IMW = image->width, IMH = image->height;
	/*
	scale = batch part
	如果内存不够，可以改成更大的scale
	*/
	int scale;
	if (IMW >= 1500)
		scale = 8;
	else
		scale = 4;
	int *histo = (int *)malloc(sizeof(int)*IMH / scale*IMW * 3 * 256);
	int patch1 = IMW * 3 * 256;
	int patch2 = 3 * 256;
	int patch3 = 256;

	for(int k = 0;k < scale; k++)
	{
		printf("%d\n", k);
		frameindex = 0;
		memset(histo,0x00,sizeof(int)*IMW*IMH/scale*3*256);
		while(frameindex < imgNames.size())
		{
			// get frame  
			images = cvLoadImage(imgNames[frameindex].data());
			frameindex ++;
			// statistic
			for (int i = 0; i < image->height/scale; i++)
			{
				int ii = i + k * image->height/scale;
				for (int j = 0; j < image->width; j++)
				{		
					// statistics			
					inten = (images->imageData[ ii*patch+j*3+0 ] & 0x00ff);
					histo[i*patch1+j*patch2+0*patch3+inten] ++;
					inten = (images->imageData[ ii*patch+j*3+1 ] & 0x00ff);
					histo[i*patch1+j*patch2+1*patch3+inten] ++;
					inten = (images->imageData[ ii*patch+j*3+2 ] & 0x00ff);	
					histo[i*patch1+j*patch2+2*patch3+inten] ++;
				}
			}
			// release
			cvReleaseImage(&images);
			// echo
		}
		// get most one
		for (int i = 0; i < image->height/scale; i++)
		{		
			int ii = i + k * image->height/scale;
			for (int j = 0; j < image->width; j++)
			{
				newimage->imageData[ ii*patch+j*3+0 ] = FindMax(&histo[i*patch1+j*patch2+0*patch3]);
				newimage->imageData[ ii*patch+j*3+1 ] = FindMax(&histo[i*patch1+j*patch2+1*patch3]);
				newimage->imageData[ ii*patch+j*3+2 ] = FindMax(&histo[i*patch1+j*patch2+2*patch3]);
			}
		}
	}

	cvSaveImage(outputbmp,newimage);
	free(histo);
	histo = NULL;
	return 0;
}

int Remove_BG( const char * inputfolder, char * BGimage, const char * outputfolder, double sample)
{
	char imgFilePath[100];
	strcpy(imgFilePath, inputfolder);
	char imgBackground[100];
	strcpy(imgBackground, BGimage);
	char outputfile[100];
	char outputFilePath[100];
	strcpy(outputFilePath, outputfolder);

	string extensions[] = { ".jpg" };
	vector<string>	img_extensions(extensions, extensions + 1);
	vector <string> imgNames;
	vector <string> imgShortNames;
	imgNames = yuStdDirFiles(imgFilePath, img_extensions);
	imgShortNames = yuStdDirFilesWithoutPath(imgFilePath, img_extensions);


	//double sample = 2;
	int frameindex = 0;
	IplImage *image,*imagesorg,*images,*background,*backgroundorg;
	IplImage *newimage;
	image = cvLoadImage(imgNames[0].data());
	backgroundorg = cvLoadImage(imgBackground);
	background = cvCreateImage(cvSize(image->width*sample,image->height*sample),image->depth,image->nChannels);
	// 插值
	cvResize(backgroundorg, background, CV_INTER_CUBIC);
	newimage = cvCreateImage(cvSize(image->width*sample,image->height*sample),image->depth,image->nChannels);
	images = cvCreateImage(cvSize(image->width*sample,image->height*sample),image->depth,image->nChannels);
	int r1,g1,b1,r2,g2,b2;
	int patch = images->width*3;
	
	frameindex = 0;
	while(frameindex < imgNames.size())
	{
		// get frame  
		imagesorg = cvLoadImage(imgNames[frameindex].data());
		// 插值
		cvResize(imagesorg, images, CV_INTER_CUBIC);
		
		 
		for(int i = 0;i < images->height; i++)
		{
			for (int j = 0; j < images->width; j++)
			{
				// statistics
				b1 = (images->imageData[ i*patch+j*3+0 ] & 0x00ff);
				g1 = (images->imageData[ i*patch+j*3+1 ] & 0x00ff);
				r1 = (images->imageData[ i*patch+j*3+2 ] & 0x00ff);
				b2 = (background->imageData[ i*patch+j*3+0 ] & 0x00ff);
				g2 = (background->imageData[ i*patch+j*3+1 ] & 0x00ff);
				r2 = (background->imageData[ i*patch+j*3+2 ] & 0x00ff);
				int dis = (b1-b2)*(b1-b2)+(g1-g2)*(g1-g2)+(r1-r2)*(r1-r2);
				if( dis > BG_THR )
				{
					newimage->imageData[ i*patch+j*3+0 ] = 255;
					newimage->imageData[ i*patch+j*3+1 ] = 255;
					newimage->imageData[ i*patch+j*3+2 ] = 255;
				}
				else
				{
					newimage->imageData[ i*patch+j*3+0 ] = 0;
					newimage->imageData[ i*patch+j*3+1 ] = 0;
					newimage->imageData[ i*patch+j*3+2 ] = 0;
				}
			}
		}

		cvSmooth(newimage,newimage,CV_MEDIAN,9);

		for(int i = 0;i < images->height; i++)
		{
			for (int j = 0; j < images->width; j++)
			{
				// statistics
				b1 = (images->imageData[ i*patch+j*3+0 ] & 0x00ff);
				g1 = (images->imageData[ i*patch+j*3+1 ] & 0x00ff);
				r1 = (images->imageData[ i*patch+j*3+2 ] & 0x00ff);
				b2 = (newimage->imageData[ i*patch+j*3+0 ] & 0x00ff);
				g2 = (newimage->imageData[ i*patch+j*3+1 ] & 0x00ff);
				r2 = (newimage->imageData[ i*patch+j*3+2 ] & 0x00ff);
				if( b2 == 255 )
				{
					newimage->imageData[ i*patch+j*3+0 ] = b1;
					newimage->imageData[ i*patch+j*3+1 ] = g1;
					newimage->imageData[ i*patch+j*3+2 ] = r1;
				}
				else
				{
					newimage->imageData[ i*patch+j*3+0 ] = 0;
					newimage->imageData[ i*patch+j*3+1 ] = 0;
					newimage->imageData[ i*patch+j*3+2 ] = 0;
				}
			}
		}

		// save
		const char* outputfilename = imgShortNames[frameindex].c_str();
		sprintf(outputfile, "%s/%s", outputFilePath, outputfilename);
		cvSaveImage(outputfile, newimage);
		frameindex ++;

		// release
		cvReleaseImage(&imagesorg);

	}
	cvReleaseImage(&image);
	cvReleaseImage(&images);
	cvReleaseImage(&background);
	cvReleaseImage(&backgroundorg);
	cvReleaseImage(&newimage);
	return 0;
}

Mat Remove_BG_OnePic(const char * imgNames, IplImage * backgroundorg, double sample)
{
	//double sample = 2;
	IplImage *image, *imagesorg, *images, *background;// 0 1 0 0
	IplImage *newimage;//0
	image = cvLoadImage(imgNames);
	background = cvCreateImage(cvSize(int(image->width*sample), int(image->height*sample)), image->depth, image->nChannels);
	// 插值
	cvResize(backgroundorg, background, CV_INTER_CUBIC);
	newimage = cvCreateImage(cvSize(image->width*sample, image->height*sample), image->depth, image->nChannels);
	images = cvCreateImage(cvSize(int(image->width*sample), int(image->height*sample)), image->depth, image->nChannels);
	cvReleaseImage(&image);
	int r1, g1, b1, r2, g2, b2;
	int patch = images->width * 3;

	// get frame  
	imagesorg = cvLoadImage(imgNames);
	// 插值
	cvResize(imagesorg, images, CV_INTER_CUBIC);


	for (int i = 0; i < images->height; i++)
	{
		for (int j = 0; j < images->width; j++)
		{
			// statistics
			b1 = (images->imageData[i*patch + j * 3 + 0] & 0x00ff);
			g1 = (images->imageData[i*patch + j * 3 + 1] & 0x00ff);
			r1 = (images->imageData[i*patch + j * 3 + 2] & 0x00ff);
			b2 = (background->imageData[i*patch + j * 3 + 0] & 0x00ff);
			g2 = (background->imageData[i*patch + j * 3 + 1] & 0x00ff);
			r2 = (background->imageData[i*patch + j * 3 + 2] & 0x00ff);
			int dis = (b1 - b2)*(b1 - b2) + (g1 - g2)*(g1 - g2) + (r1 - r2)*(r1 - r2);
			if (dis > BG_THR)
			{
				newimage->imageData[i*patch + j * 3 + 0] = 255;
				newimage->imageData[i*patch + j * 3 + 1] = 255;
				newimage->imageData[i*patch + j * 3 + 2] = 255;
			}
			else
			{
				newimage->imageData[i*patch + j * 3 + 0] = 0;
				newimage->imageData[i*patch + j * 3 + 1] = 0;
				newimage->imageData[i*patch + j * 3 + 2] = 0;
			}
		}
	}
	cvSmooth(newimage, newimage, CV_MEDIAN, 9);

	for (int i = 0; i < images->height; i++)
	{
		for (int j = 0; j < images->width; j++)
		{
			// statistics
			b1 = (images->imageData[i*patch + j * 3 + 0] & 0x00ff);
			g1 = (images->imageData[i*patch + j * 3 + 1] & 0x00ff);
			r1 = (images->imageData[i*patch + j * 3 + 2] & 0x00ff);
			b2 = (newimage->imageData[i*patch + j * 3 + 0] & 0x00ff);
			g2 = (newimage->imageData[i*patch + j * 3 + 1] & 0x00ff);
			r2 = (newimage->imageData[i*patch + j * 3 + 2] & 0x00ff);
			if (b2 == 255)
			{
				newimage->imageData[i*patch + j * 3 + 0] = b1;
				newimage->imageData[i*patch + j * 3 + 1] = g1;
				newimage->imageData[i*patch + j * 3 + 2] = r1;
			}
			else
			{
				newimage->imageData[i*patch + j * 3 + 0] = 0;
				newimage->imageData[i*patch + j * 3 + 1] = 0;
				newimage->imageData[i*patch + j * 3 + 2] = 0;
			}
		}

		// release
		cvReleaseImage(&imagesorg);

	}
	cvReleaseImage(&images);
	Mat img_uint8(newimage, 1);
	cvReleaseImage(&newimage);
	cvReleaseImage(&background);
	return img_uint8;
}

Mat Remove_BG_OnePic_mat(Mat imgNames, IplImage * backgroundorg, double sample)
{
	//double sample = 2;
	IplImage *image, *imagesorg, *images, *background;// 0 1 0 0
	IplImage *newimage;//0
	image = &IplImage(imgNames);
	background = cvCreateImage(cvSize(int(image->width*sample), int(image->height*sample)), image->depth, image->nChannels);
	// 插值
	cvResize(backgroundorg, background, CV_INTER_CUBIC);
	newimage = cvCreateImage(cvSize(image->width*sample, image->height*sample), image->depth, image->nChannels);
	images = cvCreateImage(cvSize(image->width*sample, image->height*sample), image->depth, image->nChannels);
	
	int r1, g1, b1, r2, g2, b2;
	int patch = images->width * 3;

	// get frame  
	imagesorg = &IplImage(imgNames);
	// 插值
	cvResize(imagesorg, images, CV_INTER_CUBIC);


	for (int i = 0; i < images->height; i++)
	{
		for (int j = 0; j < images->width; j++)
		{
			// statistics
			b1 = (images->imageData[i*patch + j * 3 + 0] & 0x00ff);
			g1 = (images->imageData[i*patch + j * 3 + 1] & 0x00ff);
			r1 = (images->imageData[i*patch + j * 3 + 2] & 0x00ff);
			b2 = (background->imageData[i*patch + j * 3 + 0] & 0x00ff);
			g2 = (background->imageData[i*patch + j * 3 + 1] & 0x00ff);
			r2 = (background->imageData[i*patch + j * 3 + 2] & 0x00ff);
			int dis = (b1 - b2)*(b1 - b2) + (g1 - g2)*(g1 - g2) + (r1 - r2)*(r1 - r2);
			if (dis > BG_THR)
			{
				newimage->imageData[i*patch + j * 3 + 0] = 255;
				newimage->imageData[i*patch + j * 3 + 1] = 255;
				newimage->imageData[i*patch + j * 3 + 2] = 255;
			}
			else
			{
				newimage->imageData[i*patch + j * 3 + 0] = 0;
				newimage->imageData[i*patch + j * 3 + 1] = 0;
				newimage->imageData[i*patch + j * 3 + 2] = 0;
			}
		}
	}
	cvSmooth(newimage, newimage, CV_MEDIAN, 9);

	for (int i = 0; i < images->height; i++)
	{
		for (int j = 0; j < images->width; j++)
		{
			// statistics
			b1 = (images->imageData[i*patch + j * 3 + 0] & 0x00ff);
			g1 = (images->imageData[i*patch + j * 3 + 1] & 0x00ff);
			r1 = (images->imageData[i*patch + j * 3 + 2] & 0x00ff);
			b2 = (newimage->imageData[i*patch + j * 3 + 0] & 0x00ff);
			g2 = (newimage->imageData[i*patch + j * 3 + 1] & 0x00ff);
			r2 = (newimage->imageData[i*patch + j * 3 + 2] & 0x00ff);
			if (b2 == 255)
			{
				newimage->imageData[i*patch + j * 3 + 0] = b1;
				newimage->imageData[i*patch + j * 3 + 1] = g1;
				newimage->imageData[i*patch + j * 3 + 2] = r1;
			}
			else
			{
				newimage->imageData[i*patch + j * 3 + 0] = 0;
				newimage->imageData[i*patch + j * 3 + 1] = 0;
				newimage->imageData[i*patch + j * 3 + 2] = 0;
			}
		}

		// release
		
	}
	cvReleaseImage(&images);
	Mat img_uint8(newimage, 1);
	cvReleaseImage(&newimage);
	cvReleaseImage(&background);
	return img_uint8;
}

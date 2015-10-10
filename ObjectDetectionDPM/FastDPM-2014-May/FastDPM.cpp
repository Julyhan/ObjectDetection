#include "FastDPM.h"

FastDPM::FastDPM( string model_file )
{
	// 1.
	load_model( model_file, model );
	analyze_model( model );
}

Mat FastDPM::prepareImg( Mat &img_uint8 )
// Make a uint8 type image into a float type mat with 3 channels
{
	if( img_uint8.depth()!=CV_8U || (img_uint8.channels()!=1 && img_uint8.channels()!=3) ){
		printf("Function prepareImg() only takes as input an image of 1 or 3 channels of uint8 type!");
		throw	runtime_error("");
	}
	if( img_uint8.channels()==1 )
		cvtColor( img_uint8, img_color, CV_GRAY2RGB );
	else if( img_uint8.channels()==3 )
		img_color = img_uint8;
	Mat	img;
	img_color.convertTo( img, CV_32FC3 );
	return	img;
}

void FastDPM::detect(double largetimes, int labelname, string save_txt, Mat &img, float score_thresh /* = -FLOAT_INF */, bool show_hints /* = true */, bool show_img /* = true */, string save_img /* = */)
{
	string dirSaveTxt = save_txt;
	hints = show_hints;
	detections.clear();
	prag_start = yuGetCurrentTime('M');
	// 1. Feature pyramid <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	
	if( hints ){ printf("Calculating feature pyramid ...\t"); start_clock = prag_start; }
	/* method 1. */
	featpyramid( img, model, pyra );
	if( hints ){	end_clock = yuGetCurrentTime('M');	printf("_featpyramid takes %gs\n",(end_clock-start_clock)/1000.f);	}

	update_ruleData( model, pyra.num_levels );

	// 2. Filter responses & Deformation rules & Structural rules<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	if( hints ){	printf("Applying rules ...\t"); start_clock = end_clock; }
	apply_rules3( model, pyra );
	if( hints ){	end_clock = yuGetCurrentTime('M');	printf("_apply_rules takes %gs\n",(end_clock-start_clock)/1000.f); }

	// 3. Parse detections <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	if( hints ){	printf("Parsing detections ...\t"); start_clock = end_clock; }
	detections = parse_detections( model, pyra, score_thresh );
	if( detections.empty() ) 
	{ 
		printf("NO DETECTIONS!\n"); 
		ofstream fid(dirSaveTxt, ios::app);
		fid << 0 << endl;
		fid.close();

		return; 
	}
	if( hints ){	end_clock = yuGetCurrentTime('M');	printf("_parse_detections takes %gs\n",(end_clock-start_clock)/1000.f); }

	// Save results <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	ofstream fid(dirSaveTxt, ios::app);
	fid << detections.size() << endl;
	for (unsigned i = 0; i < detections.size(); i++){
		float		x1 = detections[i][0], y1 = detections[i][1], x2 = detections[i][2], y2 = detections[i][3];
		float		level = detections[i][4];
		float		component = detections[i][5];
		float		score = detections[i][6];
		if (!save_img.empty())
		{
			if (save_img == "yaoganqian_yuanche.jpg")
			{
				//cout << x1 / largetimes + 768 << '\t' << y1 / largetimes << '\t' << x2 / largetimes + 768 << '\t' << y2 / largetimes << '\t' << labelname << endl;
				if (y1 / largetimes < 150)
				{
					fid << x1 / largetimes + 768 << '\t' << y1 / largetimes << '\t' << x2 / largetimes + 768 << '\t' << y2 / largetimes << '\t' << labelname << endl;
				}
				else
				{
					fid << 0 << '\t' << 0 << '\t' << 0 << '\t' << 0 << '\t' << 404 << endl;
				}
			}
			else if (save_img == "dongcemen_yuanche.jpg")
			{
				if (1)//y1 /largetimes < 76.8
				{
					fid << x1 / largetimes + 930 << '\t' << y1 / largetimes << '\t' << x2 / largetimes + 930 << '\t' << y2 / largetimes << '\t' << labelname << endl;
				}
				else
				{
					fid << 0 << '\t' << 0 << '\t' << 0 << '\t' << 0 << '\t' << 404 << endl;
				}
			}
			else if (save_img == "weiminghudong_yuanche.jpg")
			{
				//cout << x1 / largetimes + 1030 << '\t' << y1 / largetimes << '\t' << x2 / largetimes + 1030 << '\t' << y2 / largetimes << '\t' << labelname << endl;
				if (1)//y1 /largetimes < 76.8
				{
					fid << x1 / largetimes << '\t' << y1 / largetimes << '\t' << x2 / largetimes << '\t' << y2 / largetimes << '\t' << labelname << endl;
				}
				else
				{
					fid << 0 << '\t' << 0 << '\t' << 0 << '\t' << 0 << '\t' << 404 << endl;
				}
			}
			else if (save_img == "weiminghudong_part.bmp")
			{
				{
					
					if (y2 / largetimes < 240 && level < 24)
					{
						cout << score << '\t' << x1 / largetimes << '\t' << y1 / largetimes << '\t' << x2 / largetimes << '\t' << y2 / largetimes << '\t' << labelname << endl;
						fid << x1 / largetimes << '\t' << y1 / largetimes << '\t' << x2 / largetimes << '\t' << y2 / largetimes << '\t' << labelname << endl;
					}
					else
					{
						fid << 0 << '\t' << 0 << '\t' << 0 << '\t' << 0 << '\t' << 404 << endl;
					}
				}
			}
		}
		else
		{
			fid << x1 / largetimes << '\t' << y1 / largetimes << '\t' << x2 / largetimes << '\t' << y2 / largetimes << '\t' << labelname << endl;
		}
		
	}
	fid.close();

	// 4. Show results <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	float Elapsed_Time = 0;
	if( hints ) { prag_end = end_clock; Elapsed_Time = (prag_end-prag_start)/1000.f; }
	if( show_img){
		draw_img( img_color, detections, Elapsed_Time );
		Mat img2;
		resize(img_color, img2, Size(960, 540), 0, 0, CV_INTER_LINEAR);
		imshow( "FAST_DPM", img2 );
	}
	
}
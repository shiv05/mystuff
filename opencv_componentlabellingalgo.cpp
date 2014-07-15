#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>

using namespace cv;

Mat foreground,cur_frame;
Ptr<BackgroundSubtractor> BSObject;
int index[] = {0, 1, 1, 1, 1, 0, 1, -1, 0, -1, -1, -1, -1, 0, -1, 1};

void processImages(char* first_file_name,char* pre_num);
void processVideo(char* video_filename);

int main(int argc,char *args[])
{
	printf("started\n");
	if(argc!=3)
	{
		std::cout<<"Argument List is faulty\n";
		char temp;
		std::cin>>temp;
		return -1;
	}

	namedWindow("Input");
	namedWindow("Output");

	BSObject = new BackgroundSubtractorMOG2(50,128.0,true);
	//processImages(args[1],args[2]);
	processVideo("MVI_2083_low.avi");
	return 0;
}

int Trace(Mat foreground,int label[180][320],int i,int j,int label_count,int start)
{
	//printf("%d %d %d start:%d\n",label_count,i,j,start);
	//printf("51:74--%d\n",foreground.at<uchar>(51,74));
	/*if(i==51)
	{
		int temp;
		std::cin>>temp;
	}*/
	for(int it=0;it<8;it++)
	{
		// if pixel at position n+i is white, return n+i
		int I = i+index[((it+start)%8)*2];
		int J = j+index[((it+start)%8)*2+1];
		//std::cout<<it<<" "<<I<<" "<<J<<'\n';
		if(I<0||J<0||I>=foreground.rows||J>=foreground.cols)
		{
			//printf("%d %d\n",I,J);
			continue;
		}
		if(foreground.at<uchar>(I,J)>0)
		{
			//std::cout<<"1\n";
			label[I][J]=label_count;
			//printf("returning next:%d\n",(it+start)%8);
			return (it+start)%8;
		}
		else
		{
			//std::cout<<"2\n";
			if(label[I][J]==0)
				label[I][J]=-1;
		}
	}
	return 8;
}


std::vector<Point> TraceContour(Mat foreground,int label[180][320],int i,int j,int label_count,bool isExternal)
{
	int next = 8;
	int currentx,currenty;
	std::vector<Point> ret;
	Point current;
	if(isExternal)
		next  = Trace(foreground,label,i,j,label_count,7);
	else
		next = Trace(foreground,label,i,j,label_count,3);
	if(next==8)//isolated point
	{
		foreground.at<uchar>(i,j)=0;
		label[i][j]=-1;
		return ret;
	}
	ret.push_back(Point(i,j));
	Point P = Point(i+index[next*2],j+index[next*2+1]);
	ret.push_back(P);
	next = Trace(foreground,label,P.x,P.y,label_count,(next+6)%8);
	if(i==ret[ret.size()-1].x+index[(next*2)%16] && j==ret[ret.size()-1].y+index[(next*2+1)%16])
		return ret;
	//if(next==8)
	//return ret;
	while(true)
	{
		currentx = ret[ret.size()-1].x+index[(next*2)%16];
		currenty = ret[ret.size()-1].y+index[(next*2+1)%16];
		next = Trace(foreground,label,currentx,currenty,label_count,(next+6)%8);


		if(currentx==i && currenty==j)
			return ret;
		else
			ret.push_back(Point(currentx,currenty));
	}
}


void processVideo(char *video_filename)
{
	VideoCapture capture(video_filename);
	if(!capture.isOpened())
	{
		std::cout<<"Video file not opened\n";
		return;
	}
	capture.read(cur_frame);
	BSObject->operator()(cur_frame,foreground);
	while(true)
	{
			if(!capture.read(cur_frame))
			{
				std::cout<<"video stream over....cant read next frame\n";
				char temp;
				scanf("%c",&temp);
				return;
			}
			BSObject->operator()(cur_frame,foreground);
			imshow("pre_foreground",foreground);

			int label_count = 0;
			int label[180][320]={0};
			std::vector<std::vector<Point>> blobs;
			for(int j=0;j<foreground.cols;j++)
			{
				foreground.at<uchar>(0,j)=0;
				foreground.at<uchar>(foreground.rows-1,j)=0;
			}
			for(int i=0;i<foreground.rows;i++)
			{
				foreground.at<uchar>(i,0)=0;
				foreground.at<uchar>(i,foreground.cols-1)=0;
			}
			//imshow("test",foreground);
			//waitKey(0);
			//std::cout<<"here\n";
			for(int i=1;i<foreground.rows-1;i++)
			{
				for(int j=1;j<foreground.cols-1;j++)
				{
					if(label[i][j]==0)
					{
						if(foreground.at<uchar>(i,j)==0)
							label[i][j]=-1;
						else
						{
							if(foreground.at<uchar>(i-1,j)==0)
							{
								label_count++;
								label[i][j]=label_count;
								blobs.push_back(TraceContour(foreground,label,i,j,label_count,true));
							}
							if(foreground.at<uchar>(i+1,j)==0)
							{
								/*int temp_label;
								if(label[i][j]!=0)
									temp_label = label[i][j];
								else
									temp_label = label[i][j-1];
								if(label[i][j]!=0)
									label[i][j]=temp_label;*/
								if(label[i][j]==0)
									label[i][j] = label[i-1][j];
								if(label[i][j]!=-1)//if isolated point....color was changed to black and was labelled -1
								{
									std::vector<Point> temp_point_vec = TraceContour(foreground,label,i,j,label_count,false);
									blobs[label[i][j]-1].reserve(blobs[label[i][j]-1].size()+temp_point_vec.size());
									blobs[label[i][j]-1].insert(blobs[label[i][j]-1].end(),temp_point_vec.begin(),temp_point_vec.end());
								}
							}
							if(label[i][j]==0)
							{
								if(label[i][j-1]>0)
								{
									label[i][j]=label[i][j-1];
									blobs[label[i][j]-1].push_back(Point(i,j));
								}
							}
						}
					}
				}
			}

			for(int i=0;i<label_count;i++)
			{
				if(blobs[i].size()<500)
				{
					for(int j=0;j<blobs[i].size();j++)
					{
						//printf("%d %d--\n",blobs[i][j].x,blobs[i][j].y);
						foreground.at<uchar>(blobs[i][j].x,blobs[i][j].y)=0;
						//printf("--\n");
					}
				}
			}

			std::stringstream ss;
			rectangle(cur_frame, cv::Point(10, 2), cv::Point(100,20),cv::Scalar(255,255,255), -1);
			ss << capture.get(CV_CAP_PROP_POS_FRAMES);
			string frameNumberString = ss.str();
			putText(cur_frame, frameNumberString.c_str(), cv::Point(15, 15),
            FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
			imshow("Input",cur_frame);
			waitKey(1);
			imshow("Output",foreground);
			waitKey(1);
		}
}

void processImages(char* first_file_name,char* pre_num)
{
	string pre_num_str(pre_num);
	int pre_length=pre_num_str.length();
	cur_frame = imread(first_file_name);
	if(!cur_frame.data)
	{
		std::cout<<"error in opening images\n";
		return;
	}
	string fn(first_file_name);
	while(true)
	{
		BSObject->operator()(cur_frame,foreground);

		

		size_t index1 = fn.find_last_of("/");
		if(index1==string::npos)
		{
			index1 = fn.find_last_of("\\");
		}
		size_t index2 = fn.find_last_of(".");
		string prefix = fn.substr(0,index1+1);
		string suffix = fn.substr(index2);
		string number = fn.substr(index1+1+pre_length,index2-index1-1);
		std::istringstream iss(number);
		int frameNumber = 0;
		iss >> frameNumber;
		rectangle(cur_frame,Point(10,2),cv::Point(100,20),Scalar(255,255,255),-1);
		putText(cur_frame,number.c_str(),Point(15,15),FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0));
		imshow("Input",cur_frame);
		waitKey(1);
		imshow("Output",foreground);
		waitKey(1);
		std::ostringstream oss;
		oss << frameNumber+1;
		string next;
		string temps = oss.str();
		for(unsigned int i=1;i<=5-temps.length();i++)
			next.push_back('0');
		next.append(temps);
		string nextFileName = prefix + pre_num + next + suffix;
		//std::cout<<nextFileName<<" "<<(frameNumber+1)<<std::endl;
		cur_frame = imread(nextFileName);
		if(!cur_frame.data)
		{
			std::cout<<"cant read "<<nextFileName.c_str()<<std::endl;
			char temp;
			std::cin>>temp;
			return;
		}
		fn.assign(nextFileName);
	}
}
#include "SimEnvironment.h"
#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/opencv.hpp"


using namespace std;

void SimEnvironment:: loadMap(string fileName){
    fstream myfile;
    myfile.open(fileName.c_str(),std::fstream::in);

    if (!myfile.is_open())
        cout<< "Can't open file!" <<endl;

    if (myfile.is_open())
    {
        myfile >> map_w >> map_h;
        map_matrix.resize(map_w);
        vis_matrix.resize(map_w);
        for(int i=0;i<map_w; i++)
        {
         for(int j=0;j<map_h; j++)
            {
                int data;
                myfile>> data;
                map_matrix[i].push_back(data);
                vis_matrix[i].push_back(0);
            }
        }
        myfile.close();
    }
}


void SimEnvironment:: printMap(){
       for(int i=0;i<map_w; i++)
        {
            for(int j=0;j<map_h; j++)
                cout << map_matrix[i][j];
            cout << endl;
        }
}


void SimEnvironment::loadMap_image(string map_file)
{
	cv::Mat image;
	image = cv::imread(map_file, CV_LOAD_IMAGE_GRAYSCALE);   // Read the file
	if(! image.data )    // Check for invalid input
	{
	  cout <<  "Could not open or find the map image" << std::endl ;
	}
	else
	{
		map_h = image.rows;
		map_w = image.cols;

		//Initialization of map matrix
		map_matrix.resize(image.rows);
		for(int i =0 ; i< image.rows ; i++)
		   map_matrix[i].resize(image.cols);

		//load image data in map_matrix and in the obstacles vector.
		for(int i=0 ; i<image.rows ; i++)
		{
		   for(int j=0 ; j<image.cols ; j++)
		   {
			   if((int)image.at<uchar>(i,j) < 230)
			   {
					map_matrix[i][j] = 1;
			   }
			   else
			   {
					map_matrix[i][j] = 0;
			   }
			}
	   }
   }
}


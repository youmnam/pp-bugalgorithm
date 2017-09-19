#ifndef PATHEX_H
#define PATHEX_H
#include "Position.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
using namespace std;

class PathExecuter{
public:
	PathExecuter(){}
	//virtual void executePath(vector<unint32> path,vector<intPair> coord) = 0;
};


class GraphicalPathExecuter : public PathExecuter{
private:
	int w = 480;
	int h = 640;

public:
	GraphicalPathExecuter(): PathExecuter(){ }
	GraphicalPathExecuter(int w, int h): PathExecuter(){this->w=w; this->h=h;}
	void drawLine( cv::Mat img, cv::Point start, cv::Point end , cv::Scalar sc);
	void drawFilledCircle( cv::Mat img, cv::Point center ,int rad, int r,int g, int b );
	void executePath(std::vector<std::vector <int> > map,vector<Position> path);
};


#endif

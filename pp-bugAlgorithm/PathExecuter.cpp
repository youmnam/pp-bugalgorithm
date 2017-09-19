#include "PathExecuter.h"

void GraphicalPathExecuter :: drawLine( cv::Mat img, cv::Point start, cv::Point end, cv::Scalar sc )
	{
	  int thickness = 2;
	  int lineType = 8;
	  cv::line( img,
	    start,
	    end,
	    sc,
	    thickness,
	    lineType );
	}

	void GraphicalPathExecuter::drawFilledCircle( cv::Mat img, cv::Point center ,int rad, int r,int g, int b )
	{
	  int thickness = -1;
	  int lineType = 8;

	  cv::circle( img,
	      center,
	      rad,
	      cv::Scalar( b, g, r ),
	      thickness,
	      lineType );
	}


void GraphicalPathExecuter::executePath(std::vector<std::vector <int> > map,vector<Position> path)
{

	cv::Mat space_image = cv::Mat::zeros(w,h, CV_8UC3 );
	char space_window[] = "RRT";

	//Draw Map
	for (int i =0; i<map.size(); i++)
	{
		for (int j =0; j<map[0].size(); j++){
			if (map[i][j] == 1)
				drawFilledCircle( space_image , cv::Point(j,i) , .1, 200, 0, 50);
		}
	}


	for (int i =0; i<path.size()-1; i++)
	{
		cv::Point p1(ceil(path[i].getX()) ,ceil(path[i].getY()));
		cv::Point p2(ceil(path[i+1].getX()),ceil(path[i+1].getY()));
		drawLine( space_image, p1, p2 , cv::Scalar(0,250,130));

	}

	cv::Point start(ceil(path[0].getX()) ,ceil(path[0].getY()));
	cv::Point end (ceil(path[path.size()-1].getX()),ceil(path[path.size()-1].getY()));
	drawFilledCircle( space_image , start, 2, 0, 0, 255);
	drawFilledCircle( space_image , end, 2, 0, 0, 255);



		 cv::imshow( space_window, space_image );
		 cv::waitKey( 0 );


}

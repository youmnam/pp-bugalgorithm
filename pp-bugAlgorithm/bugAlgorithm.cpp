#include <iostream>
#include <stack>
#include <vector>
#include <cstdlib>
#include "Position.h"
#include "SimEnvironment.h"
#include "PathExecuter.h"

using namespace std;

#define INF 999999999
#define PI 3.14159265
#define heading_down 90
#define heading_left 180
#define heading_up  270
#define heading_right  0


SimEnvironment simEnvio;

double getDistance(Position p1, Position p2)
{
   return sqrt(pow(p1.getX()-p2.getX(),2)+pow(p1.getY()-p2.getY(),2));
}

int calculateHeadingDirection(Position p1, Position p2)
{
	float heading=atan2(p1.getY()-p2.getY(),p1.getX()-p2.getX())*180/PI;
    heading= round(heading) - 180  ;
    if(heading <= -180)
    	heading +=360;
    if(heading>180)
    	heading -=360;

    return heading;
}

Position displacment(Position p,int heading)
{
	float dx=cos(heading*PI/180);
	float dy=sin(heading*PI/180);
	//cout << p.getX()+1*dx <<"  " << p.getY()+1*dy << endl;
	return Position(p.getX()+1*dx,p.getY()+1*dy);
}

bool isEqualPosition(Position p1, Position p2)
{
	return trunc(p1.getX())==trunc(p2.getX()) && trunc(p1.getY())==trunc(p2.getY());
}


//tactile sensor
bool scan(Position p, int heading )
{
	Position aheadPos = displacment(p,heading);
	int i= ceil(aheadPos.getY()) ;
	int j= ceil(aheadPos.getX()) ;
//	cout << "scan (" << ceil(aheadPos.getX()) << ","<< ceil(aheadPos.getY()) << " h=" <<heading  << " matrix "<< simEnvio.map_matrix[i][j] << endl;

	//cout << " i=" << i << "  j="<<j <<endl;
	if(i<0 || j<0) return false;
	if(i>=simEnvio.getW() || j>=simEnvio.getH()) return false;
	if(simEnvio.map_matrix[i][j] == 1) //obstacle
		return true;
	else
		return false;
}
vector<Position> trajectory;
stack<Position> qhit;

bool isMLine(Position p1, Position p2, Position p){
	float slope ;
	if(p1.getX() == p2.getX()) return trunc(p.getX()) == trunc(p1.getX());
	if(p1.getY() == p2.getY()) return trunc(p.getY()) == trunc(p1.getY());
	slope = (p1.getY()-p2.getY())/(p1.getX()-p2.getX());

	return trunc(p.getY()) == trunc((slope*(p.getX()-p1.getX()) + p1.getY()));
}

typedef enum  {LEFT=1,RIGHT=2,UP=3,DOWN=4} MovmentDirection;
int nextHeading(int theta)
{
	int nextTheta = theta - 90;
	if(nextTheta < 0 )
		nextTheta +=360;

	if( 0   < nextTheta && nextTheta <90) 	nextTheta=90;
	if( 270 < nextTheta && nextTheta <360) 	nextTheta=0;
	if( 90  < nextTheta && nextTheta <180) 	nextTheta=180;
	if( 180 < nextTheta && nextTheta <270) 	nextTheta=270;

	return nextTheta;

}

pair<int,int> getBoundaryDirection(Position p)
{
	pair<int,int> boundary(0,0);
	int i = ceil(p.getY());
	int j = ceil(p.getX());

	if(i-1 >0 && simEnvio.map_matrix[i-1][j] == 1)
	{
		boundary.first=-1;
		boundary.second=0;
	}
	else if(i+1 <simEnvio.getW() && simEnvio.map_matrix[i+1][j] == 1)
	{
		boundary.first=1;
		boundary.second=0;
	}
	else if(j+1 <simEnvio.getH() && simEnvio.map_matrix[i][j+1] == 1)
	{
		boundary.first=0;
		boundary.second=1;
	}
	else if(j-1 >0 && simEnvio.map_matrix[i][j-1] == 1)
	{
		boundary.first=0;
		boundary.second=-1;
	}
	else if(i-1>0 && j-1 >0 && simEnvio.map_matrix[i-1][j-1] == 1)
	{
		boundary.first=-1;
		boundary.second=-1;
	}
	else if(i-1>0 && j+1 <simEnvio.getH() && simEnvio.map_matrix[i-1][j+1] == 1)
	{
		boundary.first=-1;
		boundary.second=1;
	}
	else if(i+1 <simEnvio.getW( )&& j-1 >0 && simEnvio.map_matrix[i+1][j-1] == 1)
	{
		boundary.first=1;
		boundary.second=-1;
	}
	else if(i+1 <simEnvio.getW( )&& j+1 <simEnvio.getH() && simEnvio.map_matrix[i+1][j+1] == 1)
	{
		boundary.first=1;
		boundary.second=1;
	}
	return boundary;
}
bool isBoundary(Position p,pair<int,int> boundary)
{
	return simEnvio.map_matrix[ceil(p.getY())+boundary.first][ceil(p.getX())+boundary.second] == 1;
}

bool isAdjacent(pair<int,int> b1, pair<int,int> b2)
{

}
Position followBoundary( int heading,  Position start, Position goal)
{
	/*1. get Heading direction of following obstacle ; first point to follow*/
	int nextH=heading,prev=heading;
	Position nextP = qhit.top(),tempP;
	pair<int,int> boundary, newBoundary;

	do
	{
		nextH = nextHeading(prev);
		Position temp =  displacment(nextP,nextH);
		prev =nextH;

		if(getDistance(temp,goal) > getDistance(nextP,goal) ) continue;
	}while(scan(nextP,nextH));

	nextP = displacment(nextP,nextH);
	boundary= getBoundaryDirection(nextP);

	trajectory.push_back(Position(nextP.getX(),nextP.getY()));

	cout << "1. next Point:" << "(" << ceil(nextP.getX()) << ","<< ceil(nextP.getY()) <<") nextH:" << nextH  << endl;
	while(true)
	//for(int n=0; n<2; n++)
	{
		if(isMLine(start,goal,nextP)) break;
		if(isEqualPosition(goal,nextP)) break;
		/*2. loop until corner of obstacle reached*/
		bool isCorener=false;
		for(int k=0;k<2; k++){
			tempP = displacment(nextP,nextH);
			cout <<"\n2. next Point:" << "(" << ceil(tempP.getX()) << ","<< ceil(tempP.getY()) <<") nextH:" << nextH  << endl;
			cout << "boundary : X=" << boundary.first << " Y=" <<boundary.second <<endl;
			if(isMLine(start,goal,nextP))
						return nextP;
			if(!isBoundary(tempP,boundary))
			{
				isCorener = true;
				break;
			}
			nextP = tempP;
			//simEnvio.vis_matrix[nextP.getY()][nextP.getX()] =1;
			trajectory.push_back(Position(nextP.getX(),nextP.getY()));
		}

		/* 3. if corner is detected and we still didn't reach goal */
		if(isCorener == true)
		{

			cout << "3. corner Point:" << "(" << ceil(tempP.getX()) << ","<< ceil(tempP.getY()) <<") nextH:" << nextH  << endl;

			/* 3.1 get new heading direction that leave bug follow the boundary of obstacle
				   test the 4 directions of tempP which is the position that above/below corner */
			int theta [4]= {0,90,180,270};
			int thetai=-1;
			for(int i=0;i<4; i++)
			{
				newBoundary = getBoundaryDirection(displacment(tempP, theta[i]));
				if(newBoundary.first != 0 || newBoundary.second != 0 ){
					//if(isAdjacent(boundary,newBoundary))
					{
						thetai = i;
						break;
					}
				}
			}
			if(thetai !=-1)
			{
				boundary = newBoundary;
				nextH=theta[thetai];
			}
			cout << "4. corner Point:" << "(" << ceil(tempP.getX()) << ","<< ceil(tempP.getY()) <<") nextH:" << nextH  << endl;

			/* 3.2 mark corner visited */
			nextP = tempP;
			//simEnvio.vis_matrix[nextP.getY()][nextP.getX()] =1;
			trajectory.push_back(Position(nextP.getX(),nextP.getY()));

		}
	}
	return nextP;
}
int main()
{
	//simEnvio.loadMap("sim_map.txt");
	simEnvio.loadMap_image("building1.jpg");

	//simEnvio.printMap();
	Position start(20,20);

	Position goal(315,420);

//	cout << "is in Line " << isMLine(start,goal,Position(5,5)) << endl;;
	cout <<  (int)displacment(start,90).getX() <<"  " <<  (int)displacment(start,90).getY() <<endl;
	cout << "Start Position:" << "(" << ceil(start.getX()) << ","<< ceil(start.getY()) <<")" << endl;
	cout << "Goal  Position:" << "(" << ceil(goal.getX())  << ","<< ceil(goal.getY())  <<")" << endl;

	//cout << "next theta :" << nextHeading(280) <<endl;

	double heading= calculateHeadingDirection(start,goal) ;
	Position ql= start;
	while(true)
	{
		//cout << "(" << ceil(ql.getX()) << ","<< ceil(ql.getY()) << ") --> ";
		if(isEqualPosition(ql,goal))
		{
			cout << "Goal reached" <<endl;
			break;
		}
		if(ceil(ql.getX())> simEnvio.getW() || ceil(ql.getY())> simEnvio.getH())
		{
			cout << "failed to reach the goal" <<endl;
			break;
		}
		if(scan(ql,heading)) //obstacle detected in front of d
		{
			cout << "obstacle detected" <<  "(" << ceil(ql.getX()) << ","<< ceil(ql.getY()) << ")" <<endl;
			qhit.push(ql);
			//simEnvio.vis_matrix[ql.getY()][ql.getX()] = 1;

			ql= followBoundary(heading,start,goal);
			qhit.pop();
			if(ql.getX() ==-1 && ql.getY() == -1)
			{
				cout <<"ERROR!"<<endl;
				break;
			}

		}else{
			ql =  displacment(ql,heading);
			//simEnvio.vis_matrix[ql.getY()][ql.getX()] = 1;
			trajectory.push_back(ql);
		}
	}
	cout << "trajectory:" <<endl;
	for(int i =0 ; i<trajectory.size(); i++)
		cout  << "(" << ceil(trajectory[i].getX()) << ","<< ceil(trajectory[i].getY()) << ")" << endl;
	GraphicalPathExecuter executer(480,640);
	executer.executePath(simEnvio.map_matrix,trajectory);
}


/*Position followBoundary( int heading,  Position start, Position goal)
{

	Position hitp = qhit.top();
	cout << "Hit Point:" << "(" << ceil(hitp.getX()) << ","<< ceil(hitp.getY()) <<")" << endl;

	int temp_heading;
	MovmentDirection dir;
	MovmentDirection obsdir;
	if(!scan(hitp,heading_right))
	{
		temp_heading = heading_right;
		dir = RIGHT;
		cout << "RIGHT" << endl;
	}
	else if(!scan(hitp,heading_left))
	{
		temp_heading = heading_right;
		dir = LEFT;
		cout << "LEFT" << endl;
	}
	else if(!scan(hitp,heading_down))
	{
		temp_heading = heading_down;
		dir = DOWN;
		cout << "DOWN" << endl;
	}
	else if(!scan(hitp,heading_up))
	{
		temp_heading = heading_up;
		dir = UP;
		cout << "UP" << endl;
	}
	Position newp= displacment(hitp,temp_heading);
		cout << "FOLLOW new Point:" << "(" << ceil(newp.getX()) << ","<< ceil(newp.getY()) <<")" << endl;

	cout << "directions: "<< dir<< ","<< obsdir << endl;
	if(scan(newp,heading_right))
	{
		obsdir = RIGHT;
	}
	else if(scan(newp,heading_left))
	{
		obsdir = LEFT;
	}
	else if(scan(newp,heading_down))
	{
		obsdir = DOWN;
	}
	else if(scan(newp,heading_up))
	{
		obsdir = UP;
	}

	cout << "directions: "<< dir<< ","<< obsdir << endl;
	cout << "isMline:" << isMLine(start,goal,newp) <<" ep:" <<isEqualPosition(newp,hitp) << "dis htip:" << getDistance(goal,hitp) << "dis  newp" << getDistance(goal,newp) << endl;
	//while(!isMLine(start,goal,newp) && !isEqualPosition(newp,hitp) && getDistance(goal,hitp)> getDistance(goal,newp)){
	for(int i=0; i<10; i++){
	if(dir == RIGHT && obsdir ==UP ){
				trajectory.push_back(newp);
				if(!scan(newp,heading_up))
				{
					dir=UP;
					temp_heading = heading_up;
					obsdir = LEFT;
				}else{
					newp= displacment(newp,temp_heading);

				}
			}
			if(dir == RIGHT && obsdir ==DOWN ){
				trajectory.push_back(newp);
				if(!scan(newp,heading_down))
				{
					dir=DOWN;
					temp_heading = heading_down;
					obsdir = LEFT;
				}else{
					newp= displacment(newp,temp_heading);

				}
			}
			if(dir == LEFT && obsdir ==UP ){
				trajectory.push_back(newp);
				if(!scan(newp,heading_up))
				{
					dir=UP;
					temp_heading = heading_up;
					obsdir = RIGHT;
				}else{
					newp= displacment(newp,temp_heading);

				}
			}
			if(dir == LEFT && obsdir ==DOWN ){
				trajectory.push_back(newp);
				Position temp= displacment(newp,temp_heading);
				if(!scan(newp,heading_down))
				{
					dir=DOWN;
					temp_heading = heading_down;
					obsdir = RIGHT;
				}else{
					newp= displacment(newp,temp_heading);

				}
			}
			if(dir == UP && obsdir ==LEFT ){
				trajectory.push_back(newp);
				if(!scan(newp,heading_left))
				{
					dir= RIGHT;
					temp_heading = heading_right;
					obsdir = DOWN;
				}else{
					newp= displacment(newp,temp_heading);

				}
			}
			if(dir == UP && obsdir ==RIGHT ){
				trajectory.push_back(newp);
				if(!scan(newp,heading_right))
				{
					dir=LEFT;
					temp_heading = heading_left;
					obsdir = DOWN;
				}else{
					newp= displacment(newp,temp_heading);

				}

			}
			if(dir == DOWN && obsdir ==LEFT ){
				trajectory.push_back(newp);
				if(!scan(newp,heading_left))
				{
					dir= RIGHT;
					temp_heading = heading_right;
					obsdir = UP;
				}else{
					newp= displacment(newp,temp_heading);
				}
			}
			if(dir == DOWN && obsdir ==RIGHT ){
				trajectory.push_back(newp);
				if(!scan(newp,heading_right))
				{
					dir=LEFT;
					temp_heading = heading_down;
					obsdir = UP;
				}else{
					newp= displacment(newp,temp_heading);
				}
			}

	}
	cout << "leave Point:" << "(" << ceil(newp.getX()) << ","<< ceil(newp.getY()) <<")" << endl;

	qhit.pop();
	return newp;
}
*/

/*
Position getLeavePoint(Position p, int heading){
	Position qleave = p;e
	while(scan(qleave,heading)){
		qleave = displacment(qleave,heading);
	}
	return qleave;
}*/

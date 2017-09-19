#ifndef TYPES_H
#define TYPES_H
#include <vector>
#include <string>

typedef bool bit;
typedef float sfloat;
typedef unsigned char unint8;
typedef unsigned int unint32;
typedef signed int sint32;
typedef std::pair<sint32,sint32> intPair;
typedef std::vector< std::vector<sfloat> > matrix; // matrix holds float
typedef std::vector< std::vector<bit> > bmatrix; //matrix holds 0 or 1

enum boundary{LU=1,LB=2,RU=3,RB=4,DS=5,US=6,LS=7,RS=8,_NAN=9};
struct corner{
	int i;
	int x;
	int y;
	char type[2]; //LU, LB, RU, RB
	corner(int ii,int xx, int yy, char t[2]){
		i=ii;
		x=xx;
		y=yy;
		type[0]=t[0];
		type[1]=t[1];
	}

};

struct edge{
	int v1;
	int v2;
	sfloat cost;
	edge(int vv1, int vv2, sfloat c){
		v1 = vv1;
		v2 = vv2;
		cost =c;
	}
};

struct StatePoint{
    int x;
    int y;
    StatePoint(){}
    StatePoint(int xx,int yy){
        x=xx;
        y=yy;
    }
};

struct RRTNode{
    StatePoint p;
    int prv;
    RRTNode(StatePoint pp, int pv){
       p.x = pp.x;
       p.y = pp.y;
       prv= pv;
    }
};



#endif

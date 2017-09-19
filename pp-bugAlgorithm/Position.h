
#include <cmath>

#ifndef POSITION_H
#define POSITION_H

class Position
{
	private:
         float x;
         float y;
    public:
        Position(){ x=0; y=0;};
		Position(float xx,float yy){
		            this->x = xx;
		            this->y = yy;
		        }

        float getX() { return x; }
        void setX( float val) { x = val; }

        float getY() { return y; }
        void setY( float val) { y = val; }



};

#endif // POSITION_H

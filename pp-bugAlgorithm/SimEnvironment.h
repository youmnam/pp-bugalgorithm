#include<vector>
#include<string>
#ifndef SIMENV_H
#define SIMENV_H

class SimEnvironment{

public:
    void loadMap(std::string fileName);
    void printMap();
    SimEnvironment(){};
    SimEnvironment(int w,int h){
        map_w = w;
        map_h = h;
    }
    SimEnvironment(std::string fileName){
       loadMap(fileName);
    }
    void loadMap_image(std::string map_file);
    int getW(){ return map_w;}
    int getH(){ return map_h;}
    std::vector<std::vector <int> > map_matrix;
    std::vector<std::vector <int> > vis_matrix;
private:
    int map_w;
    int map_h;

};
#endif // SIMENV_H

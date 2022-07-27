//
// Created by Philip on 7/22/2022.
//

#ifndef VIDEO_COORD_HPP
#define VIDEO_COORD_HPP
#include <math.h>
//3d coordinate class
class Coord {
public:
     int x = 0;
     int y = 0;
     int z = 0;

    Coord(){}

    Coord(int in_x , int in_y,int in_z){
        x = in_x;
        y = in_y;
        z = in_z;
    }
    Coord(int scalar){
        x = scalar;
        y = scalar;
        z = scalar;
    }

    double distance(Coord b){
        return sqrt(pow(x - b.x,2) +  pow(y - b.y,2) + pow(z - b.z,2));
    }

    Coord operator+(const Coord b) {
        return Coord(x + b.x,y + b.y,z + b.z);
    }
    Coord operator-(const Coord b) {
        return Coord(x - b.x,y - b.y,z - b.z);
    }
    Coord operator*(const Coord b) {
        return Coord(x * b.x,y * b.y,z * b.z);
    }
    Coord operator/(const Coord b) {
        return Coord(x / b.x,y / b.y,z / b.z);
    }

    Coord roundDivide(double amount){
        return Coord(round((double)x / amount),round((double)y / amount),round((double)z / amount) );
    }


};

#endif VIDEO_COORD_HPP
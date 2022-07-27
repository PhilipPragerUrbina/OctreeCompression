//
// Created bg Philip on 7/22/2022.
//

#ifndef VIDEO_COLOR_HPP
#define VIDEO_COLOR_HPP


#include "opencv2/core/matx.hpp"
//color class
struct Color{

    //values
    double r = 0;
    double g = 0;
    double b = 0;

    Color(){

    }
    Color(cv::Vec3b color){
        r = color[0];
        g = color[1];
        b = color[2];
    }

    Color(double red,double green,double blue){
        r = red;
        b = blue;
        g = green;
    }

    //get opencv color
    cv::Vec3b getColor(){
        return cv::Vec3b(r,g,b);
    }

    double distance(Color other){
        return sqrt(pow(r - other.r,2) +  pow(g - other.g,2) + pow(b - other.b,2));
    }

    Color operator+(const Color other) {
        return Color(r + other.r,g + other.g,b + other.b);
    }
    Color operator-(const Color other) {
        return Color(r - other.r, g - other.g, b - other.b);
    }
    Color operator*(const Color other) {
        return Color(r * other.r, g * other.g, b * other.b);
    }
    Color operator/(const Color other) {
        return Color(r / other.r, g / other.g, b / other.b);
    }
};

#endif VIDEO_COLOR_HPP
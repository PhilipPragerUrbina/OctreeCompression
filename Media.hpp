//
// Created by Philip on 7/23/2022.
//

#ifndef VIDEO_MEDIA_HPP
#define VIDEO_MEDIA_HPP
#include "opencv2/opencv.hpp"
#include "Color.hpp"
#include "Coord.hpp"
#include "Octree.hpp"

#include <string>
using namespace cv;
class Media {

public:
    Media(std::string filename, int max_frames): m_video(filename){
        if(!m_video.isOpened()){
            std::cerr << "Video not opened:" << filename<<" \n";
            return;
        }
        m_filename = filename;
        m_frame_count = min(int(m_video. get(cv::CAP_PROP_FRAME_COUNT)), max_frames);
        m_buffer = new cv::Mat[m_frame_count];
        m_fps =   m_video.get(CAP_PROP_FPS);

        fillBuffer();
        m_width = m_buffer[0].cols;
        m_height = m_buffer[0].rows;
        m_fourcc = m_video.get(CAP_PROP_FOURCC);;
    }

    bool display(std::string window_name, int delay, int start = 0, int end = 0, bool exit = true){
        //create window
        namedWindow(window_name, WINDOW_KEEPRATIO );
        if(end == 0){
            end = m_frame_count;
        }
        if(end > m_frame_count){
            end = m_frame_count;
        }

        //loop through buffer
        for (int i = start; i < end; i++) {
            //get frame
            Mat frame = m_buffer[i];
            imshow( window_name, frame ); // display
            //exit on escape
            char key =(char)waitKey(delay);
            if(key==27){
                destroyWindow(window_name);
                return true;
            }
        }
        if(exit){
            destroyWindow(window_name);
        }
        return false;
    }

    const Mat operator[](int index) {
        return m_buffer[index];
    }

    Color getColor(Coord pos){
        /*
        if(pos.x >= m_width || pos.y >= m_height || pos.z >= m_frame_count){
           std::cout << "aaaaaaaaaaaa\n";
            return Color(0,0,0);
        }
         */
        return Color(m_buffer[pos.z].at<Vec3b>(Point(pos.x,pos.y)));
    }
    void setColor(Coord pos, Color color){
        m_buffer[pos.z].at<Vec3b>(Point(pos.x,pos.y)) = color.getColor();
    }
    Coord getDim(){
        return Coord(m_buffer[0].cols, m_buffer[0].rows, m_frame_count);
    }

    int getWidth(){
        return m_width;
    }
    int getHeight(){
        return m_height;

    }
    int getFrameCount(){
        return m_frame_count;
    }

    void saveVideo(String filename){
        VideoWriter out(filename,  m_fourcc   , m_fps, Size(m_width,m_height));
        for (int i = 0; i < m_frame_count; i++) {
          out.write(m_buffer[i]);
        }
        out.release();
        std::cout << "Video saved : " << filename << "\n";
    }

    double getError(Octree* node, Color target){
        double max = 0;
        for (int x = node->min.x; x < node->max.x; x++) {
            for (int y = node->min.y;y < node->max.y; y++) {
                for (int z = node->min.z;z < node->max.z; z++) {
                    double dist = target.distance(getColor(Coord(x,y,z)));
                    if(dist > max){
                        max = dist;
                    }
                }
            }
        }
        return max;
    }

    bool checkError(Octree* node, Color target, double max){

        for (int x = node->min.x; x < node->max.x; x++) {
            for (int y = node->min.y;y < node->max.y; y++) {
                for (int z = node->min.z;z < node->max.z; z++) {
                    double dist = target.distance(getColor(Coord(x,y,z)));
                    if(dist > max){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void applyColor(Octree* node, Color target){
        if(node->isPixel()){
            //just pixel, only one vector
            m_box_count++;
        }else{
            //box has min, max , and color vector
            m_box_count += 3;
        }



        for (int x = node->min.x; x < node->max.x; x++) {
            for (int y = node->min.y;y < node->max.y; y++) {
                for (int z = node->min.z;z < node->max.z; z++) {
                    setColor(Coord(x,y,z),target);
                }
            }
        }
    }

    Color getAverage(Octree* node){
      Color average(0,0,0);
      int amount = 0;
        for (int x = node->min.x; x < node->max.x; x++) {
            for (int y = node->min.y;y < node->max.y; y++) {
                for (int z = node->min.z;z < node->max.z; z++) {
                    average = average + getColor(Coord(x,y,z));
                    amount = amount + 1;
                }
            }
        }
        return average / Color(amount,amount,amount);
    }


    //gets the number of 3d vectors in the image
    const int getSize(){
        return m_width * m_frame_count * m_height;
    }
    const int getCompressedSize(){

        return m_box_count ;
    }

    ~Media(){
        m_video.release();
    }

private:
    //number fo bounding boxes applied
    int m_box_count = 0;
    //info
    int m_width;
    int m_height;
    std::string m_filename;
    int m_frame_count;
    double m_fps = 24;
    int m_fourcc;
    //data
    cv::VideoCapture m_video;
    cv::Mat* m_buffer;
    void fillBuffer(){
        for (int i = 0; i < m_frame_count; i++) {
            //get frame
            cv::Mat frame;
            m_video >> frame;
            m_buffer[i] = frame;
        }
        std::cout << "Buffer created with frame count: " << m_frame_count << "\n";
    }
};

#endif VIDEO_MEDIA_HPP
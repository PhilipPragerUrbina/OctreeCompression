//
// Created by Philip on 7/23/2022.
//

#ifndef VIDEO_COMPRESSOR_HPP
#define VIDEO_COMPRESSOR_HPP
#include "Media.hpp"

class Compressor {
public:
    //start
    int m_min;
    //end
    int m_max;
    //maximum color difference before split
    int m_threshold;
    //max number of splits
    int m_limit ;
    Compressor(Media* video, int min = 0, int max = 0){
        m_video = video;
        m_min = min;
        m_max = max;
    }
    void run(int limit, int threshold){
        m_threshold = threshold;
        m_limit = limit;
        Coord max =  m_video->getDim();
        Coord min = Coord(0,0,0);
        if(m_min != 0){
            min.z = m_min;
        }
        if(m_max != 0){
            max.z = m_max;
        }
        m_tree = new Octree(min,max);
        divide(m_tree,0);
    }

    ~Compressor(){
        if(m_tree != nullptr){
            delete m_tree;
        }

    }

private:
    Media* m_video;
    Octree* m_tree = nullptr;

    void divide(Octree* node, int i){
        i++;
        node->value = m_video->getAverage(node);
        //divide node
        if( i < m_limit &&  m_video->checkError(node,node->value,m_threshold)){
            node->subdivide();
            for(int n = 0; n < 8; n++){
                divide(node->m_children[n], i);
            }
        }else{
            //this is a leaf node
            //normally you would do nothing here and apply the colors when the file is decoded
            //but since we do not have a file format yet we are just applying the bounding boxes right away
            m_video->applyColor(node,node->value);
        }
    }
};

#endif VIDEO_COMPRESSOR_HPP
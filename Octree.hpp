//
// Created by Philip on 7/22/2022.
//

#ifndef VIDEO_OCTREE_HPP
#define VIDEO_OCTREE_HPP


#include "Coord.hpp"
#include "Color.hpp"

class Octree {
public:
    //bounding box
    Coord min;
    Coord max;

    //color value
    Color value;

    //children
    Octree* m_children[8] = {nullptr};

    Octree(Coord in_min, Coord in_max){
        min = in_min;
        max = in_max;
    }

    bool isLeaf(){
        return m_children[0] == nullptr;
    }
    bool isPixel(){
        Coord dim = (max - min);
        int area = dim.x * dim.y * dim.z;
        return area < 2;
    }


    void subdivide() {
        //get size of parent
        Coord dim = (max - min);
        //get size of children
        Coord child_dim_1 = dim.roundDivide(2.0) ;
        Coord child_dim_2 = dim - child_dim_1;

        int x_dim[2] {child_dim_1.x , child_dim_2.x};
        int y_dim[2] {child_dim_1.y , child_dim_2.y};
        int z_dim[2] {child_dim_1.z , child_dim_2.z};
        //generate children
        for (int x = 0; x < 2; x++) {
            for (int y = 0; y < 2; y++) {
                for (int z = 0; z < 2; z++) {
                    Coord child_dim(x_dim[x], y_dim[y],z_dim[z]);
                    Coord offset = Coord(x, y, z);
                    Coord new_min = min + (offset * child_dim_1);
                    Coord new_max = new_min + child_dim;
                    int index = (z * 2 * 2) + (y * 2) + x;
                    m_children[index] = new Octree(new_min,new_max);
                }
            }
        }

    }

    ~Octree(){
        if(m_children[0] != nullptr){

            for (Octree* tree : m_children) {
                delete tree;
            }
        }

    }

};

#endif VIDEO_OCTREE_HPP
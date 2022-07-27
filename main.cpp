#include <iostream>
#include "Compressor.hpp"
#include <filesystem>

int main(int argc, char* args[])
{
    //get filename
    std::string filename = "test_video.mp4";
    if (argc > 1)  filename = args[1];
    //set values
    //max number of frames to use from video
    int max_frames = 600;
    if (argc > 2)  max_frames = std::stoi(args[2]);
    //increments in which to process video(how many frames at a time)
    int increments = 30;
    if (argc > 3)  increments = std::stoi(args[3]);


    //max color distance before split from 0-255. Smaller for higher quality.
    int threshold = 25;
    if (argc > 4)  threshold = std::stoi(args[4]);
    //maximum tree depth. Can be used to pixilate or lower overall quality
    // Make smaller for lower resolution video for max performance.
    int limit = 11;
    if (argc > 5)  limit = std::stoi(args[5]);

    //open video
    Media video(filename,max_frames);
   //compress and apply to video
   //todo separate encode and decode, maybe create a file format
    for(int i =0; i <  video.getFrameCount(); i+= increments){
        if(i + increments >= video.getFrameCount()){
            increments = i + increments - video.getFrameCount()+2;
        }

        Compressor compressor(&video, i, i+increments);
        compressor.run(limit,threshold);
        std::cout << i << " to " << i + increments << " percentage: " <<  (double)i/ video.getFrameCount()*100 << "% \n";

    }
    //display
   std::cout << "Raw size: " << video.getSize() << " vectors, Compressed: " << video.getCompressedSize() << " vectors \n";
   std::cout << "Ratio: " << (double)video.getSize() / (double)video.getCompressedSize() << " times \n";
    video.display("Wheedoo", 25);
    //save video as the video name _compressed
    std::filesystem::path where_to_save(filename);
    where_to_save.replace_filename(where_to_save.stem().string() + "_compressed" + where_to_save.extension().string());
   video.saveVideo(where_to_save.string());


    return 0;
}
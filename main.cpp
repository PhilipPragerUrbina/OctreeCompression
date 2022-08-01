#include <iostream>
#include "Compressor.hpp"
#include <filesystem>

int main(int argc, char *args[]) {
    //get filename
    std::string filename = "test_video.mp4";
    if (argc > 1) filename = args[1];
    //set values
    //max number of frames to use from video
    int max_frames = 600;
    if (argc > 2) max_frames = std::stoi(args[2]);
    //increments in which to process video(how many frames at a time)
    int increments = 30;
    if (argc > 3) increments = std::stoi(args[3]);


    //max color distance before split from 0-255. Smaller for higher quality.
    int threshold = 25;
    if (argc > 4) threshold = std::stoi(args[4]);
    //maximum tree depth. Can be used to pixilate or lower overall quality
    // Make smaller for lower resolution video for max performance.
    int limit = 11;
    if (argc > 5) limit = std::stoi(args[5]);

    //max threads at a time
    // make 1 for single threaded
    int max_threads = 50;
    if (argc > 6) max_threads = std::stoi(args[6]);
    //display user info
    std::cout << "Reading: " << filename << " to frame " << max_frames << " doing " << increments << " frames at a time \n";
    std::cout << "Compression will allow max error of " << threshold << " rgb distance, and has a max tree depth of  " << limit << "\n";
    std::cout << "Program will utilize " << max_threads << " threads at a time \n";


    //open video
    Media video(filename, max_frames);
    std::cout << "Video width: " << video.getWidth() << " height: " << video.getHeight() << " frames: " << video.getFrameCount() << "\n";

    //todo separate encode and decode, maybe create a file format
    //keep track of threads
    std::vector<std::thread> threads;
    std::atomic<double> total_percent;
    //current frames
    int i = 0;
    //compress and apply to video
    while (i < video.getFrameCount()) {
        //add max number of threads at a time. Sort of like a thread pool.
        for (int j = 0; j < max_threads; ++j) {
            if (i >= video.getFrameCount()) {
                break;
            }
            //check if near end and adjust increment
            if ((i + increments) > video.getFrameCount()) {
                increments = video.getFrameCount() - i;
            }
            //compression functions for running compressor in thread
            auto compression_lambda = [](Media *video, int limit, int threshold, int min, int max,
                                         std::atomic<double> *percentage) {
                Compressor compressor(video, min, max);
                compressor.run(limit, threshold);
                //display percentage
                *percentage = *percentage + (double) (max - min) / video->getFrameCount() * 100;
                std::cout << min << " to " << max << " percentage: " << *percentage << " % \n";
            };
            threads.emplace_back(
                    std::thread(compression_lambda, &video, limit, threshold, i, i + increments, &total_percent));
            i += increments;
        }
        //join threads
        for (auto &thread: threads) {
            thread.join();
        }
        threads.clear();
    }


    //display
    std::cout << "Raw size: " << video.getSize() << " vectors, Compressed: " << video.getCompressedSize()
              << " vectors \n";
    std::cout << "Ratio: " << (double) video.getSize() / (double) video.getCompressedSize() << " times \n";
    video.display("Wheedoo", 25);
    //save video as the video name _compressed
    std::filesystem::path where_to_save(filename);
    where_to_save.replace_filename(where_to_save.stem().string() + "_compressed" + where_to_save.extension().string());
    video.saveVideo(where_to_save.string());

    return 0;
}
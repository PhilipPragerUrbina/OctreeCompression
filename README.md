# Octree Video Compression
Also can be called volume compression.
Video compression using octrees. Similar to quadtree compression but for videos with a temporal third axis. Treats the video like a voxel volume, and compresses it like a sparse voxel octree. Just like quadtree compression, it uses color error to decide where to split.

### Dependencies
Requires opencv dll files in the build directory and the opencv include files in the project directory under opencv2 in order to build.


### Usage
You input a video and settings. The program will compress the video and then decompress it. The decompressed result will be saved as the filename + _compressed and will also be displayed. Use escape to exit video display.

Takes up to 6 parameters as command line arguments. 
1. Filename of video relative to working directory.
2. Max number of frames to process. So that you can test compression on just a small part of a longer video.
3. The number of frames to process at a time. Higher means more efficient compression but more memory taken up. The amount you should set depends on the video resolution and the capabilities of your machine.
4. The max color distance before a node is split. Smaller for higher quality.
5. The max tree depth. Can be used to pixilate or lower overall quality.
6. The max number of threads that can run at a time. Make 1 for single threaded. Change depending on the memory taken up(video resolution) and your cpu cores. For shorter videos just set it to a high number, so it does it all at once.

Default values:

1. test_video.mp4
2. 600
3. 30
4. 25
5. 11
6. 50

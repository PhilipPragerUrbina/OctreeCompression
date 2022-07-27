# Octree Video Compression
Also can be called volume compression.
Video compression using octrees. Similar to quadtree compression but for videos with a temporal third axis. Treats the video like a voxel volume, and compresses it like a sparse voxel octree. Just like quadtree compression, it uses color error to decide where to split.

Requires opencv dll files in the build directory and the opencv include files in the project directory under opencv2.

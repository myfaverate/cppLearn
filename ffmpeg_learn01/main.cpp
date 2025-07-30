#include <iostream>

#include "ffmpegLearn01.hpp"

int main(int, char**){
    getFfmpegInfo();
    getVideoDuration("D:/SoftWare/LanguageProjects/C++Projects/ffmpeg_learn01/video/video1.mp4");
    std::cout << "Hello, from ffmpeg_learn01!\n";
}


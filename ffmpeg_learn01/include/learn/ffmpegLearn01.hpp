#ifndef FFMPEGLEARN_01_HPP
#define FFMPEGLEARN_01_HPP

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavcodec/version.h>
#include <libavfilter/version.h>
#include <libavformat/avformat.h>
#include <libavformat/version.h>
#include <libavutil/version.h>
#include <libswresample/version.h>
#include <libswscale/swscale.h>
#include <libswscale/version.h>
#include <libavutil/pixdesc.h>
}

#include <sstream>
#include <fstream>
#include <iostream>

void getFfmpegInfo();
size_t getVideoDuration(std::string videoPath);
void list_protocols();
void extractYumPcmFromMp4(const std::string &videoPath);

#endif  // FFMPEGLEARN_01_HPP


#include "ffmpegLearn01.hpp"

void list_protocols() {
    void* opaque = nullptr;
    const char* name = nullptr;

    std::cout << "Input protocols:" << std::endl;
    while ((name = avio_enum_protocols(&opaque, 0))) {
        std::cout << "name" << name << "\n";  // 这里应当包含 file
    }
}

void getFfmpegInfo(){
    std::stringstream version;
    version << "libAvcodec: " <<  AV_STRINGIFY(LIBAVCODEC_VERSION) << "\n";
    version << "libAvformat: " << AV_STRINGIFY(LIBAVFORMAT_VERSION) << "\n";
    version << "libAvutil: " << AV_STRINGIFY(LIBAVUTIL_VERSION) << "\n";
    version << "libAvFilter: " << AV_STRINGIFY(LIBAVFILTER_VERSION) << "\n";
    version << "avcodecConfiguration: " << avcodec_configuration() << "\n";
    version << "avcodecLicense: " << avcodec_license() << "\n";
    std::cout << version.str() << "\n";
}

size_t getVideoDuration(std::string videoPath) {
    // list_protocols();
    void* opaque = nullptr;
    const AVInputFormat* iformat = nullptr;
    while ((iformat = av_demuxer_iterate(&opaque))) {
        // printf("Demuxer: %s", iformat->name);
    }
    av_log_set_callback([](void* ptr, int level, const char* fmt, va_list vl) {
        if (level <= AV_LOG_INFO) {
            char log[1024];
            // vsnprintf(log, sizeof(log), fmt, vl);
            // (logger::info(TAG,) "log: %s", log);
        }
    });
    // logger::info(TAG, "path: %s", path);
    // 注册所有组件（旧版本需要）
    // av_register_all(); // 新版本 FFmpeg 可能不需要

    AVFormatContext* fmt_ctx = avformat_alloc_context();
    // if (avformat_open_input(&fmt_ctx, path, nullptr, nullptr) != 0) {
    //     env->ReleaseStringUTFChars(file_path, path);
    //     return env->NewStringUTF("Cannot open input file");
    // }

    char err_buf[AV_ERROR_MAX_STRING_SIZE] = {0};
    int ret = avformat_open_input(&fmt_ctx, videoPath.c_str(), nullptr, nullptr);
    if (ret < 0) {
        // av_strerror(ret, err_buf, sizeof(err_buf));
        // logger::info(TAG, "avformat_open_input failed: %s", err_buf);  // 关键！
    }

    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        avformat_close_input(&fmt_ctx);
    }

    int64_t duration = fmt_ctx->duration; // 单位是微秒
    char info[256];
    printf("Duration: %.2f seconds\n", (double)duration / (double)AV_TIME_BASE);
    avformat_close_input(&fmt_ctx);
 
    // Placeholder for actual implementation
    // This function should return the duration of the video in seconds
    return 0; // Return 0 for now as a placeholder
}
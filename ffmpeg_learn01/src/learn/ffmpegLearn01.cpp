
#include "ffmpegLearn01.hpp"

void list_protocols() {
    void *opaque = nullptr;
    const char *name = nullptr;

    std::cout << "Input protocols:" << std::endl;
    while ((name = avio_enum_protocols(&opaque, 0))) {
        std::cout << "name" << name << "\n";  // 这里应当包含 file
    }
}

void getFfmpegInfo() {
    std::stringstream version;
    version << "libAvcodec: " << AV_STRINGIFY(LIBAVCODEC_VERSION) << "\n";
    version << "libAvformat: " << AV_STRINGIFY(LIBAVFORMAT_VERSION) << "\n";
    version << "libAvutil: " << AV_STRINGIFY(LIBAVUTIL_VERSION) << "\n";
    version << "libAvFilter: " << AV_STRINGIFY(LIBAVFILTER_VERSION) << "\n";
    version << "avcodecConfiguration: " << avcodec_configuration() << "\n";
    version << "avcodecLicense: " << avcodec_license() << "\n";
    std::cout << version.str() << "\n";
}

size_t getVideoDuration(std::string videoPath) {
    // list_protocols();
    AVFormatContext *fmt_ctx = avformat_alloc_context();
    char err_buf[AV_ERROR_MAX_STRING_SIZE] = {0};
    int ret =
        avformat_open_input(&fmt_ctx, videoPath.c_str(), nullptr, nullptr);
    if (ret < 0) {
        av_strerror(ret, err_buf, sizeof(err_buf));
    }

    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        avformat_close_input(&fmt_ctx);
    }

    int64_t duration = fmt_ctx->duration;  // 单位是微秒
    char info[256];
    printf("Duration: %.2f seconds\n", (double)duration / (double)AV_TIME_BASE);
    avformat_close_input(&fmt_ctx);
    return 0;
}

void extractYumPcmFromMp4(const std::string &videoPath) {
    AVFormatContext *avFormatContext = avformat_alloc_context();
    if (!avFormatContext) {
        std::cerr << "Could not allocate AVFormatContext." << std::endl;
        return;
    }
    int result = avformat_open_input(&avFormatContext, videoPath.c_str(),
                                     nullptr, nullptr);
    if (result < 0) {
        std::cerr << "Could not open input file: " << av_err2str(result)
                  << std::endl;
        avformat_free_context(avFormatContext);
        return;
    }

    result = avformat_find_stream_info(avFormatContext, nullptr);
    if (result < 0) {
        std::cerr << "Could not open find_stream_info file: "
                  << av_err2str(result) << std::endl;
        avformat_free_context(avFormatContext);
        avformat_close_input(&avFormatContext);
        return;
    }

    std::cout << "Number of streams: " << avFormatContext->nb_streams << "\n";
    std::cout << "Format name: " << avFormatContext->iformat->name << "\n";
    std::cout << "Format longname: " << avFormatContext->iformat->long_name
              << "\n";
    if (avFormatContext->iformat->mime_type != nullptr)
        std::cout << "Format mime_type: " << avFormatContext->iformat->mime_type
                  << "\n";
    else
        std::cout << "Format mime_type: null\n";
    std::cout << "Format extensions: " << avFormatContext->iformat->extensions
              << "\n";
    std::cout << "Duration: " << avFormatContext->duration / AV_TIME_BASE
              << " seconds\n";
    std::cout << "bit_rate : " << avFormatContext->bit_rate << " bit_rate\n";

    int videoStreamIndex = -1;
    int audioStreamIndex = -1;

    for (size_t i = 0; i < avFormatContext->nb_streams; i++) {
        if (avFormatContext->streams[i]->codecpar->codec_type ==
            AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            std::cout << "Video stream index: " << videoStreamIndex << "\n";
        } else if (avFormatContext->streams[i]->codecpar->codec_type ==
                   AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = i;
            std::cout << "Audio stream index: " << audioStreamIndex << "\n";
        }
    }

    if (videoStreamIndex == -1 || audioStreamIndex == -1) {
        std::cerr << "No video or audio stream found in the file." << std::endl;
        avformat_free_context(avFormatContext);
        avformat_close_input(&avFormatContext);
        return;
    }

    const AVStream *videoStream = avFormatContext->streams[videoStreamIndex];
    const AVStream *audioStream = avFormatContext->streams[audioStreamIndex];

    if (!videoStream || !audioStream) {
        std::cerr << "Could not find video or audio stream." << std::endl;
        avformat_free_context(avFormatContext);
        avformat_close_input(&avFormatContext);
        return;
    }

    const AVCodec *videoCodec =
        avcodec_find_decoder(videoStream->codecpar->codec_id);
    const AVCodec *audioCodec =
        avcodec_find_decoder(audioStream->codecpar->codec_id);

    if (!videoCodec || !audioCodec) {
        std::cerr << "Could not find video or audio codec." << std::endl;
        avformat_free_context(avFormatContext);
        avformat_close_input(&avFormatContext);
        return;
    }

    AVCodecContext *videoCodecContext = avcodec_alloc_context3(videoCodec);
    AVCodecContext *audioCodecContext = avcodec_alloc_context3(audioCodec);

    if (!videoCodecContext || !audioCodecContext) {
        std::cerr << "Could not allocate codec contexts." << std::endl;
        avformat_free_context(avFormatContext);
        avformat_close_input(&avFormatContext);
        return;
    }

    std::cout << std::format("videoCodecName: {}, audioCodecName: {}",
                             videoCodecContext->codec->name,
                             audioCodecContext->codec->name)
              << "\n";

    if (avcodec_parameters_to_context(videoCodecContext,
                                      videoStream->codecpar) < 0 ||
        avcodec_parameters_to_context(audioCodecContext,
                                      audioStream->codecpar) < 0) {
        std::cerr << "Could not copy codec parameters to context." << std::endl;
        avcodec_free_context(&videoCodecContext);
        avcodec_free_context(&audioCodecContext);
        avformat_close_input(&avFormatContext);
        avformat_free_context(avFormatContext);
    }

    std::cout
        << std::format(
               "videoCodecContext->width: {}, videoCodecContext->height: {}",
               videoCodecContext->width, videoCodecContext->height)
        << "\n";

    char buf[256];  // 足够大即可
    av_channel_layout_describe(&audioCodecContext->ch_layout, buf, sizeof(buf));
    std::cout << std::format(
                     "audioCodecContext->sample_rate: {}, "
                     "audioCodecContext->channel_layout: {}",
                     audioCodecContext->sample_rate, buf)
              << "\n";
    std::cout << std::format(
                     "videoCodecContext->pix_fmt: {}, "
                     "audioCodecContext->sample_fmt: {}",
                     av_get_pix_fmt_name(videoCodecContext->pix_fmt),
                     av_get_sample_fmt_name(static_cast<AVSampleFormat>(
                         audioCodecContext->sample_fmt)))
              << "\n";
    std::cout << std::format(
                     "videoCodecContext->time_base: {} / {}, "
                     "audioCodecContext->time_base: {} / {} ",
                     videoCodecContext->time_base.num,
                     videoCodecContext->time_base.den,
                     audioCodecContext->time_base.num,
                     audioCodecContext->time_base.den)
              << "\n";
    std::cout << std::format(
                     "videoCodecContext->framerate: {} / {}, "
                     "audioCodecContext->framerate: {} / {}",
                     videoStream->avg_frame_rate.num,
                     videoStream->avg_frame_rate.den,
                     audioStream->avg_frame_rate.num,
                     audioStream->avg_frame_rate.den)
              << "\n";
    std::cout << std::format(
                     "videoCodecContext->bit_rate: {}, "
                     "audioCodecContext->bit_rate: {}",
                     videoCodecContext->bit_rate, audioCodecContext->bit_rate)
              << "\n";
    std::cout
        << std::format(
               "videoCodecContext->profile: {}, audioCodecContext->profile: {}",
               videoCodecContext->profile, audioCodecContext->profile)
        << "\n";
    std::cout
        << std::format(
               "videoCodecContext->level: {}, audioCodecContext->level: {}",
               videoCodecContext->level, audioCodecContext->level)
        << "\n";

    char video_layout_buf[128];
    char audio_layout_buf[128];

    av_channel_layout_describe(&videoCodecContext->ch_layout, video_layout_buf,
                               sizeof(video_layout_buf));
    av_channel_layout_describe(&audioCodecContext->ch_layout, audio_layout_buf,
                               sizeof(audio_layout_buf));

    std::cout << std::format(
                     "videoCodecContext->ch_layout: {}, "
                     "audioCodecContext->ch_layout: {}",
                     video_layout_buf, audio_layout_buf)
              << "\n";

    std::cout << std::format(
                     "videoCodecContext->ch_layout.nb_channels: {}, "
                     "audioCodecContext->ch_layout.nb_channels: {}",
                     videoCodecContext->ch_layout.nb_channels,
                     audioCodecContext->ch_layout.nb_channels)
              << "\n";
    std::cout << std::format(
                     "videoCodecContext->ch_layout.u.mask: {}, "
                     "audioCodecContext->ch_layout.u.mask: {}",
                     videoCodecContext->ch_layout.u.mask,
                     audioCodecContext->ch_layout.u.mask)
              << "\n";

    if (avcodec_open2(videoCodecContext, videoCodec, nullptr) < 0 ||
        avcodec_open2(audioCodecContext, audioCodec, nullptr) < 0) {
        std::cerr << "Could not open codec." << std::endl;
        avcodec_free_context(&videoCodecContext);
        avcodec_free_context(&audioCodecContext);
        avformat_close_input(&avFormatContext);
        avformat_free_context(avFormatContext);
        return;
    }

    AVPacket *avPacket = av_packet_alloc();
    AVFrame *avFrame = av_frame_alloc();
    if (!avPacket || !avFrame) {
        std::cerr << "Could not allocate AVPacket AVFrame." << std::endl;
        avcodec_free_context(&videoCodecContext);
        avcodec_free_context(&audioCodecContext);
        avformat_close_input(&avFormatContext);
        avformat_free_context(avFormatContext);
        return;
    }

    std::ofstream videoFile("output.yuv", std::ios::binary);
    std::ofstream audioFile("output.pcm", std::ios::binary);

    if (videoFile.fail() || audioFile.fail()) {
        std::cerr << "Could not open output files." << std::endl;
        av_packet_free(&avPacket);
        av_frame_free(&avFrame);
        avcodec_free_context(&videoCodecContext);
        avcodec_free_context(&audioCodecContext);
        avformat_close_input(&avFormatContext);
        avformat_free_context(avFormatContext);
        return;
    }

    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        if (avPacket->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(videoCodecContext, avPacket) < 0) {
                std::cerr << "Could not send video packet." << std::endl;
                break;
            }
            while (avcodec_receive_frame(videoCodecContext, avFrame) >= 0) {
                // videoFile.write(reinterpret_cast<const char
                // *>(avFrame->data[0]),
                //                 avFrame->linesize[0] * avFrame->height);
                // 写 Y 分量
                for (int i = 0; i < avFrame->height; i++) {
                    videoFile.write(
                        reinterpret_cast<const char *>(
                            avFrame->data[0] + i * avFrame->linesize[0]),
                        avFrame->width);
                }
                // 写 U 分量
                for (int i = 0; i < avFrame->height / 2; i++) {
                    videoFile.write(
                        reinterpret_cast<const char *>(
                            avFrame->data[1] + i * avFrame->linesize[1]),
                        avFrame->width / 2);
                }
                // 写 V 分量
                for (int i = 0; i < avFrame->height / 2; i++) {
                    videoFile.write(
                        reinterpret_cast<const char *>(
                            avFrame->data[2] + i * avFrame->linesize[2]),
                        avFrame->width / 2);
                }
                // 播放视频
                // ffplay -f rawvideo -vf format=yuv420p -video_size 662x1280 -framerate 60 output.yuv
            }
        } else if (avPacket->stream_index == audioStreamIndex) {
            if (avcodec_send_packet(audioCodecContext, avPacket) < 0) {
                std::cerr << "Could not send audio packet." << std::endl;
                break;
            }
            while (avcodec_receive_frame(audioCodecContext, avFrame) >= 0) {
                // TODO 没正常播放
                // LR LR ...
                // or
                // LL...
                // RR...
                // 这里是planner格式
                // 播放音频  ffplay -f f32le -ar 44100 -ch_layout stereo -i output.pcm
                int dataSize = av_get_bytes_per_sample(audioCodecContext->sample_fmt);
                int nb_channels = audioCodecContext->ch_layout.nb_channels;
                if (dataSize < 0) {
                    std::cerr << "Unsupported sample format." << std::endl;
                    break;
                }
                for(int i = 0; i < avFrame->nb_samples; i++) {
                    for(int ch = 0; ch < nb_channels; ch++) {
                        const uint8_t *sample =
                        avFrame->data[ch] + i * dataSize;
                        audioFile.write(reinterpret_cast<const char
                        *>(sample), dataSize);
                    }
                }
                // Interleaved: 所有通道交错排列
                // audioFile.write(
                //     reinterpret_cast<const char *>(avFrame->data[0]),
                //     avFrame->nb_samples * nb_channels * dataSize);
            }
        }
        av_frame_unref(avFrame);
        av_packet_unref(avPacket);
    }

    videoFile.close();
    audioFile.close();

    av_frame_free(&avFrame);
    av_packet_free(&avPacket);

    avcodec_free_context(&videoCodecContext);
    avcodec_free_context(&audioCodecContext);
    avformat_close_input(&avFormatContext);
    avformat_free_context(avFormatContext);

    std::cout << "end..." << std::endl;
    std::cout << "end..." << std::endl;
    std::cout << "end..." << std::endl;
}
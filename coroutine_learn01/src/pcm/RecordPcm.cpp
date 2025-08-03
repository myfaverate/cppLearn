#include "recordPcm.hpp"

void record() {
    // 这里可以添加录音的具体实现
    std::cout << "Recording PCM audio..." << std::endl;
    
    // WAVEFORMATEX
    WAVEFORMATEX waveFormat = {};
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 2; // 立体声
    waveFormat.nSamplesPerSec = 44100; // 采样率
    waveFormat.wBitsPerSample = 16; // 每个样本16位
    waveFormat.nBlockAlign = (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0; // PCM格式没有额外数据
    // 这里可以添加更多的录音逻辑，比如打开设备、开始录音等

    WAVEHDR waveHeader = {};
    HWAVEIN hWaveIn;

    if(waveInOpen(&hWaveIn, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
        std::cerr << "Failed to open wave input device." << std::endl;
        return;
    }

    // 静态缓冲区
    const int BUFFER_SIZE = 44100 * (16 / 8) * 2 * 5; // 5秒的缓冲区，立体声16位
    char buffer[BUFFER_SIZE];

    waveHeader.lpData = buffer;
    waveHeader.dwBufferLength = BUFFER_SIZE;
    waveHeader.dwFlags = 0;
    waveHeader.dwLoops = 1;

    auto result = waveInPrepareHeader(hWaveIn, &waveHeader, sizeof(WAVEHDR));
    if (result != MMSYSERR_NOERROR) {
        std::cerr << "Failed to prepare wave header." << std::endl;
        waveInClose(hWaveIn);
        return;
    }
    // 添加缓冲区并开始录音
    result = waveInAddBuffer(hWaveIn, &waveHeader, sizeof(WAVEHDR));

    if (result != MMSYSERR_NOERROR) {
        std::cerr << "Failed to add wave buffer." << std::endl;
        waveInUnprepareHeader(hWaveIn, &waveHeader, sizeof(WAVEHDR));
        waveInClose(hWaveIn);
        return;
    }

    result = waveInStart(hWaveIn);

    if (result != MMSYSERR_NOERROR) {
        std::cerr << "Failed to start wave input." << std::endl;
        waveInUnprepareHeader(hWaveIn, &waveHeader, sizeof(WAVEHDR));
        waveInClose(hWaveIn);
        return;
    }

    std::cout << "Recording " << 5 << " seconds...\n";
    Sleep(5 * 1000);

    waveInReset(hWaveIn); // 清空缓冲区队列并中止正在录音
    waveInStop(hWaveIn);
    waveInUnprepareHeader(hWaveIn, &waveHeader, sizeof(WAVEHDR));
    waveInClose(hWaveIn);

    std::ofstream out("raw.pcm", std::ios::binary);
    out.write(buffer, BUFFER_SIZE);
    out.close();

    std::cout << "Saved to raw.pcm\n";
    
    std::cout << "Recording finished." << std::endl;
}
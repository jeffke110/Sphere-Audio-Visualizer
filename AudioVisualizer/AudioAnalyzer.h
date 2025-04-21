#ifndef AUDIOANALYZER_H
#define AUDIOANALYZER_H
#pragma once


#define MINIMP3_IMPLEMENTATION
#define MINIMP3_EX_IMPLEMENTATION
#include "minimp3_ex.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#define max(a, b) ((a) > (b) ? (a) : (b))
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>
#include <cmath>
#include <algorithm>
#include <fftw3.h>
#include <mutex>

#define FFT_SIZE 1024
#define NUM_BARS 64
#define BAR_HEIGHT 20


class AudioAnalyzer {
public:
    AudioAnalyzer(const char* filename) : keep_running(true), samples_collected(0), channels(2) {
        fft_input.resize(FFT_SIZE, 0.0f);

        if (mp3dec_ex_open(&g_mp3, filename, MP3D_SEEK_TO_SAMPLE)) {
            std::cerr << "Failed to open MP3 file." << std::endl;
            exit(1);
        }
        channels = g_mp3.info.channels;

        config = ma_device_config_init(ma_device_type_playback);
        config.sampleRate = g_mp3.info.hz;
        config.playback.format = ma_format_s16;
        config.playback.channels = channels;
        config.dataCallback = dataCallback;
        config.pUserData = this;

        if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
            std::cerr << "Failed to initialize audio device." << std::endl;
            exit(1);
        }

        fft_output = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * FFT_SIZE);
        fft_plan = fftwf_plan_dft_r2c_1d(FFT_SIZE, fft_input.data(), fft_output, FFTW_ESTIMATE);
    }

    ~AudioAnalyzer() {
        stop();
        fftwf_destroy_plan(fft_plan);
        fftwf_free(fft_output);
        ma_device_uninit(&device);
        mp3dec_ex_close(&g_mp3);
    }

    void start() {
        keep_running = true;
        analyzer_thread = std::thread(&AudioAnalyzer::analyzeLoop, this);
        ma_device_start(&device);
    }

    void stop() {
        keep_running = false;
        if (analyzer_thread.joinable()) analyzer_thread.join();
    }
    
    std::vector<float> getMagnitudes() {
        std::lock_guard<std::mutex> lock(magMutex);
        return latestMagnitudes;
    }

private:
    mp3dec_ex_t g_mp3;
    ma_device_config config;
    ma_device device;
    std::thread analyzer_thread;
    std::atomic<bool> keep_running;
    std::atomic<int> samples_collected;
    int channels;
    std::vector<float> fft_input;
    fftwf_complex* fft_output;
    fftwf_plan fft_plan;
    std::vector<float> latestMagnitudes;
    std::mutex magMutex;

    void clearConsole() {
#ifdef _WIN32
        system("cls");
#else
        std::cout << "\033[2J\033[1;1H";
#endif
    }
    void drawSpectrum(const std::vector<float>& magnitudes) {
        clearConsole();

        for (int y = BAR_HEIGHT; y > 0; y--) {
            for (int x = 0; x < NUM_BARS; x++) {
                if (magnitudes[x] / 2 >= y)
                    std::cout << "|";
                else
                    std::cout << " ";
            }
            std::cout << "\n";
        }
    }


    void analyzeLoop() {
        while (keep_running) {
            if (samples_collected >= FFT_SIZE) {
                fftwf_execute(fft_plan);

                std::vector<float> magnitudes(NUM_BARS, 0.0f);
                std::vector<float> smoothedMagnitudes = std::vector<float>(NUM_BARS, 0.0f);
                float cap = 1000.0f; // or any max value that works well visually

                for (int i = 0; i < FFT_SIZE / 2; i++) {
                    float mag = std::sqrt(fft_output[i][0] * fft_output[i][0] + fft_output[i][1] * fft_output[i][1]);
                    mag = min(mag, cap); // Clamp to max value
                    int band = i * NUM_BARS / (FFT_SIZE / 2);
                    magnitudes[band] = max(magnitudes[band], mag);
                }
                //drawSpectrum(magnitudes);
                samples_collected = 0;
                float smoothing = 0.2f;  // 0.0 = no update, 1.0 = instant change

                for (int i = 0; i < NUM_BARS; ++i) {
                    // Smooth toward new value
                    smoothedMagnitudes[i] = smoothedMagnitudes[i] * (1.0f - smoothing) + magnitudes[i] * smoothing;

                    // Optional: add soft cap to reduce spikes
                    float softCap = 800.0f;
                    if (smoothedMagnitudes[i] > softCap) {
                        smoothedMagnitudes[i] = softCap + (smoothedMagnitudes[i] - softCap) * 0.25f;
                    }
                }

                std::lock_guard<std::mutex> lock(magMutex);
                latestMagnitudes = smoothedMagnitudes;
                //std::this_thread::sleep_for(std::chrono::milliseconds(10));

            }
     
        }
    }

    static void dataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount) {
        AudioAnalyzer* self = reinterpret_cast<AudioAnalyzer*>(device->pUserData);
        size_t samplesToRead = frameCount * self->channels;
        mp3dec_ex_read(&self->g_mp3, (mp3d_sample_t*)output, samplesToRead);

        if (self->samples_collected < FFT_SIZE) {
            for (ma_uint32 i = 0; i < frameCount && self->samples_collected < FFT_SIZE; i++) {
                short* s = (short*)output;
                float left = s[i * self->channels] / 32768.0f;
                float right = (self->channels == 2) ? s[i * self->channels + 1] / 32768.0f : left;
                self->fft_input[self->samples_collected++] = (left + right) / 2.0f;
            }
        }
    }


};


#endif // !AUDIOANALYZER_H


#include <portaudio.h>
#include <cmath>
#include <stdexcept>

class ToneGenerator {
private:
    PaStream *stream;
    float frequency;
    float amplitude;

    static int paCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData) {
        ToneGenerator *generator = static_cast<ToneGenerator*>(userData);
        float *out = static_cast<float*>(outputBuffer);
        static float phase = 0.0f;

        for (unsigned long i = 0; i < framesPerBuffer; i++) {
            *out++ = generator->amplitude * sin(phase);
            phase += 2.0f * M_PI * generator->frequency / 44100.0f;
            if (phase >= 2.0f * M_PI) phase -= 2.0f * M_PI;
        }

        return paContinue;
    }

public:
    ToneGenerator(float freq = 440.0f, float amp = 0.5f) 
        : frequency(freq), amplitude(amp), stream(nullptr) {
        PaError err = Pa_Initialize();
        if (err != paNoError) {
            throw std::runtime_error("PortAudio initialization failed");
        }
    }

    ~ToneGenerator() {
        if (stream) {
            Pa_CloseStream(stream);
        }
        Pa_Terminate();
    }

    void start() {
        PaError err = Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, 44100,
                                           256, paCallback, this);
        if (err != paNoError) {
            throw std::runtime_error("Failed to open PortAudio stream");
        }

        err = Pa_StartStream(stream);
        if (err != paNoError) {
            throw std::runtime_error("Failed to start PortAudio stream");
        }
    }

    void stop() {
        if (stream) {
            Pa_StopStream(stream);
        }
    }

    void setFrequency(float freq) {
        frequency = freq;
    }

    void setAmplitude(float amp) {
        amplitude = amp;
    }
};

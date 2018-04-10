#pragma once

#include <Gamebuino-Meta.h>
#include <utility/Sound.h>

class GSFX
{
public:
    static const uint8_t FPP = 8; // Fixed point precision
    static const uint8_t MAX_PATTERNS = 8; //Max FX per Pattern
    static const uint8_t SR_DIVIDER = 44100 / SOUND_FREQ;

    enum class WaveType : int32_t // so we can cast FX as an int32_t array
    {
        NOISE,
        SQUARE,
        WAVE_COUNT
    };

    // Users create and use these
    union FX {
        struct {
            GSFX::WaveType type; // Type of the wave that will be played
            int32_t volume_start; // Volume amplitude as a 8 bit fixed point 
            int32_t volume_sweep; // How much the volume change for each sample (8 bit fixed point)

            int32_t period_start; 
            int32_t period_sweep;

            int32_t length;
        };
        int32_t params[6];
    };

    struct Pattern
    {
        FX fxs[MAX_PATTERNS];
        uint8_t length;
    };

    GSFX();
    ~GSFX();

    // Init the sfx library so it can play sound
    int8_t init();

    // Plays the given FX structure
    void play(const GSFX::FX & fx);
    void play(const GSFX::Pattern & pattern);



private:
    bool is_init;
    int8_t _channel_id;

    class Sound_Handler_GSFX : public Gamebuino_Meta::Sound_Handler
    {
    public:
        static const int NUM_SAMPLES = 2048 / GSFX::SR_DIVIDER;
        uint8_t _buffer[NUM_SAMPLES]; // Put first so it's memory aligned to 32bit
        Sound_Handler_GSFX(GSFX * parent);
        ~Sound_Handler_GSFX();

        bool init();
        void update();
        void rewind();
        void generateNoise() __attribute__((optimize("-O3"))); // Handle noise instrument
        void generateSquare() __attribute__((optimize("-O3")));

        void play(const GSFX::FX & fx);
        void play(const GSFX::Pattern & pattern);

        uint32_t getPos();
        GSFX::FX _current_fx;
        GSFX::Pattern _current_pattern;
        uint8_t _current_pattern_fx;

        uint32_t _current_fx_time;
        int32_t _current_fx_volume;
        int32_t _current_fx_freq;
        uint16_t _head_index;

        int32_t _noise_period;
        int32_t _square_period;
        int32_t _square_polarity;

        void resetGenerators();

        inline uint8_t getVolume() 
        {
            _current_fx_volume += _current_fx.volume_sweep * SR_DIVIDER;
            return (min(127,max(0,(_current_fx_volume >> FPP)))); 
        } __attribute__((optimize("-O3")));

        inline int32_t getFrequency()
        {
            _current_fx_freq += _current_fx.period_sweep * SR_DIVIDER;
            return (_current_fx_freq);
        } __attribute__((optimize("-O3")));
        
        GSFX *_parent;
    };

    Sound_Handler_GSFX _handler;

};
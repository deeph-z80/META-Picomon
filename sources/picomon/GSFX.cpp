#include "GSFX.h"
#include <Gamebuino-Meta.h>

GSFX::GSFX() : _handler(this)
{

}

GSFX::~GSFX()
{

}

int8_t GSFX::init()
{
    _channel_id = gb.sound.play(&_handler, true);
    is_init = (_channel_id != -1);
    if (is_init)
    {
        _handler.init();
    }
    return _channel_id;
}

void GSFX::play(const GSFX::FX & fx)
{
    _handler.play(fx);
}

void GSFX::play(const GSFX::Pattern & pattern)
{
    _handler.play(pattern);
}


// SOUND HANDLER

GSFX::Sound_Handler_GSFX::Sound_Handler_GSFX(GSFX * parent): Gamebuino_Meta::Sound_Handler(NULL), _parent(parent)
{

}

GSFX::Sound_Handler_GSFX::~Sound_Handler_GSFX()
{

}

bool GSFX::Sound_Handler_GSFX::init()
{
    // init buffer to 0 volume
    memset(_buffer, 0x80, NUM_SAMPLES);
    _head_index = 0;
    channel->index = 0;
    channel->total = NUM_SAMPLES;
    channel->last = false;
    channel->use = true;
    channel->buffer = &_buffer[0];

    _current_fx_time = UINT32_MAX;
    _current_fx = {GSFX::WaveType::NOISE, 0xFFFF,-80,0,0,10000};
    _current_fx_volume =  0;
    _current_fx_freq =  0;
    _noise_period = 0;

    _current_pattern_fx = UINT8_MAX;

    resetGenerators();
}

void GSFX::Sound_Handler_GSFX::update()
{
    // Check if we should advance in the pattern
    if (_current_fx_time >= _current_fx.length)
    {
        if (_current_pattern_fx < _current_pattern.length-1)
        {
            _current_pattern_fx++;
            play(_current_pattern.fxs[_current_pattern_fx]);
        }
    }

    // Generate sound
    if (_current_fx_time < _current_fx.length)
    {
        switch(_current_fx.type)
        {
            case GSFX::WaveType::NOISE:
                generateNoise();
                break;
            case GSFX::WaveType::SQUARE:
                generateSquare();
                break;
            default:
                // WTF man
                break;
        }
    }
    else if (_current_fx_time != UINT32_MAX)
    {
        memset(_buffer, 0x80, NUM_SAMPLES);
        _current_fx_time = UINT32_MAX;
    }
    else
    {
        //do nothing
    }
}

void GSFX::Sound_Handler_GSFX::rewind()
{
    //rewind stuff
}

uint32_t GSFX::Sound_Handler_GSFX::getPos()
{
    return UINT32_MAX;
}

void GSFX::Sound_Handler_GSFX::play(const GSFX::FX & fx)
{
    _current_fx = fx;
    _current_fx_time = 0;
    _current_fx_volume = fx.volume_start;
    _current_fx_freq = fx.period_start;

    resetGenerators();
}

void GSFX::Sound_Handler_GSFX::play(const GSFX::Pattern & pattern)
{
    _current_pattern = pattern;
    _current_pattern_fx = 0;
    play(_current_pattern.fxs[_current_pattern_fx]);
}

void GSFX::Sound_Handler_GSFX::resetGenerators()
{
    _square_period = 0;
    _noise_period = 0;
    _square_polarity = 1;
}

void GSFX::Sound_Handler_GSFX::generateNoise()
{
    uint32_t target = channel->index;
    static uint16_t lfsr = 1;
    do
    {
        uint32_t volume = getVolume() + 0x80;

        if(--_noise_period <= 0) 
        {
            _noise_period = max((getFrequency()>>(FPP+4))/ GSFX::SR_DIVIDER,0);
            bool bit = (lfsr ^ (lfsr >> 1)) & 1;
            lfsr = (lfsr >> 1) ^ (bit << 14);
        }
        uint32_t sample = (lfsr & 1) ? 0x80808080 : (volume << 24) | (volume << 16) | (volume << 8) | volume;

        _head_index = (_head_index+1)%(NUM_SAMPLES>>2);
        _current_fx_time +=4*SR_DIVIDER;
        ((uint32_t*)(&_buffer[0]))[_head_index] = sample;
    }
    while (_head_index != target>>2);
}

void GSFX::Sound_Handler_GSFX::generateSquare()
{
    uint32_t target = channel->index;
    do
    {
        uint32_t volume = getVolume();
        _square_period -=256;
        if(_square_period <= 0) 
        {
            _square_period += max(getFrequency()/ GSFX::SR_DIVIDER,0);
            _square_polarity = - _square_polarity;
        }
        volume = 0x80 + _square_polarity * volume;
        uint32_t sample =  (volume << 24) | (volume << 16) | (volume << 8) | volume;

        _head_index = (_head_index+1)%(NUM_SAMPLES>>2);
        _current_fx_time +=4*SR_DIVIDER;
        ((uint32_t*)(&_buffer[0]))[_head_index] = sample;
    }
    while (_head_index != target>>2);
}
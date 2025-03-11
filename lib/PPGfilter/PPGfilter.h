#ifndef _PPGFILTER_H_
#define _PPGFILTER_H_

#include <Arduino.h>

class PPGfilter{
    private:
        //EMA filter
        float EMA_ALPHA_ = 0.6;
        float EMA_ALPHA = 0.2;
        int EMA_LP = 0;
        int EMA_HP = 0;
        int EMA_LP_ = 0;
        int xf = 0;
        int xf_1 = 0;
        int xf_2 = 0;
        int contf = 0;
        unsigned long lastDebounceTime = 0;
        unsigned long debounceDelay = 50;
        int signal;
        int ppgAdjusted;
    public:
        PPGfilter();
        void begin();
        int EMAFilter(int signal);
};

#endif
#ifndef _PPGFILTER_H_
#define _PPGFILTER_H_

#include <Arduino.h>

class PPGfilter{
    private:
        //EMA filter
        float ema_alpha_;
        int EMA_LP = 0;
        int EMA_HP = 0;
        int EMA_LP_ = 0;
        int xf = 0;
        int xf_1 = 0;
        int xf_2 = 0;
        int contf = 0;
        unsigned long lastDebounceTime = 0;
        unsigned long debounceDelay;
        int signal;
    public:
        PPGfilter();
        ~PPGfilter();
        int EMAFilter(int, float, unsigned long);
};

#endif
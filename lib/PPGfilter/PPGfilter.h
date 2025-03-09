#ifndef _PPGFILTER_H_
#define _PPGFILTER_H_

#include <Arduino.h>

class PPGfilter{
    private:
        //EMA filter
        float EMA_ALPHA_ = 0.05;
        float EMA_ALPHA = 0.2;
        int EMA_LP = 0;
        int EMA_HP = 0;
        int EMA_LP_ = 0;
        int signal;
    public:
        PPGfilter();
        ~PPGfilter();
        void init();
        int EMAFilter(int signal);
};

#endif
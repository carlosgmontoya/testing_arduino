#include <Arduino.h>
#include "PPGfilter.h"


PPGfilter::PPGfilter(){

}

PPGfilter::~PPGfilter(){

}

void PPGfilter::init(){

}

int PPGfilter::EMAFilter(int signal){
    this->signal = signal;
    // Low pass
    EMA_LP_ = EMA_ALPHA_ * signal + (1 - EMA_ALPHA_) * EMA_LP_;

    // High pass
    EMA_LP = EMA_ALPHA * EMA_LP_ + (1 - EMA_ALPHA) * EMA_LP;
    EMA_HP = EMA_LP_- EMA_LP;
    return EMA_HP;
}
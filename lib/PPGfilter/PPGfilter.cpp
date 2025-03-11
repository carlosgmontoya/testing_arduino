#include <Arduino.h>
#include "PPGfilter.h"


PPGfilter::PPGfilter(){

}

void PPGfilter::begin(){

}

int PPGfilter::EMAFilter(int signal){
    this->signal = signal;

    // Low pass
    EMA_LP_ = EMA_ALPHA_ * signal + (1 - EMA_ALPHA_) * EMA_LP_;
    // return EMA_LP_;

    // High pass
    // EMA_LP = EMA_ALPHA * EMA_LP_ + (1 - EMA_ALPHA) * EMA_LP;
    // EMA_HP = EMA_LP_- EMA_LP;
    // return EMA_HP;


    //Peak and valley corrector
      xf = EMA_LP_;
    //xf = signal;
    

    // Pico (cambia a pendiente negativa)
    if(xf - xf_1 <0 && xf_1 - xf_2 >= 0 && contf == 0)
    {
        lastDebounceTime = millis(); 
        contf++;
    }

   
    if((millis() - lastDebounceTime) < debounceDelay) 
    {
        xf = xf_1;
    }
    else
    {
        contf=0;
    }

    xf_2 = xf_1;
    xf_1 = xf;
    
    return xf;
   
}
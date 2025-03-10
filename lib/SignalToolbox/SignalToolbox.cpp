 
#include <Arduino.h>
#include "SignalToolbox.h"

SignalToolbox::SignalToolbox(){

}

SignalToolbox::~SignalToolbox(){

}

void SignalToolbox::init(){

}

int SignalToolbox::Frequhr(int filter2){
    this->filter2 = filter2;

    //Peak and valley detector
    xn = filter2;

    // Valley (cambia a pendiente positiva)
    if(xn - xn_1 > 0 && xn_1 - xn_2 <= 0)
    {
        if(conthr >= 1)
        {
            endhr = millis();
            periodhr = endhr - starthr;
            freqhr = 60000 / periodhr;
            conthr = 0;
            starthr = millis();
        }
    }    
    
    // Pico (cambia a pendiente negativa)
    if(xn - xn_1 <0 && xn_1 - xn_2 >= 0)
    {
        conthr++;
    }

    return freqhr;
}

int SignalToolbox::Averagehr(int filter3){
    this->filter3 = filter3;

    //Average
    xp = filter3;

    // hr prom
    datahr[0]=xp;
    for(int ihr=0; ihr<9; ihr++)
    {
        datahr[9-ihr]=datahr[8-ihr];
        sumahr = sumahr + datahr[9-ihr];
    }
    promhr=sumahr/9;
    sumahr=0;

    return promhr;
}

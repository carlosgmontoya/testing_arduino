 
#include <Arduino.h>
#include "SignalToolbox.h"

SignalToolbox::SignalToolbox(){

}

void SignalToolbox::begin(){

}

/*
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

    xn_2 = xn_1;
    xn_1 = xn;

    return freqhr;
}

*/

void SignalToolbox::Averagehr(int filter3){
    this->filter3 = filter3;

    //Peak and valley detector
    xp = filter3;

    // VALLE (cambia a pendiente positiva)
    if(xp - xp_1 > 0 && xp_1 - xp_2 <= 0)
    {

        // INCIA VALLE Frecuencia instantánea y promedio
        if(contphr >= 1)
        {
            endphr = millis();
            periodphr = endphr - startphr;
            freqphr = 60000 / periodphr;


            // hr prom
            datahr[0]=freqphr;
            for(int ihr=0; ihr<9; ihr++)
            {
                datahr[9-ihr]=datahr[8-ihr];
                sumahr = sumahr + datahr[9-ihr];
            }
            promhr=sumahr/9;
            sumahr=0;

            contphr = 0;
            startphr = millis();
        }

        // FINALIZA VALLE Frecuencia instantánea y promedio

        // INICIA VALLE Amplitud e Intensidad
        pahr = xp_1;
        contra++;
        // FINALIZA VALLE Amplitud

    }    
    
    // PICO (cambia a pendiente negativa)
    if(xp - xp_1 <0 && xp_1 - xp_2 >= 0)
    {

        // INICIA PICO Frecuencia instantánea y promedio
        contphr++;
        // FINALIZA PICO Frecuencia instantánea y promedio

        // INICIA PICO Amplitud e Intensidad
        if(contra >= 1)
        {

            pbhr = xp_1;
            amphr = pbhr - pahr;
            inthr = (pbhr + pahr)/2;
            contra = 0;


        }
        // FINALIZA PICO Amplitud e Intensidad
    }

    xp_2 = xp_1;
    xp_1 = xp;

//    return promhr;
}

int SignalToolbox::GetFreqhr()
{
    return freqphr;
}

int SignalToolbox::GetAvehr()
{
    return promhr;
}

int SignalToolbox::GetAmphr()
{
    return amphr;
}
int SignalToolbox::GetInthr()
{
    return inthr;
}


/*
int SignalToolbox::Amplitudehr(int filter4){
    this->filter4 = filter4;

    //Peak and valley detector
    xa = filter4;

    // Valley (cambia a pendiente positiva)
    if(xa - xa_1 > 0 && xa_1 - xa_2 <= 0)
    {
        pahr = xa_1;
        contra++;
    }    
    
    // Pico (cambia a pendiente negativa)
    if(xa - xa_1 <0 && xa_1 - xa_2 >= 0)
    {
        if(contra >= 1)
        {
            pbhr = xa_1;
            amphr = pbhr - pahr;
            contra = 0;
        }
    }

    xa_2 = xa_1;
    xa_1 = xa;

    return amphr;

}

int SignalToolbox::Intensityhr(int filter5){
    this->filter5 = filter5;

    //Peak and valley detector
    xb = filter5;

    // Valley (cambia a pendiente positiva)
    if(xb - xb_1 > 0 && xb_1 - xb_2 <= 0)
    {
        pchr = xb_1;
        contrb++;
    }    
    
    // Pico (cambia a pendiente negativa)
    if(xb - xb_1 <0 && xb_1 - xb_2 >= 0)
    {
        if(contrb >= 1)
        {
            pdhr = xb_1;
            inthr = (pchr + pdhr)/2;
            contrb = 0;
        }
    }

    xb_2 = xb_1;
    xb_1 = xb;

    return inthr;

}
*/
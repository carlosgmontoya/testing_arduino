#ifndef _SIGNALTOOLBOX_H_
#define _SIGNALTOOLBOX_H_
#include <Arduino.h>

class SignalToolbox{
    private:
        // Peak detector
        int xn = 0;
        int xn_1 = 0;
        int xn_2 = 0;

        // Peak detector
        int xp = 0;
        int xp_1 = 0;
        int xp_2 = 0;

        // Peaks of peaks (intensity)
        int pn = 0;
        int pn_1 = 0;
        int pn_2 = 0;

        // Peaks of amplitude
        int pa = 0;
        int pa_1 = 0;
        int pa_2 = 0;

        // Peaks of frequency
        int pb = 0;
        int pb_1 = 0;
        int pb_2 = 0;

        // Peaks of sum
        int pc = 0;
        int pc_1 = 0;
        int pc_2 = 0;

        // Frequency of intensity
        int startr = 0;
        int endr = 0;
        int halfr = 0;
        int periodr = 0;
        int freqr = 0;

        // Frequency of amplitude
        int starta = 0;
        int enda = 0;
        int halfa = 0;
        int perioda = 0;
        int freqa = 0;
        int contra = 0;

        // Frequency of frequency
        int startb = 0;
        int endb = 0;
        int halfb = 0;
        int periodb = 0;
        int freqb = 0;
        int contrb = 0;

        // Frequency of intensity
        int startc = 0;
        int endc = 0;
        int halfc = 0;
        int periodc = 0;
        int freqc = 0;
        int contrc = 0;

        // Frequency of heart
        int starthr = 0;
        int endhr = 0;
        int periodhr = 0;
        int freqhr = 0;
        int conthr = 0;
        int startphr = 0;
        int endphr = 0;
        int periodphr = 0;
        int freqphr = 0;
        int contphr = 0;
        int pahr = 0;
        int pbhr = 0;
        int amphr = 0;
        int conta = 0;
        int periodhr_1=0;
        int datahr[10]={0,0,0,0,0,0,0,0,0,0};
        int sumahr=0;
        int promhr=0;
        int ihr=0;
        int filter2;
        int filter3;
    public:
        SignalToolbox();
        ~SignalToolbox();
        void init();
        int Frequhr(int filter2);
        int Averagehr(int filter3);
};

#endif
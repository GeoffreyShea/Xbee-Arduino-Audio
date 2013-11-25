/**
 */

#ifndef Dac_h
#define Dac_h

#include <stdint.h>

class Dac {
    public:
        Dac();
        void startPlayback(void);
        void stopPlayback(void);
        void send(uint16_t data);
};




#endif //Dac_h

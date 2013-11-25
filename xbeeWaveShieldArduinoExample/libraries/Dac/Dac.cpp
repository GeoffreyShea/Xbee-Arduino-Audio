/**
 */

#include <avr/interrupt.h>
#include <mcpDac.h>
#include <Dac.h>

#define SAMPLE_RATE 22000 //8000

Dac::Dac() {

}

void Dac::startPlayback(void) {
    // Setup mode for DAC ports
    mcpDacInit();
    
    // Set up timer one
    
    // Normal operation - no pwm not connected to pins
    TCCR1A = 0;
    
    // no prescaling, CTC mode
    TCCR1B = _BV(WGM12) | _BV(CS10); 
    
    // Sample rate - play stereo interleaved
    OCR1A =  F_CPU / SAMPLE_RATE;    // 16e6 / 8000 = 2000
    
    // Enable timer interrupt for DAC ISR
    TIMSK1 |= _BV(OCIE1A);
    
    sei();
}

void Dac::stopPlayback(void) {
    TIMSK1 &= ~_BV(OCIE1A);   // turn off interrupt
}

//------------------------------------------------------------------------------
// send 12 bits to dac
// trusted compiler to optimize and it does 
// csLow to csHigh takes 8 - 9 usec on a 16 MHz Arduino
void Dac::send(uint16_t data) {
    mcpDacCsLow();
    // send DAC config bits
    mcpDacSdiLow();
    mcpDacSckPulse();  // DAC A
    mcpDacSckPulse();  // unbuffered
    mcpDacSdiHigh();
    mcpDacSckPulse();  // 1X gain
    mcpDacSckPulse();  // no SHDN
    // send top 12 data bits
    mcpDacSendBit(data, 15);
    mcpDacSendBit(data, 14);
    mcpDacSendBit(data, 13);
    mcpDacSendBit(data, 12);
    mcpDacSendBit(data, 11);
    mcpDacSendBit(data, 10);
    mcpDacSendBit(data,  9);
    mcpDacSendBit(data,  8);
    mcpDacSendBit(data,  7);
    mcpDacSendBit(data,  6);
    mcpDacSendBit(data,  5);
    mcpDacSendBit(data,  4);
    mcpDacCsHigh();
}


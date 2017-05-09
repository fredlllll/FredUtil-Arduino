// this holds code that is associated with optimizations

/*
methods for usage (you can use const variables for the arguments in <>):
    nop<count>();
        emits 'count' nop instructions in the binary. one nop == 1 cycle
    switchOn<pin>();
    switchOff<pin>();
        switches a pin on or off in 2 cycles. works with the A0-A5 macros too
    switchOnExclusive<pin>();
        switches a pin on exclusively in a port in 1 cycle. all other pins of the port will be set to LOW
    switchOffPortOfPin<pin>();
        switches the port of a pin off in 1 cycle (and with that the pin itself).
        
the described methods throw static assertion errors if they detect that you used an invalid pin
*/

#ifdef DEBUG_OPTIMIZATION
#define DBGMARKER() __asm__("sleep\n\t") //so its easier to see the start and end of a function in assembly
#else
#define DBGMARKER()
#endif

template<long count> __attribute__((always_inline)) inline void nop();

template<long nanos> struct NanosToCycles{
    enum { cycles = (long)(nanos / ((double)1000000000/F_CPU)) };
};

const uint8_t cyclesPerLoop = 7;
template<long nanos> inline void delayNanoseconds(){
    if( NanosToCycles<nanos>::cycles <= cyclesPerLoop){
        nop<NanosToCycles<nanos>::cycles>();
    }else{
        //TODO: force gcc to NOT unroll the loop. adding volatile fucks with the code too much though
        for(volatile long i = 0; i< NanosToCycles<nanos>::cycles/cyclesPerLoop;i++ ){
            __asm__ __volatile__("");
        }
        nop<1>();//loop uses 1 cycle less on last iteration, cause its not branching
        nop<NanosToCycles<nanos>::cycles%cyclesPerLoop>();
    }
    /*
    adiw	r24, 0x01	; 1
    cpi	r24, 0x73	; 115
    cpc	r25, r1
    brne	.-12     	; 0x1b0 <main+0x8c>
    */
}

template<long nanos> __attribute__((error("this is currently broken"))) inline void delayNanoseconds();

template<> inline void delayNanoseconds<0>(){
    
}

// creates an arbitrary amount of nops
// usage: nop<5>();
template<long count> __attribute__((always_inline)) inline void nop(){
    nop<count - 1>();
    __asm__ __volatile__("nop\n\t");
}

template<> __attribute__((always_inline)) inline void nop<0>(){
    // do nothing. this is needed so the above nop recursion ends
}

// converts a pin to the address of a port in an uint8_t (cant return pointer here cause c++ is stupid)
constexpr uint8_t fredDigitalPinToPort(const uint8_t pin){
    return (pin < 8) ? (uint8_t)&PORTD : (pin < 14) ? (uint8_t)&PORTB : (pin >= A0 && pin <= A5) ? (uint8_t)&PORTC : 0;
}

// converts pin to offset that is used in switch(On/Off)
constexpr uint8_t fredDigitalPinToIndex(const uint8_t pin){
    return (pin < 8) ? pin : (pin < 14) ? pin-8 : (pin >= A0 && pin <= A5) ? pin-A0 : 0;
}

template<uint8_t pinIndex, uint8_t port> __attribute__((always_inline)) inline void switchOnInPort(){
    *((volatile uint8_t*)port) |= 1 << pinIndex;
}

template<> inline void switchOnInPort<0,0>(){
    // do nothing if port is 0 (aka cant be resolved)
}

// switches on a pin in 2 cycles
// usage: switchOn<5>();
template<uint8_t pin> inline void switchOn(){
    switchOnInPort<fredDigitalPinToIndex(pin),fredDigitalPinToPort(pin)>();
    static_assert(fredDigitalPinToPort(pin) != 0,"pin doesnt resolve to port");
}

template<uint8_t pinIndex, uint8_t port> inline void switchOffInPort(){
    *((volatile uint8_t*)port) &= ~(1 << pinIndex);
}

template<> __attribute__((always_inline)) inline void switchOffInPort<0,0>(){
    // do nothing if port is 0 (aka cant be resolved)
}

// switches off a pin in 2 cycles
// usage: switchOff<5>();
template<uint8_t pin> __attribute__((always_inline)) inline void switchOff(){
    switchOffInPort<fredDigitalPinToIndex(pin),fredDigitalPinToPort(pin)>();
    static_assert(fredDigitalPinToPort(pin) != 0,"pin doesnt resolve to port");
}

template<uint8_t pinIndex, uint8_t port> __attribute__((always_inline)) inline void switchOnExclusiveInPort(){
    *((volatile uint8_t*)port) = 1 << pinIndex;
}

template<> __attribute__((always_inline)) inline void switchOnExclusiveInPort<0,0>(){
    // do nothing if port is 0 (aka cant be resolved)
}

// switches on a pin in 1 cycles. sets other pins of the port to LOW
// usage: switchOnExclusive<5>();
template<uint8_t pin> __attribute__((always_inline)) inline void switchOnExclusive(){
    switchOnExclusiveInPort<fredDigitalPinToIndex(pin),fredDigitalPinToPort(pin)>();
    static_assert(fredDigitalPinToPort(pin) != 0,"pin doesnt resolve to port");
}

// switches off whole port in 1 cycle
// usage: switchPortOff<(uint8_t)&PORTB>();
template<uint8_t port> __attribute__((always_inline)) inline void switchPortOff(){
    *((volatile uint8_t*)port) = 0;
}

// switches off the port of a pin in 1 cycle. use this if you only use 1 pin of a port
template<uint8_t pin> __attribute__((always_inline)) inline void switchOffPortOfPin(){
    switchPortOff<fredDigitalPinToPort(pin)>();
    static_assert(fredDigitalPinToPort(pin) != 0,"pin doesnt resolve to port");
}

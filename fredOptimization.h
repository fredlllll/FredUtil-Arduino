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

// creates an arbitrary amount of nops
// usage: nop<5>();
template<int count> inline void nop(){
    nop<count - 1>();
    __asm__ __volatile__("nop\n\t");
}

template<> inline void nop<0>(){
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

template<uint8_t pinIndex, uint8_t port> inline void switchOnInPort(){
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

template<> inline void switchOffInPort<0,0>(){
    // do nothing if port is 0 (aka cant be resolved)
}

// switches off a pin in 2 cycles
// usage: switchOff<5>();
template<uint8_t pin> inline void switchOff(){
    switchOffInPort<fredDigitalPinToIndex(pin),fredDigitalPinToPort(pin)>();
    static_assert(fredDigitalPinToPort(pin) != 0,"pin doesnt resolve to port");
}

template<uint8_t pinIndex, uint8_t port> inline void switchOnExclusiveInPort(){
    *((volatile uint8_t*)port) = 1 << pinIndex;
}

template<> inline void switchOnExclusiveInPort<0,0>(){
    // do nothing if port is 0 (aka cant be resolved)
}

// switches on a pin in 1 cycles. sets other pins of the port to LOW
// usage: switchOnExclusive<5>();
template<uint8_t pin> inline void switchOnExclusive(){
    switchOnExclusiveInPort<fredDigitalPinToIndex(pin),fredDigitalPinToPort(pin)>();
    static_assert(fredDigitalPinToPort(pin) != 0,"pin doesnt resolve to port");
}

// switches off whole port in 1 cycle
// usage: switchPortOff<(uint8_t)&PORTB>();
template<uint8_t port> inline void switchPortOff(){
    *((volatile uint8_t*)port) = 0;
}

// switches off the port of a pin in 1 cycle. use this if you only use 1 pin of a port
template<uint8_t pin> inline void switchOffPortOfPin(){
    switchPortOff<fredDigitalPinToPort(pin)>();
    static_assert(fredDigitalPinToPort(pin) != 0,"pin doesnt resolve to port");
}

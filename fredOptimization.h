// this holds code that is associated with optimizations

// this creates an arbitrary amount of nops (not tested with int, so its limited to 255 for now)
// the compiler optimizes this so it will be just nops in the assembly, no overhead
// usage: nop<5>();
// or:
// const uint8_t count = 5; //const is important here
// nop<count>();
template<uint8_t count> inline void nop(){
  nop<count - 1>();
  __asm__ __volatile__("nop\n\t");
}

template<> inline void nop<0>(){
  
}

// this switches on specific pins on PORTD (D0 to D7). each switch takes 2 cycles
template<uint8_t pin> inline void switchOnPortD() {
  PORTD |= 1 << pin;
}

// counterpart to the above, switches off a specific pin on PORTD in 2 cycles
template<uint8_t pin> inline void switchOffPortD() {
  PORTD &= ~(1 << pin);
}

// if you only use one pin of a port, you can use this. it only uses 1 cycle, but sets all other pins of the port to LOW
template<uint8_t pin> inline void switchOnOnly() {
  PORTD = 1 << pin;
}

// switches off the whole PORTD, but only takes 1 cycle
inline void switchPortDOff() {
  PORTD = 0;
}
template<uint8_t exp, typename retType> constexpr retType powerOf10() {
    if (exp == 0) {
        return 1;
    } else {
        return powerOf10<exp - 1, retType>() * 10;
    }
}

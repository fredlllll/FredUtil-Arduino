#ifndef FREDSTRUCTCONFIG_H_
#define FREDSTRUCTCONFIG_H_

template<typename datatype> class StructConfig {
    static const uint8_t configExistsOffset = 0;
    static const uint8_t configDataOffset = 1;

    const int baseAddress;
    const datatype* defaultConfig;
    datatype* config;

public:
    StructConfig(const int baseAddress, const datatype* defaultConfig, datatype* config) :
            baseAddress(baseAddress), defaultConfig(defaultConfig), config(config) {
    }

    //restore the default config
    void restoreDefault() {
        setConfigExists(false);
        *config = *defaultConfig;
    }

    //check if the data in eeprom is valid
    bool getConfigExists() {
        return EEPROM.read(baseAddress + configExistsOffset);
    }

    //sets if the data in eeprom is valid
    void setConfigExists(bool exists) {
        EEPROM.update(baseAddress + configExistsOffset, exists ? 255 : 0);
    }

    //loads config from eeprom, or the default if it doesnt exist yet
    void loadConfig() {
        if (getConfigExists()) {
            EEPROM.get(baseAddress + configDataOffset, *config);
        } else { //if it doesnt exist, lets just load the default
            *config = *defaultConfig;
        }
    }

    //saves config to eeprom and sets exists to true
    void saveConfig() {
        EEPROM.put(baseAddress + configDataOffset, *config);
        setConfigExists(true);
    }
};

#endif /* LIBRARIES_FREDUTIL_FREDSTRUCTCONFIG_H_ */

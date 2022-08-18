#ifndef PCA9633_H
#define PCA9633_H

#include <stdint.h>
#include <stddef.h>
#include <HAL.h>

class pca9633 {

public:
    void begin(uint8_t address, I2CTransfer transferFunc);
    void reset(void);

    void setOutput(uint8_t led, uint8_t state);
    void setBrightness(uint8_t led, uint8_t brightness);
    
private:
    
    uint8_t i2c_address;

    uint8_t output_states;

    I2CTransfer transferFuncPtr;

    void setModeReg(uint8_t modeReg, uint8_t mode);

    int8_t twi_write(uint8_t reg, uint8_t *data, size_t len);
    int8_t twi_read(uint8_t reg, uint8_t *data, size_t len);

};


#endif
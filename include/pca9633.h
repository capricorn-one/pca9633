#ifndef PCA9633_H
#define PCA9633_H

#include <stdint.h>
#include <stddef.h>

typedef int8_t (*pca9633_transfer_func_ptr_t)(uint8_t address, uint8_t reg, uint8_t regLength, void *data, size_t len, uint8_t read);

class pca9633 {

public:
    void begin(uint8_t address, pca9633_transfer_func_ptr_t twi_transfer_func);
    void reset(void);

    void setOutput(uint8_t led, uint8_t state);
    void setBrightness(uint8_t led, uint8_t brightness);
    
private:
    
    uint8_t i2c_address;

    uint8_t output_states;

    pca9633_transfer_func_ptr_t twi_transfer_func_ptr;

    void setModeReg(uint8_t modeReg, uint8_t mode);

    int8_t twi_write(uint8_t reg, void *data, size_t len);
    int8_t twi_read(uint8_t reg, void *data, size_t len);

};


#endif
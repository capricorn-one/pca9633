#ifndef PCA9633_H
#define PCA9633_H

#include <stdint.h>
#include <stddef.h>


typedef struct {
    int8_t (*transfer)(uint8_t address, uint8_t reg, uint8_t *data, size_t len, uint8_t read);
    void (*delay_ms)(uint32_t ms);
} pca9633_hal_t;

class pca9633 {

public:
    void begin(uint8_t address, pca9633_hal_t *pca9633_hal);
    void reset(void);

    void setOutput(uint8_t led, uint8_t state);
    void setBrightness(uint8_t led, uint8_t brightness);
    
private:
    
    uint8_t i2c_address;

    uint8_t output_states;

    void setModeReg(uint8_t modeReg, uint8_t mode);

    pca9633_hal_t *hal;

    int8_t twi_write(uint8_t reg, uint8_t *data, size_t len);
    int8_t twi_read(uint8_t reg, uint8_t *data, size_t len);

};

#endif
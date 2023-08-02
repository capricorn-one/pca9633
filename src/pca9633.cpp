#include "pca9633.h"

#define PCA9633_BASE_ADDRESS        0x60
#define PCA9633_ALLCALL_ADDRESS     0x70
#define PCA9633_RESET_ADDRESS       0x03

#define PCA9633_REG_MODE1           0x00
#define PCA9633_REG_MODE2           0x01
#define PCA9633_REG_PWM0            0x02
#define PCA9633_REG_PWM1            0x03
#define PCA9633_REG_PWM2            0x04
#define PCA9633_REG_PWM3            0x05
#define PCA9633_REG_GRPPWM          0x06
#define PCA9633_REG_GRPFREQ         0x07
#define PCA9633_REG_LEDOUT          0x08
#define PCA9633_REG_SUBADR1         0x09
#define PCA9633_REG_SUBADR2         0x0A
#define PCA9633_REG_SUBADR3         0x0B
#define PCA9633_REG_ALLCALLADD      0x0C

#define PCA9633_MODE1_SLEEP_EN              (1<<4)
#define PCA9633_MODE1_OSC_EN                (0<<4)
#define PCA9633_MODE1_SUB1                  (1<<3)
#define PCA9633_MODE1_SUB2                  (1<<2)
#define PCA9633_MODE1_SUB3                  (1<<1)
#define PCA9633_MODE1_ALLCALL_EN            (1<<0)

#define PCA9633_MODE2_GROUP_DIMMING         (0<<5)
#define PCA9633_MODE2_GROUP_BLINKING        (1<<5)
#define PCA9633_MODE2_INVERT_OUTPUT         (1<<4)
#define PCA9633_MODE2_OCH_ACK               (1<<3)
#define PCA9633_MODE2_OUTDRV_OPENDRAIN      (0<<2)
#define PCA9633_MODE2_OUTNE(X)              (X&0x03)

void pca9633::begin(uint8_t address, pca9633_hal_t *pca9633_hal) {
    
    i2c_address = PCA9633_BASE_ADDRESS + address;

    hal = pca9633_hal;

    output_states = 0x00;

    reset();

    hal->delay_ms(5);

    setModeReg(0, (PCA9633_MODE1_OSC_EN | PCA9633_MODE1_ALLCALL_EN));
    setModeReg(1, (PCA9633_MODE2_OUTDRV_OPENDRAIN | PCA9633_MODE2_OUTNE(1)));
    
}

void pca9633::reset() {
    if(hal) {
        uint8_t resetBytes[2] = {0xA5, 0x5A};
        hal->transfer(PCA9633_RESET_ADDRESS, 0x00, resetBytes, 2, 0);
    }
}

void pca9633::setOutput(uint8_t led, uint8_t state) {
    
    if(led > 3) led = 3;
        
    output_states &= ~(0x03<<(2*led));      // clear current states

    output_states |= (state<<(2*led + 1));

    twi_write( PCA9633_REG_LEDOUT, &output_states, 1);
}

void pca9633::setBrightness(uint8_t led, uint8_t brightness) {
    twi_write( PCA9633_REG_PWM0 + (led % 4), &brightness, 1);
}

void pca9633::setModeReg(uint8_t modeReg, uint8_t mode) {
    modeReg &= 0x01;

    twi_write(PCA9633_REG_MODE1 + modeReg, &mode, 1);
}

int8_t pca9633::twi_write(uint8_t reg, uint8_t *data, size_t len) {
    if(hal)
        return hal->transfer(i2c_address, reg, data, len, 0);
    return -1;
}

int8_t pca9633::twi_read(uint8_t reg, uint8_t *data, size_t len) {
    if(hal)
        return hal->transfer(i2c_address, reg, data, len, 1);
    return -1;
}
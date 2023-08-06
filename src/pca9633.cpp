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


static int8_t pca9633_write(pca9633_hal_t *pca9633_hal, uint8_t reg, uint8_t *data, size_t len) {
    if(pca9633_hal)
        return pca9633_hal->transfer(pca9633_hal->address + PCA9633_BASE_ADDRESS, reg, data, len, 0);
    return -1;
}

static int8_t pca9633_read(pca9633_hal_t *pca9633_hal, uint8_t reg, uint8_t *data, size_t len) {
    if(pca9633_hal)
        return pca9633_hal->transfer(pca9633_hal->address + PCA9633_BASE_ADDRESS, reg, data, len, 1);
    return -1;
}

void pca9633_begin(pca9633_hal_t *pca9633_hal) {
    
    pca9633_hal->output_states = 0x00;

    pca9633_reset(pca9633_hal);

    pca9633_hal->delay_ms(5);

    pca9633_setModeReg(pca9633_hal, 0, (PCA9633_MODE1_OSC_EN | PCA9633_MODE1_ALLCALL_EN));
    pca9633_setModeReg(pca9633_hal, 1, (PCA9633_MODE2_OUTDRV_OPENDRAIN | PCA9633_MODE2_OUTNE(1)));
    
}

void pca9633_reset(pca9633_hal_t *pca9633_hal) {
    if(pca9633_hal) {
        uint8_t resetBytes[2] = {0xA5, 0x5A};
        pca9633_hal->transfer(PCA9633_RESET_ADDRESS, 0x00, resetBytes, 2, 0);
    }
}

void pca9633_setOutput(pca9633_hal_t *pca9633_hal, uint8_t led, uint8_t state) {
    
    if(led > 3) led = 3;
        
    pca9633_hal->output_states &= ~(0x03<<(2*led));      // clear current states

    pca9633_hal->output_states |= (state<<(2*led + 1));

    pca9633_write(pca9633_hal, PCA9633_REG_LEDOUT, &pca9633_hal->output_states, 1);
}

void pca9633_setBrightness(pca9633_hal_t *pca9633_hal, uint8_t led, uint8_t brightness) {
    pca9633_write(pca9633_hal, PCA9633_REG_PWM0 + (led % 4), &brightness, 1);
}

void pca9633_setModeReg(pca9633_hal_t *pca9633_hal, uint8_t modeReg, uint8_t mode) {
    modeReg &= 0x01;

    pca9633_write(pca9633_hal, PCA9633_REG_MODE1 + modeReg, &mode, 1);
}


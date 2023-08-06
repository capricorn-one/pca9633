#ifndef PCA9633_H
#define PCA9633_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int8_t (*transfer)(uint8_t address, uint8_t reg, uint8_t *data, size_t len, uint8_t read);
    void (*delay_ms)(uint32_t ms);
    uint8_t address;
    uint8_t output_states;
} pca9633_hal_t;

    void pca9633_begin(pca9633_hal_t *pca9633_hal);

    void pca9633_reset(pca9633_hal_t *pca9633_hal);

    void pca9633_setOutput(pca9633_hal_t *pca9633_hal, uint8_t led, uint8_t state);

    void pca9633_setBrightness(pca9633_hal_t *pca9633_hal, uint8_t led, uint8_t brightness);
    
    void pca9633_setModeReg(pca9633_hal_t *pca9633_hal, uint8_t modeReg, uint8_t mode);


#ifdef __cplusplus
}
#endif

#endif
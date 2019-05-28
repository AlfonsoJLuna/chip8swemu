#include "input.h"
#include <stm32f1xx_hal.h>


static bool input_ready = 0;

static void inputInit()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef config_gpio = {0};

    config_gpio.Pull  = GPIO_NOPULL;
    config_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    config_gpio.Mode  = GPIO_MODE_INPUT;
    config_gpio.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
        GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 |
        GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
        GPIO_PIN_14 | GPIO_PIN_15;

    HAL_GPIO_Init(GPIOB, &config_gpio);

    input_ready = 1;
}

bool inputRead(uint8_t key)
{
    if (!input_ready)
    {
        inputInit();
    }

    bool status = 0;

    if (input_ready)
    {
        switch (key)
        {
            case 0x0: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET; break;
            case 0x1: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET; break;
            case 0x2: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET; break;
            case 0x3: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET; break;
            case 0x4: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_RESET; break;
            case 0x5: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_RESET; break;
            case 0x6: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_RESET; break;
            case 0x7: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_RESET; break;
            case 0x8: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET; break;
            case 0x9: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_RESET; break;
            case 0xA: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET; break;
            case 0xB: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_RESET; break;
            case 0xC: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET; break;
            case 0xD: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_RESET; break;
            case 0xE: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET; break;
            case 0xF: status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET; break;
            default: break;
        }
    }

    return status;
}

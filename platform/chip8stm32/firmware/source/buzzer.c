#include "buzzer.h"
#include <stdbool.h>
#include <stm32f1xx_hal.h>


// Buzzer -> PA1 (TIM2_CH2)

#define SYS_CLK 72000000    // TIMxCLK Clock supplied to TIM2 in Hz
#define PWM_CLK 500         // Desired PWM frequency in Hz
#define PWM_RES 254         // Duty Cycle resolution

static TIM_HandleTypeDef config;
static TIM_OC_InitTypeDef config_oc;
static bool buzzer_ready = 0;


static bool configureTimer()
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    config = (TIM_HandleTypeDef){0};

    config.Instance           = TIM2;
    config.Init.Prescaler     = (uint32_t)(SYS_CLK / (PWM_CLK * PWM_RES)) - 1;
    config.Init.CounterMode   = TIM_COUNTERMODE_UP;
    config.Init.Period        = PWM_RES;
    config.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    if (HAL_TIM_PWM_Init(&config) != HAL_OK)
    {
        return 1;
    }

    return 0;
}


static bool configurePWM()
{
    config_oc = (TIM_OC_InitTypeDef){0};

    config_oc.Pulse        = 0;
    config_oc.OCMode       = TIM_OCMODE_PWM1;
    config_oc.OCPolarity   = TIM_OCPOLARITY_HIGH;
    config_oc.OCFastMode   = TIM_OCFAST_DISABLE;
    config_oc.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    config_oc.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    config_oc.OCIdleState  = TIM_OCIDLESTATE_RESET;

    if (HAL_TIM_PWM_ConfigChannel(&config, &config_oc, TIM_CHANNEL_2) != HAL_OK)
    {
        return 1;
    }

    return 0;
}


static void configureGPIO()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef config_gpio = {0};

    config_gpio.Mode  = GPIO_MODE_AF_PP;
    config_gpio.Pull  = GPIO_NOPULL;
    config_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    config_gpio.Pin   = GPIO_PIN_1;

    HAL_GPIO_Init(GPIOA, &config_gpio);
}


static bool buzzerInit()
{
    if (configureTimer())
    {
        return 1;
    }

    if (configurePWM())
    {
        return 1;
    }

    configureGPIO();

    buzzer_ready = 1;

    return 0;
}


void buzzerBeep()
{
    if (!buzzer_ready)
    {
        buzzerInit();
    }

    if (buzzer_ready)
    {
        HAL_TIM_PWM_Stop(&config, TIM_CHANNEL_2);
        config_oc.Pulse = 127;
        HAL_TIM_PWM_ConfigChannel(&config, &config_oc, TIM_CHANNEL_2);
        HAL_TIM_PWM_Start(&config, TIM_CHANNEL_2);
    }
}

void buzzerSilent()
{
    if (!buzzer_ready)
    {
        buzzerInit();
    }

    if (buzzer_ready)
    {
        HAL_TIM_PWM_Stop(&config, TIM_CHANNEL_2);
        config_oc.Pulse = 0;
        HAL_TIM_PWM_ConfigChannel(&config, &config_oc, TIM_CHANNEL_2);
        HAL_TIM_PWM_Start(&config, TIM_CHANNEL_2);
    }
}

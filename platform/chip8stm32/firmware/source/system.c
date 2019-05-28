#include "system.h"
#include <stm32f1xx_hal.h>


static int configureClocks()
{
    RCC_OscInitTypeDef config_osc = {0};

    config_osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    config_osc.HSEState       = RCC_HSE_ON;
    config_osc.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    config_osc.PLL.PLLState   = RCC_PLL_ON;
    config_osc.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    config_osc.PLL.PLLMUL     = RCC_PLL_MUL9;

    if (HAL_RCC_OscConfig(&config_osc) != HAL_OK)
        return 1;

    RCC_ClkInitTypeDef config_clk = {0};

    config_clk.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    config_clk.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    config_clk.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    config_clk.APB2CLKDivider = RCC_HCLK_DIV1;
    config_clk.APB1CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&config_clk, FLASH_LATENCY_2) != HAL_OK)
        return 1;

    return 0;
}


int systemInit()
{
    HAL_Init();

    if (configureClocks())
        return 1;

    return 0;
}


void systemDelay(uint32_t time_ms)
{
    HAL_Delay(time_ms);
}


uint32_t systemMillis()
{
    return HAL_GetTick();
}

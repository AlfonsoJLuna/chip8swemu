#include "lcd.h"
#include "stm32h7xx_hal.h"
#include "main.h"

uint16_t framebuffer[320 * 240];

void lcd_backlight_off() {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
}
void lcd_backlight_on() {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
}

void lcd_init(SPI_HandleTypeDef *spi, LTDC_HandleTypeDef *ltdc) {

  // Turn display *off* completely.
  lcd_backlight_off();

  // 3.3v power to display *SET* to disable supply.
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);


  // TURN OFF CHIP SELECT
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  // TURN OFF PD8
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);

  // Turn off CS
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_Delay(100);

  lcd_backlight_on();


// Wake
// Enable 3.3v
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
  HAL_Delay(1);
  // Enable 1.8V
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
  // also assert CS, not sure where to put this yet
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  HAL_Delay(7);



// HAL_SPI_Transmit(spi, "\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55", 10, 100);
  // Lets go, bootup sequence.
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);

  HAL_Delay(10);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_SPI_Transmit(spi, "\x08\x80", 2, 100);
  HAL_Delay(2);
  
  // CS
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  // HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_SPI_Transmit(spi, "\x6E\x80", 2, 100);
  HAL_Delay(2);
  // CS
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  // HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_SPI_Transmit(spi, "\x80\x80", 2, 100);
  
  HAL_Delay(2);
  // CS
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  // HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_SPI_Transmit(spi, "\x68\x00", 2, 100);
  HAL_Delay(2);
  // CS
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  // HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_SPI_Transmit(spi, "\xd0\x00", 2, 100);
  HAL_Delay(2);
  // CS
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  // HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_SPI_Transmit(spi, "\x1b\x00", 2, 100);
  
  HAL_Delay(2);
  // CS
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  // HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_SPI_Transmit(spi, "\xe0\x00", 2, 100);
  
  
  HAL_Delay(2);
  // CS
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  // HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_SPI_Transmit(spi, "\x6a\x80", 2, 100);
  
  HAL_Delay(2);
  // CS
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  // HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_SPI_Transmit(spi, "\x80\x00", 2, 100);
  HAL_Delay(2);
  // CS
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  // HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_SPI_Transmit(spi, "\x14\x80", 2, 100);
  HAL_Delay(2);
  // CS
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);


  HAL_LTDC_SetAddress(ltdc,(uint32_t) &framebuffer,0);
}
#include "display.h"
#include <stdbool.h>
#include <stm32f1xx_hal.h>


// Dimensions
#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64

// Commands from https://github.com/adafruit/Adafruit_SSD1306
// Most of them also work with SSH1106 display
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

#define SSD1306_PAGESTARTADDRESS 0xB0

// Scrolling
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A


static SPI_HandleTypeDef config;
static bool display_ready = 0;
static uint8_t buffer[1024];


static void configureGPIO()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef config_gpio = {0};

    // PA2. DISPLAY_RESET
    config_gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    config_gpio.Pull  = GPIO_NOPULL;
    config_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    config_gpio.Pin   = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOA, &config_gpio);

    // PA3. DISPLAY_DC
    config_gpio.Pin   = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &config_gpio);

    // PA4. DISPLAY_CS
    config_gpio.Pin   = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOA, &config_gpio);

    // PA5. DISPLAY_SCK
    config_gpio.Mode  = GPIO_MODE_AF_PP;
    config_gpio.Pull  = GPIO_PULLDOWN;
    config_gpio.Pin   = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOA, &config_gpio);

    // PA7. DISPLAY_MOSI
    config_gpio.Pin   = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &config_gpio);

    // CS always enabled
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}


static bool configureSPI()
{
    __HAL_RCC_SPI1_CLK_ENABLE();

    config = (SPI_HandleTypeDef){0};

    config.Instance               = SPI1;
    config.Init.Mode              = SPI_MODE_MASTER;
    config.Init.Direction         = SPI_DIRECTION_1LINE;
    config.Init.DataSize          = SPI_DATASIZE_8BIT;
    config.Init.CLKPolarity       = SPI_POLARITY_LOW;
    config.Init.CLKPhase          = SPI_PHASE_1EDGE;
    config.Init.NSS               = SPI_NSS_SOFT;
    config.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2; // 36/2 = 18MHz SCK
    config.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    config.Init.TIMode            = SPI_TIMODE_DISABLE;
    config.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    config.Init.CRCPolynomial     = 7;

    if (HAL_SPI_Init(&config) != HAL_OK)
    {
        return 1;
    }

    return 0;
}


static bool sendCommand(uint8_t command)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

    if (HAL_SPI_Transmit(&config, &command, 1, 2000) != HAL_OK)
    {
        return 1;
    }

    return 0;
}


static bool sendData(uint8_t data)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);

    if (HAL_SPI_Transmit(&config, &data, 1, 2000) != HAL_OK)
    {
        return 1;
    }

    return 0;
}


static bool sendInitCommands()
{
    bool error = 0;

    error |= sendCommand(SSD1306_DISPLAYOFF);            // 0xAE
    error |= sendCommand(SSD1306_SETDISPLAYCLOCKDIV);    // 0xD5
    error |= sendCommand(0x80);                          // the suggested ratio
    error |= sendCommand(SSD1306_SETMULTIPLEX);          // 0xA8
    error |= sendCommand(SSD1306_LCDHEIGHT - 1);
    error |= sendCommand(SSD1306_SETDISPLAYOFFSET);      // 0xD3
    error |= sendCommand(0x0);                           // no offset
    error |= sendCommand(SSD1306_SETSTARTLINE | 0x0);    // line #0
    error |= sendCommand(SSD1306_CHARGEPUMP);            // 0x8D
    error |= sendCommand(0x14);
    error |= sendCommand(SSD1306_MEMORYMODE);            // 0x20
    error |= sendCommand(0x00);                          // 0x0 act like ks0108
    error |= sendCommand(SSD1306_SEGREMAP | 0x1);
    error |= sendCommand(SSD1306_COMSCANDEC);
    error |= sendCommand(SSD1306_SETCOMPINS);            // 0xDA
    error |= sendCommand(0x12);
    error |= sendCommand(SSD1306_SETCONTRAST);           // 0x81
    error |= sendCommand(0xCF);
    error |= sendCommand(SSD1306_SETPRECHARGE);          // 0xd9
    error |= sendCommand(0xF1);
    error |= sendCommand(SSD1306_SETVCOMDETECT);         // 0xDB
    error |= sendCommand(0x40);
    error |= sendCommand(SSD1306_DISPLAYALLON_RESUME);   // 0xA4
    error |= sendCommand(SSD1306_NORMALDISPLAY);         // 0xA6
    error |= sendCommand(SSD1306_DEACTIVATE_SCROLL);
    error |= sendCommand(SSD1306_DISPLAYON);          // turn on oled panel

    return error;
}


static bool displayInit()
{
    configureGPIO();

    if (configureSPI())
    {
        return 1;
    }

    // Keep the DISPLAY_RESET pin LOW during 10ms
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

    if (sendInitCommands())
    {
        return 1;
    }

    display_ready = 1;

    return 0;
}


static inline void setPixel(int x, int y)
{
    buffer[x + (y/8)*128] |=  (1 << (y&7));
}


static inline void clearPixel(int x, int y)
{
    buffer[x + (y/8)*128] &= ~(1 << (y&7));
}


static inline bool getPixel(uint8_t* chip8_buffer, int row, int col)
{
    return (chip8_buffer[(128 * row + col) / 8] >> (7 - (col % 8))) & 1;
}


void displayRefresh(uint8_t* chip8_buffer)
{
    if (!display_ready)
    {
        displayInit();
    }

    if (display_ready)
    {
        // Convert from chip-8 buffer format to display buffer format (this needs rework)
        for (int row = 0; row < 64; row++)
        {
            for (int col = 0; col < 128; col++)
            {
                if (getPixel(chip8_buffer, row, col))
                {
                    setPixel(col, row);
                }
                else
                {
                    clearPixel(col, row);
                }
            }
        }

        // Send display buffer
        for (uint8_t page = 0; page < 64 / 8; page++)
        {
            sendCommand(SSD1306_PAGESTARTADDRESS + page);
            for (uint8_t col = 2; col < 130; col++)
            {
                sendCommand(col & 0xF);
                sendCommand(SSD1306_SETHIGHCOLUMN | (col >> 4));
                sendData(buffer[col - 2 + page * 128]);
            }
        }
    }
}

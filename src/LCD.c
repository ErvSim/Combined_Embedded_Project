#include "LCD.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include <stdlib.h>

#define SDA 16
#define SCL 17
#define I2C_PORT i2c0
#define LCD_ADDR 0x27
// Bitmask definitions for LCD control lines connected via PCF8574
#define LCD_RS 0x01        // Register Select (0 = command, 1 = data)
#define LCD_RW 0x02        // Read/Write (0 = write, 1 = read) [we always write for LCD]
#define LCD_ENABLE 0x04    // Enable bit (used to latch data)
#define LCD_BACKLIGHT 0x08 // Backlight control

void pcf_write_byte(uint8_t data);
void lcd_send_command(uint8_t cmd);
void lcd_init(void);
void lcd_send_char(char c);
void lcd_send_string(const char *str);
void lcd_display_distance(float distance_value);
void lcd_backlight_off(void);

void lcd_init(void)
{
    sleep_ms(50); // Wait for LCD power-up

    // Send 0x30 three times to reset LCD into 8-bit mode first
    for (int i = 0; i < 3; i++)
    {
        pcf_write_byte(0x30 | LCD_BACKLIGHT | LCD_ENABLE);
        pcf_write_byte(0x30 | LCD_BACKLIGHT);
        sleep_ms(5);
    }

    // Switch to 4-bit mode
    pcf_write_byte(0x20 | LCD_BACKLIGHT | LCD_ENABLE);
    pcf_write_byte(0x20 | LCD_BACKLIGHT);
    sleep_ms(5);

    // Function set: 4-bit mode, 2-line display, 5x8 dots
    lcd_send_command(0x28);

    // Display ON, cursor OFF, blink OFF
    lcd_send_command(0x0C);

    // Clear display
    lcd_send_command(0x01);

    // Entry mode set: move cursor to right, no display shift
    lcd_send_command(0x06);

    sleep_us(1000); // Short delay after init
}

void pcf_write_byte(uint8_t data)
{
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &data, 1, false);
    sleep_us(100);
}

void lcd_send_command(uint8_t cmd)
{
    uint8_t high = (cmd & 0xF0);     // Get high nibble (upper 4 bits)
    uint8_t low = (cmd << 4) & 0xF0; // Get low nibble (lower 4 bits shifted to upper position)

    // Send high nibble with Enable pulse
    pcf_write_byte(high | LCD_BACKLIGHT | LCD_ENABLE);
    pcf_write_byte(high | LCD_BACKLIGHT);

    // Send low nibble with Enable pulse
    pcf_write_byte(low | LCD_BACKLIGHT | LCD_ENABLE);
    pcf_write_byte(low | LCD_BACKLIGHT);
}


// Sends a single character to the LCD (with RS = 1)
void lcd_send_char(char c)
{
    uint8_t high = (c & 0xF0);     // Get high nibble
    uint8_t low = (c << 4) & 0xF0; // Get low nibble

    // Send high nibble with RS set (data mode)
    pcf_write_byte(high | LCD_BACKLIGHT | LCD_ENABLE | LCD_RS);
    pcf_write_byte(high | LCD_BACKLIGHT | LCD_RS);

    // Send low nibble with RS set (data mode)
    pcf_write_byte(low | LCD_BACKLIGHT | LCD_ENABLE | LCD_RS);
    pcf_write_byte(low | LCD_BACKLIGHT | LCD_RS);
}
void lcd_send_string(const char *str)
{
    int counter = 0;

    while (*str != '\0')
    {

        if (*str >= 32 && *str <= 126)
        {
            lcd_send_char(*str);
        }
        else
        {
            lcd_send_char(' ');
        }

        counter++;
        str++;
    }
}
void lcd_display_distance(float distance_value)
{
    char buffer[20];

    lcd_send_command(0x01); // Clear display
    sleep_ms(2);            // Wait for clear to complete

    lcd_send_command(0x80); // Set cursor to beginning of first line
    lcd_send_string("The distance is:");

    lcd_send_command(0xC0); // Set cursor to beginning of second line
    sprintf(buffer, "%.2f inches", distance_value);
    lcd_send_string(buffer);
}

void lcd_backlight_off(void)
{
    // Send "display off" command and remove the backlight bit manually if needed
    lcd_send_command(0x01); // Clear
    lcd_send_command(0x08); // Display OFF
    i2c_write_blocking(I2C_PORT, LCD_ADDR, (uint8_t[]){ 0x00 }, 1, false);
}

# CYD_Text_Display
Text display program using the ESP32-2432S028R. This is a non-standard CYD component using a USB-C port.

# GPIO Pin Table (so far)

| Pin | Name/Function | Notes                           |
| --- | ------------- | ------------------------------- |
| 2   | TFT_DC        |                                 |
| 9   |               |                                 |
| 12  | TFT_MISO      | XPT2046_MISO                    |
| 13  | TFT_MOSI      | XPT2046_MOSI                    |
| 14  | TFT SCLK      | XPT2046_CLK                     |
| 15  | TFT_CS        |                                 |
| 33  | XPT2046_CS    | RGB Red?<br>                    |
| 36  | XPT2046_IRQ   |                                 |
| 27  | TFT_BL        | Backlight set to high in script |
| 16  | RGB?          |                                 |
| 17  | RGB?          |                                 |
| 5   | SD_CS         |                                 |
| 18  | SD_CLK        | SD_SCK                          |
| 19  | SD_MISO       |                                 |
| 23  | SD_MOSI       |                                 |


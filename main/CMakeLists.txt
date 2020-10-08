idf_component_register(SRCS "test_SSD1306_i2c.c" "test_SSD1306.c" "main.c" 
"crypto_atl.c" 
"u8g2_atl.c"
"u8g2_esp32_hal.c"



                    INCLUDE_DIRS "." 
                    # ../components/lcdgfx/src
                    REQUIRES atlast spiffs console vfs fatfs nvs_flash spi_flash crypto u8g2)
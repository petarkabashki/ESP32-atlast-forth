idf_component_register(SRCS "main.c" 

"u8g2_esp32_hal.c"



                    INCLUDE_DIRS "." 
                    REQUIRES atlast spiffs console vfs fatfs nvs_flash spi_flash crypto u8g2)
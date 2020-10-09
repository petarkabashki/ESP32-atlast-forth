/* SPIFFS filesystem example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"

#include "esp_vfs_fat.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "atlast.h"
#include "crypto_atl.h"
#include "u8g2_atl.h"
#include "system_atl.h"
#include "nvs_atl.h"
#include "twofish2_atl.h"

#include <u8g2.h>

#include "sdkconfig.h"
#include "u8g2_esp32_hal.h"

// SDA - GPIO21
#define PIN_SDA 21

// SCL - GPIO22
#define PIN_SCL 22

static const char *TAG = "ssd1306";
#define PROMPT_STR ":>"

static void initialize_nvs(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void initialize_spiffs()
{

    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    }
    else
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
}

u8g2_t u8g2;

void task_SSD1306i2c(void *ignore)
{
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
    u8g2_esp32_hal.sda = PIN_SDA;
    u8g2_esp32_hal.scl = PIN_SCL;
    u8g2_esp32_hal_init(u8g2_esp32_hal);

    // u8g2_t u8g2; // a structure which will contain all the data for one display
    u8g2_Setup_ssd1306_i2c_128x32_univision_f(
        &u8g2,
        U8G2_R0,
        //u8x8_byte_sw_i2c,
        u8g2_esp32_i2c_byte_cb,
        u8g2_esp32_gpio_and_delay_cb); // init u8g2 structure
    u8x8_SetI2CAddress(&u8g2.u8x8, 0x78);

    ESP_LOGI(TAG, "u8g2_InitDisplay");
    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,

    ESP_LOGI(TAG, "u8g2_SetPowerSave");
    u8g2_SetPowerSave(&u8g2, 0); // wake up display
    ESP_LOGI(TAG, "u8g2_ClearBuffer");
    u8g2_ClearBuffer(&u8g2);
    ESP_LOGI(TAG, "u8g2_DrawBox");
    u8g2_DrawBox(&u8g2, 0, 26, 80, 6);
    u8g2_DrawFrame(&u8g2, 0, 26, 100, 6);

    ESP_LOGI(TAG, "u8g2_SetFont");
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
    ESP_LOGI(TAG, "u8g2_DrawStr");
    u8g2_DrawStr(&u8g2, 2, 17, "Hi Maraban!");
    ESP_LOGI(TAG, "u8g2_SendBuffer");
    u8g2_SendBuffer(&u8g2);

    ESP_LOGI(TAG, "All done!");

    // vTaskDelete(NULL);
}

// #define BUF_SIZE (1024)

/*

: text oled.clear 0 25 rot oled.drawstr oled.send ;

*/

static void initialize_console(void)
{
    /* Drain stdout before reconfiguring it */
    fflush(stdout);
    fsync(fileno(stdout));

    /* Disable buffering on stdin */
    setvbuf(stdin, NULL, _IONBF, 0);

    /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);

    /* Configure UART. Note that REF_TICK is used so that the baud rate remains
     * correct while APB frequency is changing in light sleep mode.
     */
    const uart_config_t uart_config = {
        .baud_rate = CONFIG_ESP_CONSOLE_UART_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .source_clk = UART_SCLK_REF_TICK,
    };
    /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM,
                                        256, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(CONFIG_ESP_CONSOLE_UART_NUM, &uart_config));

    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);

    /* Initialize the console */
    esp_console_config_t console_config = {
        .max_cmdline_args = 8,
        .max_cmdline_length = 256,
#if CONFIG_LOG_COLORS
        .hint_color = atoi(LOG_COLOR_CYAN)
#endif
    };
    ESP_ERROR_CHECK(esp_console_init(&console_config));

    /* Configure linenoise line completion library */
    /* Enable multiline editing. If not set, long commands will scroll within
     * single line.
     */
    // linenoiseSetMultiLine(1);

    // /* Tell linenoise where to get command completions and hints */
    // linenoiseSetCompletionCallback(&esp_console_get_completion);
    // linenoiseSetHintsCallback((linenoiseHintsCallback *)&esp_console_get_hint);

    // /* Set command history size */
    // linenoiseHistorySetMaxLen(100);

    /* Don't return empty lines */
    linenoiseAllowEmpty(false);

    // #if CONFIG_STORE_HISTORY
    //     /* Load command history from filesystem */
    //     linenoiseHistoryLoad(HISTORY_PATH);
    // #endif
}

extern void app_main(void)
{
    initialize_nvs();
    initialize_spiffs();
    initialize_console();

    atl_init();
    atl_primdef(crypto_fcns);
    atl_primdef(u8g2_fcns);
    atl_primdef(system_fcns);
    atl_primdef(nvs_fcns);
    // // atl_primdef(twofish2_fcns);

    // task_SSD1306i2c(NULL);
    // xTaskCreate(task_SSD1306i2c, "task_SSD1306i2c", 512, NULL, 10, NULL);
    const char *prompt = LOG_COLOR_I PROMPT_STR LOG_RESET_COLOR;

    int probe_status = linenoiseProbe();
    if (probe_status)
    { /* zero indicates success */
        printf("\n"
               "Your terminal application does not support escape sequences.\n"
               "Line editing and history features are disabled.\n"
               "On Windows, try using Putty instead.\n");
        linenoiseSetDumbMode(1);
        const char *prompt = LOG_COLOR_I PROMPT_STR LOG_RESET_COLOR;
    }

    /* Main loop */
    while (true)
    {
        fflush(stdout);
        fsync(fileno(stdout));
        vTaskDelay(10);

        /* Get a line using linenoise.
         * The line is returned when ENTER is pressed.
         */
        char *line = linenoise(prompt);
        if (line == NULL)
        { /* Break on EOF or error */
            continue;
        }
        /* Add the command to the history if not empty*/
        if (strlen(line) > 0)
        {
            printf("\n");
            int evalstat = atl_eval(line);
            if (evalstat != 0)
                printf("Atlass evalstat: %i\n", evalstat);
            linenoiseFree(line);
        }
    }
}

// static void atlast_task(void *arg)
// {

//     printf(":>");

//     uint8_t *data = (uint8_t *)malloc(BUF_SIZE);

//     int p = 0, cl = 0;
//     while (1)
//     {

//         // Read data from the UART
//         int l = uart_read_bytes(UART_NUM_0, data + cl, BUF_SIZE, 20 / portTICK_RATE_MS);
//         // Write data back to the UART
//         if (l > 0)
//         {
//             printf("received %i, total %i \n", l, cl);
//             printf("all data: %s\n", data);
//             printf("new data: %s\n", data + cl);

//             uart_write_bytes(UART_NUM_0, data + cl, l);
//             cl += l;
//         }
//         p = 0;
//         while (p < cl)
//         {

//             // printf("stripping p: %i, l: %i, cl: %i \n", p, l, cl);
//             if ((data[p] == '\n') || (data[p] == '\r'))
//             {
//                 int offs = 1;
//                 while ((data[offs] == '\n') || (data[offs] == '\r'))
//                     offs++;

//                 data[p] = 0;
//                 fflush(stdout);
//                 printf("evaluating: %s \n", data);
//                 atl_eval((char *)data);
//                 atl_eval("\n");

//                 for (int j = p + offs; j < cl; j++)
//                     data[j - (p + offs)] = data[j];

//                 printf(":>");
//                 fflush(stdout);
//                 // printf("\n==>");
//                 cl -= (offs + p);
//                 break;
//             }
//             // printf("after eval %i, total %i \n", l, cl);
//             p++;
//         }
//         vTaskDelay(10);
//         while (true)
//         {
//             /* Get a line using linenoise.
//          * The line is returned when ENTER is pressed.
//          */
//             char *line = linenoise(":>");
//             if (line == NULL)
//             { /* Ignore empty lines */
//                 vTaskDelay(10);
//                 continue;
//             }
//             // SSD1306_DrawText(10, 10, line, 1);
//             // SSD1306_Display();
//         }
//     }
// }
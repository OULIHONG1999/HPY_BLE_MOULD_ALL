#include "system/includes.h"
#include "sensor_cfg.h"
#include "led_sensor.h"

#if SINGLE_LED

#define LED_PORT IO_PORTB_06
#define LED_DATA_LEN 3
static u8 led_data[LED_DATA_LEN] = {0};
static u8 led_state = 0;

void init_sensor_device()
{
    gpio_set_die(LED_PORT, 1);
    gpio_set_pull_up(LED_PORT, 0);
    gpio_set_pull_down(LED_PORT, 1);
    gpio_set_direction(LED_PORT, 0);
    gpio_write(LED_PORT, 0);

    led_data[0] = 0xab;
    led_data[1] = 0x28;
    led_data[2] = led_state;

}

static void set_led_state(u8 value)
{
    gpio_write(LED_PORT, value);
}

// 将得到的数据进行更新
void updata_sensor_data(u8 **data, u8 *len)
{
    led_data[2] = led_state;
    *len = LED_DATA_LEN;
    *data = led_data;
    // put_buf(led_data,LED_DATA_LEN);
    // memcpy(data, led_data, LED_DATA_LEN);
}

/// @brief 处理来自主控方向的数据命令
/// @param data
/// @param len
void receive_master_data(u8 *data, u8 *len)
{
    // put_buf(data, *len);
    if (data[0] == 0xab)
    {
        // head   ID    command
        // 0XAB   0xff  0x00

        printf("get command %d",data[3]);
        led_state = data[2];
        set_led_state(led_state);
    }
}

#endif
#include "system/includes.h"
#include "sensor_cfg.h"
#include "ble.h"
#include "sensor_include.h"
#include "sensor_main.h"
#include "led_blink.h"
#include "key.h"

#define led_port IO_PORTB_00

typedef struct
{
    u8 *sensor_data;
    u8 sensor_data_len;
    // 收到数据的回调函数
    void (*recv)(u8 *, u8 *);
    // 发送数据的回调函数
    void (*updata)(u8 *, u8 *);
    void (*init_sensor)();
} Sensor_s;

// 初始化Sensor_s结构体
static Sensor_s sensor = {
    .sensor_data = NULL,
    .sensor_data_len = 0,
    .updata = updata_sensor_data,
    .recv = receive_master_data,
    .init_sensor = init_sensor_device,
};

void send_data_timer()
{
    sensor.updata(&sensor.sensor_data, &sensor.sensor_data_len);
    ble_send_data(sensor.sensor_data, sensor.sensor_data_len);
    put_buf(sensor.sensor_data, sensor.sensor_data_len);
}

void ble_data_callback(u8 *data, u8 len)
{
    sensor.recv(data, &len);
}

void ble_work_state_check_timer()
{
    u8 state = ble_gatt_server_get_work_state();
    // printf("ble_work_state_check_timer: %d\n", state);
    if (state == 2)
        set_led_state(led_port, LED_KEEP_ON);
    else
        set_led_state(led_port, LED_BLINK);
}

// 三击断开连接
void disconnect_master_callback()
{
    printf("disconnect device\n");
    // 断开连接
    trans_disconnect();
}

// 长按关机
void close_device_callback()
{
    printf("关机。。。\n");
    // 关机
    extern void spple_set_soft_poweroff(void);
    spple_set_soft_poweroff();
}

void init_led()
{
    add_led(led_port, LED_BLINK_SLOW, LED_BLINK);
    sys_hi_timer_add(NULL, led_blink_timer, 10);
}

void init_key()
{
    key_register_callback(TRIPLE_CLICK, disconnect_master_callback);
    key_register_callback(LONG_PRESS, close_device_callback);
}

void init_sensor()
{
    sensor.init_sensor();
}

void sensor_main(void *p)
{
    init_led();
    init_key();
    init_sensor();
    sys_hi_timer_add(NULL, ble_work_state_check_timer, 100);
    sys_hi_timer_add(NULL, send_data_timer, 1000);

    while (1)
    {
        clr_wdt();
        os_time_dly(1);
    }
}
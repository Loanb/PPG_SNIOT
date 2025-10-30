#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <app_version.h>
#include <app/drivers/sensors/max30102.h>
#include <stdint.h>
#include "global.h"
#define MY_STACK_SIZE 500
#define MY_PRIORITY 5
void start_acquisition(const struct device *max30102);
void configuration_led (const struct device *max30102);
LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

/**
 * Handles MAX30102 hardware interrupt
 */
static void trigger_handler(const struct device *max30102, const struct sensor_trigger *trigger)
{
    switch (trigger->type)
    {
    case SENSOR_TRIG_DATA_READY:
        if (sensor_sample_fetch(max30102))
        {
            LOG_ERR("Sample fetch error\n");
            return;
        }
        LOG_INF("Fetched sample\n");
        struct sensor_value red;
        struct sensor_value ir;
        sensor_channel_get(max30102, SENSOR_CHAN_RED, &red);
        sensor_channel_get(max30102, SENSOR_CHAN_IR, &ir);
        struct ppg_sample sample = {
            .ir = ir.val1,
            .red = red.val1};
            printk("%d \n", ir.val1);
        break;
    default:
        LOG_ERR("Unknown trigger\n");
    }
}

/**
 *  Configure the MAX30102 hardware trigger interrupt
 */
static void drdy_trigger_mode(const struct device *max30102)
{
    printk("Ready to perform acquisition !") ;
    struct sensor_trigger trig = {
        .type = SENSOR_TRIG_DATA_READY,
        .chan = SENSOR_CHAN_ALL,
    };

    if (sensor_trigger_set(max30102, &trig, trigger_handler))
    {
        LOG_ERR("Could not set trigger\n");
        return;
    }
    LOG_INF("Set trigger handler\n");
}

void start_acquisition(const struct device *max30102) {
    const struct sensor_value acq_val = {.val1 = 1, .val2 = 0} ;
        if (sensor_attr_set(max30102, SENSOR_CHAN_ALL, (enum
            sensor_attribute)MAX30102_ATTR_ACQUISITION, &acq_val) != 0)
    {
        printk("Failed to start sensor !\n") ;
        return ;
    }

    drdy_trigger_mode(max30102) ;

    printk("Ready to perform acquisition !\n") ;
}

void configuration_led (const struct device *max30102){
    struct sensor_value pa_val ; 
    pa_val.val1 = 50 ;
    pa_val.val2 = 0 ;

    if (sensor_attr_set(max30102, SENSOR_CHAN_RED,SENSOR_ATTR_PRIV_START, &pa_val) != 0)
    {
        LOG_ERR("Failed to set RED LED pulse amplitude.") ;
    }
    else
    {
        LOG_INF("Set RED LED PA to %d", pa_val.val1) ;
    }
    if (sensor_attr_set(max30102, SENSOR_CHAN_IR,SENSOR_ATTR_PRIV_START, &pa_val) != 0)
    {
        LOG_ERR("Failed to set IR LED pulse amplitude.") ;
    }
    else
    {
        LOG_INF("Set IR LED PA to %d", pa_val.val1) ;
    }
}

int main(void)
{
    printk("Starting App ! %s\n", CONFIG_BOARD_TARGET);
    const struct device *max30102 = DEVICE_DT_GET_ANY(maxim_max30102);
    start_acquisition(max30102);
    configuration_led(max30102);
    while (1) {
        k_sleep(K_SECONDS(1));
        //printk("Hello World\n");
    }
}


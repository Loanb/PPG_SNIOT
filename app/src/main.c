#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <app_version.h>
#include <app/drivers/sensors/max30102.h>
#include <stdint.h>
#include "global.h"

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

int main(void)
{
    printk("Starting App ! %s\n", CONFIG_BOARD_TARGET);

    while (1) {
        k_sleep(K_SECONDS(1));
    }
}

#ifndef __TASK_COMMON__
#define __TASK_COMMON__
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>


void led_task(void *args, void *arg1, void *arg2);
int display_task(void *args, void *arg1, void *arg2);
#define CURRENT_NAME() k_thread_name_get(k_current_get())

#endif //__TASK_COMMON__
#include "tasks_common.h"
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

LOG_MODULE_REGISTER(main);

K_THREAD_DEFINE(led_task_p, 1024, led_task, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(display_task_p, 1024, display_task, NULL, NULL, NULL, 5, 0, 0);

int main(void) {
  k_thread_name_set(led_task_p, "led_task");
  k_thread_name_set(display_task_p, "display_task");
  const char *task_name = CURRENT_NAME();
  for (;;) {
    LOG_INF("in task: %s\n", task_name);
    k_msleep(1000);
  }
  k_thread_join(led_task_p, K_NO_WAIT);
  k_thread_join(display_task_p, K_NO_WAIT);
  return 0;
}

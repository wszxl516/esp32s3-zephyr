#include "tasks_common.h"

LOG_MODULE_REGISTER(display_task);

int display_task(void *args, void *arg1, void *arg2) {
  const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
  uint16_t width;
  uint16_t heigh;
  if (!device_is_ready(display_dev)) {
    LOG_ERR("Device not ready, aborting test");
    return 0;
  }
  display_set_orientation(display_dev, DISPLAY_ORIENTATION_NORMAL);
  if (display_set_pixel_format(display_dev, PIXEL_FORMAT_MONO10) != 0) {
    LOG_ERR("Failed to set required pixel format");
    return 0;
  }
  LOG_INF("Initialized %s\n", display_dev->name);

  if (cfb_framebuffer_init(display_dev)) {
    LOG_ERR("Framebuffer initialization failed!\n");
    return 0;
  }
  width = cfb_get_display_parameter(display_dev, CFB_DISPLAY_WIDTH);
  heigh = cfb_get_display_parameter(display_dev, CFB_DISPLAY_HEIGH);
  cfb_framebuffer_clear(display_dev, true);
  display_blanking_off(display_dev);
  uint8_t font_width = 0, font_heigh = 0;
  LOG_INF("width: %d, heigh: %d\n", width, heigh);
  for (int idx = 0; idx < 42; idx++) {
    if (cfb_get_font_size(display_dev, idx, &font_width, &font_heigh) == 0) {
      cfb_framebuffer_set_font(display_dev, idx);
      LOG_INF("idx: %d,font_width: %d, font_heigh: %d\n", idx, font_width,
             font_heigh);
      break;
    }
  }
  const struct cfb_position start = {.x = 0, .y = 0};
  const struct cfb_position end = {.x = width - 1, .y = heigh - 1};
  uint16_t x = 0, y = 0;
  int16_t x_p = 1, y_p = 1;
  const char *text = "zephyr";
  uint16_t text_width = strlen(text) * font_width, text_heigh = font_heigh;
  const char *task_name = CURRENT_NAME();
  for (;;) {
    if (x >= width - text_width)
      x_p = -1;
    if (y >= heigh - text_heigh)
      y_p = -1;
    if (x <= 0)
      x_p = 1;
    if (y <= 0)
      y_p = 1;
    cfb_framebuffer_clear(display_dev, true);
    cfb_draw_rect(display_dev, &start, &end);
    cfb_draw_text(display_dev, text, x, y);
    cfb_framebuffer_finalize(display_dev);
    x += x_p;
    y += y_p;
    if (y == 0) LOG_INF("task: %s is running!\n", task_name);
    k_msleep(10);
  }
  return 0;
}
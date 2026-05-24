#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/dt-bindings/led/led.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

void led_strip_init(){
   const struct device* dev = DEVICE_DT_GET(DT_ALIAS(led_strip));
   if (!device_is_ready(dev)) {
      LOG_ERR("LED strip device not ready");
      return;
   }
   LOG_INF("LED strip ready");
   static struct led_rgb pixels[256]{0};
   pixels[0].b = 255;
   
   auto update = led_strip_update_rgb(dev, pixels, 256);
   if (update != 0){
      LOG_ERR("LED strip was not updated");
      return;  
   } else
      LOG_INF("update was fine");
}



int main(){
  led_strip_init();   
  return 0;
}

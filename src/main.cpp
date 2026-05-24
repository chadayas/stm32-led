#include<zephyr/kernel.h>
#include<zephyr/drivers/led_strip.h>
#include<zephyr/dt-bindings/led/led.h>
#include<zephyr/device.h>
#include<zephyr/logging/log.h>
#include<cstdint>

constexpr uint16_t MAX {256};

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

void led_animation(const struct device* dev){
   uint8_t i;
   static struct led_rgb pixels[MAX];
   for (i=0; i < MAX-1; i++){
      pixels[0].b = i; 
      auto update = led_strip_update_rgb(dev, pixels, MAX);
      if (update != 0){
         LOG_ERR("LED strip was not updated");
         return;  
      } else
      LOG_INF("update was fine"); 
   } 
}


void led_strip_init(){
   const struct device* dev = DEVICE_DT_GET(DT_ALIAS(led_strip));
   if (!device_is_ready(dev)) {
      LOG_ERR("LED strip device not ready");
      return;
   }
   LOG_INF("LED strip ready");
   led_animation(dev);    
}



int main(){
  led_strip_init();   
  return 0;
}

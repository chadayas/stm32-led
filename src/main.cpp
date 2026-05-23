#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/dt-bindings/led/led.h>
#include <zephyr/device.h>

#define LED_NODE DT_ALIAS(led0)
void led_strip_init(){
   const struct device* dev = DEVICE_DT_GET(DT_ALIAS(led_strip)); 
   struct led_rgb pixels[256]{0}; 
   pixels[0].b = 255; 
   led_strip_update_rgb(dev, pixels, 256);
}



int main(){
  led_strip_init();   
  return 0;
}

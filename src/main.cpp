#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/dt-bindings/led/led.h>

#define LED_NODE DT_ALIAS(led0)
void led_strip_init(){
   const struct device* dev = DEVICE_DT_GET(DT_ALIAS(led_strip)); 
   struct led_rgb pixels[256]{0}; 
   pixels[0].b = 255; 
   led_strip_update_rgb(device ,pixels, 256);
}
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

int main(){
   
  return 0;
}

  
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

int butflag;

void GPIO_init(){
	gpio_set_direction(5,GPIO_MODE_DEF_OUTPUT);
	
	gpio_pullup_dis(GPIO_NUM_5);
	
	gpio_set_level(5, 0);

}

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
	butflag = 1;
}

void app_main(void){
	
	GPIO_init();
	butflag = 0;
	gpio_set_direction(39,GPIO_MODE_DEF_INPUT);
	
	//change gpio intrrupt type for one pin
   	gpio_set_intr_type(39, GPIO_PIN_INTR_POSEDGE);

    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(39, gpio_isr_handler, (void*) 39);
	
	int i = 0;
	printf("starting main loop!\n");
	while(1){
		if(butflag){
			printf("%d,flagged high!",i);
			if(i % 2 == 0){
				gpio_set_level(5, 0);
			}
			else{
				gpio_set_level(5, 1);
			}
			i++;
			butflag = 0;
		}
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}
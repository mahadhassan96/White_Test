#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

int butflag;
gpio_num_t array[3] = {33,32,16};

void GPIO_init(){
	gpio_set_direction(33,GPIO_MODE_DEF_OUTPUT);
	gpio_set_direction(32, GPIO_MODE_DEF_OUTPUT);
	gpio_set_direction(16,GPIO_MODE_DEF_OUTPUT);
	
	gpio_pullup_dis(GPIO_NUM_16);
	gpio_pullup_dis(GPIO_NUM_33);
	gpio_pullup_dis(GPIO_NUM_32);
	
	gpio_set_level(32, 0);
	gpio_set_level(16, 0);
	gpio_set_level(33, 0);
}

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
	butflag = 1;
}

void app_main(void){
	
	GPIO_init();
	counter = 0;
	butflag = 0;
	gpio_set_direction(39,GPIO_MODE_DEF_INPUT);
	
	//change gpio intrrupt type for one pin
   	gpio_set_intr_type(39, GPIO_PIN_INTR_POSEDGE);

    	//install gpio isr service
    	gpio_install_isr_service(0);
    	//hook isr handler for specific gpio pin
    	gpio_isr_handler_add(39, gpio_isr_handler, (void*) 39);
	
	timer_config_t timer;
	
	timer.alarm_en = TIMER_ALARM_DIS;
	timer.counter_en = TIMER_START;
	timer.intr_type = TIMER_INTR_LEVEL;
	timer.counter_dir = TIMER_COUNT_UP;
	timer.auto_reload = TIMER_AUTORELOAD_DIS;
	timer.divider = 64;
	
	timer_init(TIMER_GROUP_0, TIMER_0, timer);
	
	int i = 0;
	printf("starting main loop!\n");
	while(1){
		if(butflag){
			if(i>2){
				i=0;
				gpio_set_level(array[2], 0);
			}
			gpio_set_level(array[i], 1);
			if(i>=1){gpio_set_level(array[i-1], 0);}
			i++;
			butflag=0;
		}
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}

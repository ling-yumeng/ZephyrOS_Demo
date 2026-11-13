/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/device.h>
#include "tempsensor.h"

#define SLEEP_TIME_US   300000

/* The devicetree node identifier for the "led0" alias. */

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct pwm_dt_spec led = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));
static const struct pwm_dt_spec led1 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led1));
static const struct pwm_dt_spec led2 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led2));
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET(DT_ALIAS(user_button), gpios);
static const struct device *i2c_main_bus = DEVICE_DT_GET(DT_ALIAS(tempsensor_i2c));

static struct gpio_callback button_cb_data;

bool button_int_sig = false;

#define CARRIER_PERIOD_NS   PWM_HZ(960) 

void set_pwm0(int brightness);
void set_pwm1(int brightness);
void set_pwm2(int brightness);
void button_callback_function(const struct device *port, struct gpio_callback *cb, gpio_port_pins_t pins);
void button_tasks();
void tempsensor_callback(float temperature);

bool light_off = false;
tempsensor__job_queue tempsensor__job_queue_0;

int main(void)
{
	tempsensor__job_queue_0=tempsensor__construct_job_queue();
	int brightness0 = 255;
	int brightness1 = 255;
	int brightness2 = 255;
	int i = 0;
	int timestep = SLEEP_TIME_US / 255;
	gpio_pin_configure_dt(&button0, GPIO_INPUT);
	gpio_pin_interrupt_configure_dt(&button0, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_init_callback(&button_cb_data, button_callback_function, BIT(button0.pin));
	gpio_add_callback(button0.port, &button_cb_data);
	printf("Light: %s.\n", light_off ? "OFF" : "ON");
	while (1) {
		if(i >= 765) {
			i = 0;
		}
		i++;
		if(i<255) {
			brightness2 = i;
			brightness0 = 255-i;
		}
		else if(i<510) {
			brightness0 = i-255;
			brightness1 = 510-i;
		}
		else if(i<765) {
			brightness1 = i-510;
			brightness2 = 765-i;
		}
		if(light_off) {
			set_pwm0(0);
			set_pwm1(0);
			set_pwm2(0);
		}
		else {
			set_pwm0(brightness0);
			set_pwm1(brightness1);
			set_pwm2(brightness2);
		}
		button_tasks();
		tempsensor__do_continue(i2c_main_bus, &tempsensor__job_queue_0);
		k_usleep(timestep);
	}
	return 0;
}

void button_callback_function(const struct device *port, struct gpio_callback *cb, gpio_port_pins_t pins) {
	button_int_sig = true;
}

void tempsensor_callback(float temperature) {
	printf("Tempsensor: %d C\n", (int)temperature);
}

void button_tasks() {
	if(button_int_sig) {
		button_int_sig = false;
		if(gpio_pin_get_dt(&button0)) {
			printf("pressed!\n");

			//Light ON/OFF
			/*
			light_off = !light_off;
			printf("Light: %s.\n", light_off ? "OFF" : "ON");
			*/

			//Tempsensor Logic
			tempsensor__read_temperature(&tempsensor__job_queue_0, tempsensor_callback);
		}
	}
}

void set_pwm0(int brightness) {
	uint32_t pulse_ns;

	if(brightness > 255) {
		brightness = 255;
	}

	pwm_set_dt(&led, CARRIER_PERIOD_NS, (CARRIER_PERIOD_NS*brightness)/255);
}

void set_pwm1(int brightness) {
	uint32_t pulse_ns;

	if(brightness > 255) {
		brightness = 255;
	}

	pwm_set_dt(&led1, CARRIER_PERIOD_NS, (CARRIER_PERIOD_NS*brightness)/255);
}

void set_pwm2(int brightness) {
	uint32_t pulse_ns;

	if(brightness > 255) {
		brightness = 255;
	}

	pwm_set_dt(&led2, CARRIER_PERIOD_NS, (CARRIER_PERIOD_NS*brightness)/255);
}

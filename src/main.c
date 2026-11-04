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

/* 1000 msec = 1 sec */
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

#define CARRIER_PERIOD_NS   PWM_HZ(960) 

void set_pwm0(int brightness);
void set_pwm1(int brightness);
void set_pwm2(int brightness);

int main(void)
{
	int brightness0 = 255;
	int brightness1 = 255;
	int brightness2 = 255;
	int i = 0;
	int timestep = SLEEP_TIME_US / 255;
	bool polar = false;
	while (1) {
		if(i >= 2550) {
			i = 0;
		}
		i++;
		if(i<255) {
			brightness0 = 255-i;
		}
		else if(i<510) {
			brightness0 = i-255;
		}
		else if(i<765) {
			brightness1 = 765-i;
		}
		else if(i<1020) {
			brightness1 = i-765;
		}
		else if(i<1275) {
			brightness2 = 1275-i;
		}
		else if(i<1530) {
			brightness2 = i-1275;
		}
		set_pwm0(brightness0);
		set_pwm1(brightness1);
		set_pwm2(brightness2);
		k_usleep(timestep);
	}
	return 0;
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

#pragma once
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>

typedef struct tempsensor__job_queue {
    char *instruct_ring;
    void **arguments_ring;
    char *i_beg;
    char *i_end;
    void **a_beg;
    void **a_end;
} tempsensor__job_queue;

void tempsensor__do_continue(const struct device *i2c_bus, tempsensor__job_queue *q);
tempsensor__job_queue tempsensor__construct_job_queue();
void tempsensor__read_temperature(tempsensor__job_queue *q, void (*callback)(float temperature));

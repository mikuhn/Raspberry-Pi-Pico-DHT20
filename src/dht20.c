/**
 * Copyright (c) 2023 Michael Kuhn
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "hardware/i2c.h"
#include "pico/stdlib.h"

#include "dht20.h"


int dht20_init() {
  sleep_ms(100);

  uint8_t buf[1] = {DHT20_COMMAND_STATUS};
  i2c_write_blocking(i2c_default, DHT20_ADDRESS, buf, 1, true);
  i2c_read_blocking(i2c_default, DHT20_ADDRESS, buf, 1, false);

  if (buf[0] != DHT20_RETURN_STATUS_OK) {
    printf("init not sucessfull: %x\n", buf[0]);
    return EXIT_FAILURE;
  }

  sleep_ms(10);
  return EXIT_SUCCESS;
}

int dht20_measure(dht20_measurement *measurement) {
  uint8_t buf[7] = {DHT20_TRIGGER_MEASUREMENT, DHT20_TRIGGER_MEASUREMENT_PARAM1,
                    DHT20_TRIGGER_MEASUREMENT_PARAM2};

  i2c_write_blocking(i2c_default, DHT20_ADDRESS, buf, 3, false);

  bool success = false;
  for (int i = 0; i < 5; i++) {
    sleep_ms(80);
    i2c_read_blocking(i2c_default, DHT20_ADDRESS, buf, 1, true);
    success = (buf[0] & DHT20_RETURN_STATUS_BUSY_MASK) == 0x0;
    if (success) {
      break;
    }
  }

  if (!success) {
    printf("Sensor not ready. Maximum number of tries reached.");
    return EXIT_FAILURE;
  }

  i2c_read_blocking(i2c_default, DHT20_ADDRESS, buf, 7, false);

  int humidity_raw = (buf[1] << 12) | (buf[2] << 8) | (buf[3] >> 4);
  int temperature_raw = ((buf[3] << 16) | (buf[4] << 8) | buf[5]) & 0xfffff;

  measurement->humidity = (humidity_raw / powf(2, 20)) * 100;
  measurement->temperature = ((temperature_raw / powf(2, 20)) * 200) - 50;

  return EXIT_SUCCESS;
}
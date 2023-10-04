/**
 * Copyright (c) 2023 Michael Kuhn
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DHT20_H
#define DHT20_H

#define DHT20_ADDRESS 0x38
#define DHT20_COMMAND_STATUS 0x71
#define DHT20_RETURN_STATUS_OK 0x18
#define DHT20_TRIGGER_MEASUREMENT 0xAC
#define DHT20_TRIGGER_MEASUREMENT_PARAM1 0x33
#define DHT20_TRIGGER_MEASUREMENT_PARAM2 0x00
#define DHT20_RETURN_STATUS_BUSY_MASK 0x80

typedef struct {
  float humidity;
  float temperature;
} dht20_measurement;

int dht20_init();
int dht20_measure(dht20_measurement *measurement);

#endif // DHT20_H
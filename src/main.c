/**
 * Copyright (c) 2023 Michael Kuhn
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#include "dht20.h"

// I2C reserves some addresses for special purposes. We exclude these from the
// scan. These are any addresses of the form 000 0xxx or 111 1xxx
bool reserved_addr(uint8_t addr) {
  return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

int main() {
  // Enable UART so we can print status output
  stdio_init_all();
  sleep_ms(1500);

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) ||             \
    !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c/bus_scan example requires a board with I2C pins
  puts("Default I2C pins were not defined");
#else
  // This example will use I2C0 on the default SDA and SCL pins (GP4, GP5 on a
  // Pico)
  i2c_init(i2c_default, 100 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  // Make the I2C pins available to picotool
  bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN,
                             GPIO_FUNC_I2C));

  dht20_init();
  dht20_measurement dht20_result;

  printf("humidity temperature\n");

  while (1) {
    dht20_measure(&dht20_result);
    printf("%.02f%%   %.02fC\n", dht20_result.humidity,
           dht20_result.temperature);
    sleep_ms(2000);
  }
#endif
}

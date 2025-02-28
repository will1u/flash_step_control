/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "read16.pio.h"

#define PIO_BASE_PIN 10

/// \tag::hello_uart[]


int main() {
    stdio_init_all();
    PIO pio = pio0;
    
    uint offset = pio_add_program(pio, &read16_program);

    uint sm = pio_claim_unused_sm(pio, true);

    read16_program_init(pio, sm, offset, PIO_BASE_PIN);
    
    uint32_t value_array[16];
    uint32_t value = 0x00000001;
    for (int i = 0; i < 16; i ++) {
        value_array[i] = value;         
        value = value * 2;
    }

    char userInput = '\0';
    bool invalidInputDisplayed = false;
    bool waitingForCommand = true;
    
    while (true) {
        if (waitingForCommand) {
            printf("Command (1 = flash, 0 = step): \n");
            waitingForCommand = false; // Avoid repeated prints
        }
        
        userInput = getchar_timeout_us(10000);

        if (userInput == '1') {
            invalidInputDisplayed = false;
            waitingForCommand = true;
            printf("LED flashing at 500ms intervals!\n");

            char userInput2 = '\0';
            printf("Press 2 to stop: \n");
            
            while (userInput2 != '2') {
                // Blink
                pio_sm_put_blocking(pio, sm, 0x03FFFFFF);
                sleep_ms(500);
                // Blonk
                pio_sm_put_blocking(pio, sm, 0x00000000);
                sleep_ms(500);
                
                userInput2 = getchar_timeout_us(10000); 
            }
            printf("Stopped flashing.\n");
        } 
        else if (userInput == '0') {
            invalidInputDisplayed = false;
            waitingForCommand = true;
            printf("LED flashing at 100ms intervals!\n");
            
            char userInput2 = '\0';
            printf("Press 2 to stop: \n");
            
            while (userInput2 != '2') {
                for (int i = 0; i < 16; i ++) {
                    uint32_t value = value_array[i];
                    pio_sm_put_blocking(pio, sm, value);
                    sleep_ms(100);

                    userInput2 = getchar_timeout_us(10000); 
                    if (userInput2 == '2') {
                        break;
                    }
                }
                
            }
            printf("Stopped flashing.\n");
        } 
        else if (userInput != PICO_ERROR_TIMEOUT && !invalidInputDisplayed) {
            gpio_put(25, 0);
            printf("Invalid input! Please enter 1 or 0.\n");
            invalidInputDisplayed = true; 
        }
    }
}



/// \end::hello_uart[]

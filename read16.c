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
    
    uint32_t value_array[8];
    uint32_t value = 0x00020001;

    for (int i = 0 ; i < 8 ; i++) {
        value_array[i] = value;
        value = value * 4;
    }

    char userInput = '\0';
    bool invalidInputDisplayed = false;
    bool waitingForCommand = true;
    while (true) {
        if (waitingForCommand) {
            printf("Command (1 = flash, 0 = step): \n");
            waitingForCommand = false; 
        }
        
        userInput = getchar_timeout_us(10000);

        if (userInput == '1') {
            invalidInputDisplayed = false;
            waitingForCommand = true;
            int repeats = 1;
            printf("Enter number of repeats (empty is default as 1): ");
            scanf("%d", &repeats);
            
            pio_sm_put_blocking(pio, sm, 0x00000000);
            for (int i = 0; i < repeats; i++){
                
                // pio_sm_put_blocking(pio, sm, 0xFFFFFFFF);
                pio_sm_put_blocking(pio, sm, 0x0000FFFF);
            }
            printf("Stopped flashing.\n");
            pio_sm_put_blocking(pio, sm, 0x00000000);

        } 

        else if (userInput == '0') {
            invalidInputDisplayed = false;
            waitingForCommand = true;

            int repeats = 1;
            printf("Enter number of repeats (empty is default as 1): ");
            scanf("%d", &repeats);

            printf("All flashed! \n");

            printf("Step flashing!\n");

            pio_sm_put_blocking(pio, sm, 0x00000000);
            for (int i = 0; i < repeats; i++){
                for (int j = 0; j < 8; j++) {
                    uint32_t value = value_array[j];
                    pio_sm_put_blocking(pio, sm, value);
                    // sleep_ms(500);                
                }        
            }
            pio_sm_put_blocking(pio, sm, 0x00000000);
            printf("Stopped flashing.\n");
        } 
    }
}



/// \end::hello_uart[]

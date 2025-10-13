/**
 * @file board_io.c
 * @brief Implementation of board_io.h control functions.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <avr/io.h>
#include <util/delay.h>

#include <board_io.h>

#include <DrinkCreator6000_Pins.h>

void circulation_on(void){
    PORTE|=(1<<CIRCULATION_PUMP_PIN);
}

void circulation_off(void){
    PORTE&=~(1<<CIRCULATION_PUMP_PIN);
}

void cooler_on(void){
    PORTD=(1<<PELTIER1_PIN)|(1<<PELTIER2_PIN);
}

void cooler_off(void){
    PORTD&=~((1<<PELTIER1_PIN)|(1<<PELTIER2_PIN));
}

void fans_on(void){
    PORTE|=(1<<FANS_PIN);
}

void fans_off(void){
    PORTE&=~(1<<FANS_PIN);
}

void pumps_enable(void){
    PORTC&=~(1<<OE_PIN);
}

void pumps_disable(void){
    PORTC|=(1<<OE_PIN);
}

bool read_keyboard_int(void){
    return PIND&(1<<KEYBOARD_INT_PIN); 
}

bool is_temp_ready(void){
    return (PINB&(1<<TEMPDATA_RDY))>0;
}

void shiftOut(uint8_t value){
    uint8_t i=0;

    // SH_PIN PC2 -> Shift Clock
    // DS_PIN PC0 -> Serial Data
    // ST_PIN PC1 -> Latch

    PORTC&=~(1<<ST_PIN);

    for(;i<8;i++){
        if(value&(1<<(7-i))){
            PORTC|=(1<<DS_PIN);
        }
        else{
            PORTC&=~(1<<DS_PIN);
        }

        PORTC|=(1<<SH_PIN);
        _delay_us(2);
        PORTC&=~(1<<SH_PIN);
        _delay_us(2);

    }

    PORTC|=(1<<ST_PIN);
}
#include <avr/interrupt.h>
#include <avr/io.h>

#include <DrinkCreator6000_Pins.h>

#include <buzzer.h>

//Dobra musze miec jakas tablice dla fsm
// moge zmieniac czestotliwosc np.
// wgl to isr jest do zmiany bo sam sprzet moze sterowac tym pinem
// mozemy zrobic tablice dzwiekow dla kazdego komunikatu
// i w isr bedziemy ja przetwarzac
// na pewno wymagaloby to dodatkowego semafora czy nagle dwa zadania sie probuja dostac do tego
// na pewno musimy jakos ogarnac problem i czestotliwosci i odstepow miedzy "bzyczeniem" zeby miec dzwiek
// wiec to jutro/dzisiaj sobie jakos rozplanuje

void initializeBuzzer(void){
    DDRB|=(1<<BUZZER_PIN);
    PORTB&=~(1<<BUZZER_PIN);

    TCCR1A=0;
    TCNT1=0;
    OCR1A=249;
    TCCR1B|=(1<<WGM12);
    TCCR1B|=(1<<CS11)|(1<<CS10);    
}

void activateBuzzer(uint8_t tone){
    TIMSK1|=(1<<OCIE1A);
}

void deactivateBuzzer(void){
    TIMSK1&=~(1<<OCIE1A);
    PORTB&=~(1<<BUZZER_PIN);
}

ISR(TIMER1_COMPA_vect){
    PORTB^=(1<<BUZZER_PIN);
}

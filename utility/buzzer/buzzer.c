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

#include <SPI.h>

#define BUFFER_SIZE 128
char buff[ BUFFER_SIZE ];

volatile uint8_t index = 0;
volatile bool data_ready = false;

void setup( void ) {
  Serial.begin( 9600 );
  pinMode( SS, INPUT );
  pinMode( MOSI, INPUT );
  pinMode( MISO, OUTPUT );
  pinMode( SCK, INPUT );
  SPCR |= _BV( SPE );
  SPI.attachInterrupt();
}

ISR( SPI_STC_vect ) {
  byte c = SPDR;
  if ( index < BUFFER_SIZE ) {
    if ( c == 0xAF ) data_ready = true;
    else buff[ index ] = c;
    ++index;
  }
}

void loop( void ) {
  if ( data_ready ) {
    for ( int i = 0; i < index; i++ ) {
      Serial.write( buff[ i ] );
    }
    Serial.write( '\n' );
    index = 0;
    data_ready = false;
  }
}

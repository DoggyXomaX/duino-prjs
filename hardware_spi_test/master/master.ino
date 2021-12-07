#include <SPI.h>

const char data[] = "Hello, there is some string, i think";
const byte data_size = sizeof( data );

void setup() {
  Serial.begin( 9600 );
  SPI.begin();
  pinMode( SS, OUTPUT );
  pinMode( MOSI, OUTPUT );
  pinMode( MISO, INPUT );
  pinMode( SCK, OUTPUT );
  digitalWrite( SS, HIGH );
  Serial.println( F( "Wait for it..." ) );
  delay( 3000 );
}

void loop() {
  digitalWrite( SS, LOW );
  SPI.beginTransaction( SPISettings( 1000000, MSBFIRST, SPI_MODE0 ) );
  Serial.println( F( "Transaction..." ) );
  for ( int i = 0; i < data_size; ++i ) {
    SPI.transfer( data[ i ] );
  }
  SPI.transfer( 0xAF );
  Serial.println( F( "End transaction" ) );
  SPI.endTransaction();
  digitalWrite( SS, HIGH );
  delay( 1000 );
}

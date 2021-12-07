// CLK
// CS
// MOSI
// MISO



#define PINMODE_D2(value) DDR

bool input_bit = 0;
bool output_bit = 0;

bool is_master = false;
bool clock_bit = false;

byte pattern[] = { 
  0, 0, 0, 0, 1, 1, 1, 1, 
  0, 0, 1, 1, 0, 0, 1, 1, 
  0, 1, 0, 1, 0, 1, 0, 1, 
  0, 0, 0, 1, 1, 1, 0, 1
};
byte pattern_pointer = 0;

void update_bits_output() {
  PORTB = (
    PINB & 0b11100111 | 
    ( !input_bit << 3 ) |
    ( !output_bit << 4 )
  );
}

void setup() {
  // erial.begin( 9600 );
  // delay( 500 );
  DDRB  = 0b00111010;
  DDRD  = 0b00011110;
  PORTB = 0b00011000;
  PORTD = 0b00001000;
}

void loop() {
  static int endMillis = 0;
  int currentMillis = millis();

  if ( endMillis < currentMillis ) {
    endMillis = currentMillis + 250;

    // DEBUG
//    Serial.print( "pattern_pointer = " );
//    Serial.print( pattern_pointer );
//    Serial.print( ", input bit = " );
//    Serial.print( input_bit, BIN );
//    Serial.print( ", output_bit = " );
//    Serial.println( output_bit, BIN );
    // DEBUG

    is_master = !( PIND & ( 1 << 7 ) );
    input_bit = is_master;

    if ( is_master ) {
      pattern_pointer++;
      if ( pattern_pointer >= 32 ) pattern_pointer = 0;
      bool pointer_bit = pattern[ pattern_pointer ];
      
      clock_bit = pointer_bit;
      output_bit = clock_bit;

      PORTD = PIND & ~( 1 << 2 ) | ( clock_bit << 2 );
    }
  }

  if ( !is_master ) {
    clock_bit = PIND & ( 1 << 6 );
    output_bit = !!clock_bit;
  }

  update_bits_output();
}

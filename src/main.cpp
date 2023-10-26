#include "Decoder.h"

Decoder decoder;

void setup()
{
#if defined(DEBUG_SERVO_MOVEMENT) || defined(DEBUG_DCC_PACKETS)
  Serial.begin(115200);
#else
  #include <avr8-stub.h>
  debug_init();
#endif

  // put your setup code here, to run once:
  decoder.setupDCC();
}

void loop()
{
  // put your main code here, to run repeatedly:
  decoder.process();
}
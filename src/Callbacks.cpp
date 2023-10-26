#include "CV.h"
#include "Decoder.h"

void notifyCVResetFactoryDefault()
{
    for (unsigned int j = 0; j < sizeof(FactoryDefaultCVs) / sizeof(struct CVPair); j++)
    {
        decoder.Dcc.setCV(FactoryDefaultCVs[j].CV, FactoryDefaultCVs[j].Value);
    }
}

// This function is called whenever a normal DCC Turnout Packet is received and we're in Output Addressing Mode
void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
{
    static int debounce = 0;

#ifdef DEBUG_DCC_PACKETS
    Serial.print("notifyDccAccTurnoutOutput: ");
    Serial.print(Addr, DEC);
    Serial.print(", ");
    Serial.print(Direction, DEC);
    Serial.print(((Direction_t)Direction == closePosition) ? " (close)" : " (open)");
    Serial.print(", ");
    Serial.println(OutputPower, HEX) ;
#endif  // DEBUG_DCC_PACKETS

    if (int(OutputPower) != debounce)  {
        debounce = OutputPower;
        if (debounce == 0) decoder.processTurnoutData(Addr, (Direction_t)Direction);
    }    
}

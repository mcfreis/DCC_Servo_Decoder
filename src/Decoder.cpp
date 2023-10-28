#include "Decoder.h"
#include "CV.h"

#define SERVO_ON(idx) { \
    servo[servoIdx].attach(pwmPin[servoIdx]); \
    digitalWrite(powerPin[servoIdx], HIGH); \
}

#define SERVO_OFF(idx) { \
    digitalWrite(powerPin[servoIdx], LOW); \
    servo[servoIdx].detach(); \
}

Decoder::Decoder()
{    
    setupButton.reset();
}

Decoder::~Decoder() {}

void Decoder::setupDCC()
{
    console.displayIntro();

    // Do we want to configure the decoder?
    checkIfSetupMode();

    // Possibly set factory defaults
    resetCVs();

    // Setup which External Interrupt, the Pin is associated with that we're using and enable the Pull-Up
    Dcc.pin(digitalPinToInterrupt(DCC_IN_PIN), DCC_IN_PIN, true);

    // Call the main DCC Init function to enable the DCC Receiver
    Dcc.init(MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0);

    // Fetch address
    baseAddress = (Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB) << 8) + Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB);
    console.displayAddress(baseAddress);

    // Attach Servos and load settings
    initServos();

    // Possibly ...
    startSetup();
}

void Decoder::checkIfSetupMode()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    pinMode(PUSH_BUTTON_PIN, INPUT);
    setupButton.reset();
    
    while (analogRead(PUSH_BUTTON_PIN) < 512) {}
    setupButton.press();

    // nach X ms zum programmieren übergehen
    if ((TIMEOUT_PUSH_TO_SETUP <= setupButton.delta()) && 
        (setupButton.delta() < TIMEOUT_PUSH_TO_FACTORY_RESET))
    {
        decoderSM = setupAddress;
    }
    // nach X sec ein Reset der Konfiguration durchführen
    else if (setupButton.delta() >= TIMEOUT_PUSH_TO_FACTORY_RESET)
    {
        doResetEEProm = true;
    }

    setupButton.reset();
    digitalWrite(LED_PIN, LOW);
}

void Decoder::resetCVs()
{
    // Sollte das EEProm noch "frisch" sein, dann ein Reset forcieren
    if (Dcc.getCV(CV_SERVO_CLOSE) == 255)
    {
        doResetEEProm = true;
    }
    // EEProm auf die "Werkseinstellungen" setzen
    if (doResetEEProm == true)
    {
        console.displayResetFactorySettings();

        notifyCVResetFactoryDefault();
        doResetEEProm = false;
    }
}

void Decoder::initServos()
{
    for (int servoIdx = 0; servoIdx < SERVO_COUNT; servoIdx++)
    {
        // Load settings
        servoSettings[servoIdx].posClosed = Dcc.getCV(CV_SERVO_OFFSET(CV_SERVO_CLOSE, servoIdx));
        servoSettings[servoIdx].posThrown = Dcc.getCV(CV_SERVO_OFFSET(CV_SERVO_THROW, servoIdx));
        servoSettings[servoIdx].posCurrent = Dcc.getCV(CV_SERVO_OFFSET(CV_SERVO_POSITION, servoIdx));
        servoSettings[servoIdx].config = (Config_t)Dcc.getCV(CV_SERVO_OFFSET(CV_SERVO_CONFIG, servoIdx));
        servoSettings[servoIdx].speed = Dcc.getCV(CV_SERVO_OFFSET(CV_SERVO_SPEED, servoIdx));
        
        // Clean up
        servoSettings[servoIdx].direction = endPosition;
        if (servoSettings[servoIdx].config != Config_t::configured) 
            servoSettings[servoIdx].posCurrent = SERVO_ANGLE_CENTER;

        // Configure Power Pin
        pinMode(powerPin[servoIdx], OUTPUT);
        setServoToPosition(servoIdx);  // Turns power on automatically
        delay(TIMEOUT_WAIT_BEFORE_OFF);
        SERVO_OFF(servoIdx);

        console.displayReadSettings(servoIdx, &servoSettings[servoIdx]);
    }
}

void Decoder::saveServo(int servoIdx)
{
    SERVO_OFF(servoIdx);
    servoSettings[servoIdx].direction = endPosition;
    servoSettings[servoIdx].config = Config_t::configured;

    Dcc.setCV(CV_SERVO_OFFSET(CV_SERVO_CLOSE, servoIdx), servoSettings[servoIdx].posClosed);
    Dcc.setCV(CV_SERVO_OFFSET(CV_SERVO_THROW, servoIdx), servoSettings[servoIdx].posThrown);
    Dcc.setCV(CV_SERVO_OFFSET(CV_SERVO_POSITION, servoIdx), servoSettings[servoIdx].posCurrent);
    Dcc.setCV(CV_SERVO_OFFSET(CV_SERVO_SPEED, servoIdx), servoSettings[servoIdx].speed);
    Dcc.setCV(CV_SERVO_OFFSET(CV_SERVO_CONFIG, servoIdx), (uint8_t)servoSettings[servoIdx].config);

    console.displaySaveSettings(servoIdx, &servoSettings[servoIdx]);
}

void Decoder::wobbleServo(int iterations, int servoIdx)
{
    const int positions[2][2] = {
        {SERVO_WOBBLE_CLOSED, SERVO_WOBBLE_THROWN}, 
        {SERVO_WOBBLE_THROWN, SERVO_WOBBLE_CLOSED}
    };
    int idx = 0;

    if (servoSettings[servoIdx].posCurrent < SERVO_ANGLE_CENTER) idx = 1;

    console.startWobble();
    SERVO_ON(servoIdx);
    delay(TIMEOUT_WAIT_BEFORE_OFF);
    for (int j = 0; j < iterations; j++)
    {
        servo[servoIdx].write(positions[idx][0]);
        delay(TIMEOUT_WOBBLE_PERIODE);
        servo[servoIdx].write(positions[idx][1]);
        delay(TIMEOUT_WOBBLE_PERIODE);
    }
    servo[servoIdx].write(servoSettings[servoIdx].posClosed);
    delay(TIMEOUT_WAIT_BEFORE_OFF);
    SERVO_OFF(servoIdx);
    console.endWobble();
}

void Decoder::startSetup()
{
    if (decoderSM == setupAddress)
    {
        wobbleServo(4, 0);
        console.displayWaitingForAddress();
    }
}

void Decoder::configureCycleToSpeed()
{
    decoderSM = setupServoSpeed;
    console.displaySwitchToSpeed(currentServoIdx, &servoSettings[currentServoIdx]);

    servoSettings[currentServoIdx].direction = endPosition;

    setupButton.reset();
}

void Decoder::configureCycleThroughServos()
{
    if (decoderSM == setupServoPositionClose)
    {
        // Prepare for setup to correct position
        servoSettings[currentServoIdx].direction = throwPosition;
        setServoToPosition(currentServoIdx);

        // Jetzt den anderen Anschlag
        decoderSM = setupServoPositionThrow;
        console.displaySwitchFromCloseToThrow(currentServoIdx, &servoSettings[currentServoIdx]);

        setupButton.reset();
        
        return;
    }

    if ((currentServoIdx >= 0) && (currentServoModified == true)) saveServo(currentServoIdx);   

    currentServoIdx++;
    currentServoModified = false;

    if ((currentServoIdx >= 0) && (currentServoIdx < SERVO_COUNT))
    {
        wobbleServo(2, currentServoIdx);

        // Prepare for setup to correct position
        servoSettings[currentServoIdx].direction = closePosition;
        setServoToPosition(currentServoIdx);

        // Nächstes Servo ansteuern
        decoderSM = setupServoPositionClose;
        console.displaySetupClose(currentServoIdx, &servoSettings[currentServoIdx]);

        setupButton.reset();
    }
    else
    {
        wobbleServo(4, 0);

        // Wir sind fertig
        decoderSM = normalOperation; 
        console.displayFinish();   

        asm volatile ("  jmp 0");  // reset!   
    }
}

void Decoder::process()
{
    // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
    Dcc.process();

    if (decoderSM == setupAddress)
    {
        if (setupButton.since() > TIMEOUT_WAIT_FOR_ADDRESS)
        {
            wobbleServo(8, 0);

            asm volatile ("  jmp 0");  // reset!
        }
    }
    else if ((decoderSM == setupServoPositionClose) || (decoderSM == setupServoPositionThrow))
    { 
        if (setupButton.since() > TIMEOUT_SWITCH_SERVO_POS_SETUP)
        {
            if (decoderSM == setupServoPositionClose) configureCycleThroughServos();
            else configureCycleToSpeed();
        }
        else
            setServoToPosition(currentServoIdx);
    }
    else if (decoderSM == setupServoSpeed)
    { 
        if ((servoSettings[currentServoIdx].direction == endPosition) && 
            (setupButton.since() > TIMEOUT_SWITCH_SPEED_TO_POS))
        {
            configureCycleThroughServos();
        }
        else
            testServoSpeed(currentServoIdx);
    }

    if ((decoderSM == normalOperation) || (decoderSM == setupServoSpeed))
    {
        for (int servoIdx = 0; servoIdx < SERVO_COUNT; servoIdx++)
        {
            // Solange das Servo nicht die Endposition erreicht hat ...
            if (servoSettings[servoIdx].direction != endPosition)
            {
                moveServoToPosition(servoIdx, servoSettings[servoIdx].direction);
            }
        }
    }
}

void Decoder::setServoToPosition(int servoIdx)
{
    // Servo einschalten und neuen Wert setzen
    SERVO_ON(servoIdx);

    if (servoSettings[servoIdx].direction == throwPosition)
    {
        servo[servoIdx].write(servoSettings[servoIdx].posThrown);
        servoSettings[servoIdx].posCurrent = servoSettings[servoIdx].posThrown;
    }
    else if (servoSettings[servoIdx].direction == closePosition)
    {
        servo[servoIdx].write(servoSettings[servoIdx].posClosed);
        servoSettings[servoIdx].posCurrent = servoSettings[servoIdx].posClosed;
    }
    else
    {
        servo[servoIdx].write(servoSettings[servoIdx].posCurrent);
    }
    servoSettings[servoIdx].direction = endPosition;
}

void Decoder::testServoSpeed(int servoIdx)
{    
    // Servo einschalten und neuen Wert setzen
    SERVO_ON(servoIdx);

    // Den Test nur aus der Ruheposition starten
    if (servoSettings[servoIdx].direction == endPosition)
    {
        // Toogle direction
        lastDirection = (lastDirection == closePosition) ? throwPosition : closePosition;
        
        servoSettings[servoIdx].direction = lastDirection;
        servoSettings[servoIdx].tLastMove = millis();
    }
}

void Decoder::processTurnoutData(uint16_t Addr, Direction_t Direction)
{
    if (decoderSM == normalOperation)
    {
        setTurnoutDirection(Addr, Direction);
    }
    else
    {
        setupButton.press();

        if (decoderSM == setupAddress)
        {
            baseAddress = Addr - (Addr % 4) + 1;
            console.displayAddress(baseAddress);

            Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, baseAddress % 256);
            Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, baseAddress / 256);

            // Move on to position configuration
            currentServoIdx = -1;
            configureCycleThroughServos();
        }
        else if ((decoderSM == setupServoPositionClose) || (decoderSM == setupServoPositionThrow))
        {
            configureServoPosition(Direction);
        }
        else if (decoderSM == setupServoSpeed)
        {
            configureServoSpeed(Direction);
        }
    }
}

// Mit dieser Funktionen werden die Servo-Wege konfiguriert (erweitert oder verringert)
// abhängig von der Tastendruckdauer und des aktuell gewählten Servos.
// Die neuen Werte werden hier noch nicht gespeichert bzw. auch nicht an das Servo übertragen!
void Decoder::configureServoPosition(Direction_t Direction)
{
    uint8_t *pPosition;

    if (decoderSM == setupServoPositionClose)
    {
        pPosition = &servoSettings[currentServoIdx].posClosed;

        if (Direction == throwPosition)
        {
            servoSettings[currentServoIdx].direction = closePosition;
            *pPosition += 2;    
        }
        else if (Direction == closePosition)
        {
            servoSettings[currentServoIdx].direction = closePosition;
            *pPosition -= 2;
        } 
    }
    else
    {
        pPosition = &servoSettings[currentServoIdx].posThrown;

        if (Direction == throwPosition)
        {
            servoSettings[currentServoIdx].direction = throwPosition;
            *pPosition -= 2;

        }
        else if (Direction == closePosition)
        {
            servoSettings[currentServoIdx].direction = throwPosition;
            *pPosition += 2;
        }
    }

    // Keep'm in the bands
    if (*pPosition <= SERVO_ANGLE_MIN) *pPosition = SERVO_ANGLE_MIN;
    if (*pPosition >= SERVO_ANGLE_MAX) *pPosition = SERVO_ANGLE_MAX;

    currentServoModified = true;

    console.displayPositions(&servoSettings[currentServoIdx]);
}

// Stellzeit des Servos einstellen (vergrößern oder verkleinern).
// Die neuen Werte werden hier noch nicht gespeichert.
void Decoder::configureServoSpeed(Direction_t Direction)
{
    uint8_t *pSpeed = &servoSettings[currentServoIdx].speed;

    if (Direction == throwPosition) *pSpeed += 2;  // ms
    else if (Direction == closePosition) *pSpeed -= 2;  // ms

    // Keep'm in the bands
    if (*pSpeed >= SERVO_SPEED_MAX) *pSpeed = SERVO_SPEED_MAX;
    else if (*pSpeed <= SERVO_SPEED_MIN) *pSpeed = SERVO_SPEED_MIN;

    currentServoModified = true;
    
    console.displaySpeed(&servoSettings[currentServoIdx]);
}

void Decoder::setTurnoutDirection(uint16_t Addr, Direction_t Direction)
{
    int servoIdx = Addr - baseAddress;

    servoSettings[servoIdx].direction = Direction;
    servoSettings[servoIdx].tLastMove = millis();

    SERVO_ON(servoIdx);

    console.displayTurnoutToggle(servoIdx, &servoSettings[servoIdx], Direction);
}

// Servo unter Berücksichtigung des Speeds auf die Position fahren. Wenn
// die Position erreicht ist, dann Position speichern und Servo ausschalten,
void Decoder::moveServoToPosition(int servoIdx, Direction_t Direction)
{
    uint8_t *pCurrent = &servoSettings[servoIdx].posCurrent;

    if (millis() > (servoSettings[servoIdx].tLastMove + servoSettings[servoIdx].speed))
    {
        if (Direction == throwPosition)
        {
            if (*pCurrent > servoSettings[servoIdx].posThrown) *pCurrent -= 1;
            else if (*pCurrent < servoSettings[servoIdx].posThrown) *pCurrent += 1;

            servo[servoIdx].write(*pCurrent);
            servoSettings[servoIdx].tLastMove = millis();
        }

        if (Direction == closePosition)
        {
            if (*pCurrent < servoSettings[servoIdx].posClosed) *pCurrent += 1;
            else if (*pCurrent > servoSettings[servoIdx].posClosed) *pCurrent -= 1;

            servo[servoIdx].write(*pCurrent);
            servoSettings[servoIdx].tLastMove = millis();
        }

        // Wenn Endposition erreicht ist, dann in CV speichern und Servo ausschalten
        if ((*pCurrent == servoSettings[servoIdx].posClosed) || 
            (*pCurrent == servoSettings[servoIdx].posThrown))
        {
            SERVO_OFF(servoIdx);
            servoSettings[servoIdx].direction = endPosition;
            if (!((decoderSM == setupServoSpeed) && (currentServoModified == false)))
            {
                Dcc.setCV(CV_SERVO_OFFSET(CV_SERVO_POSITION, servoIdx), *pCurrent);
                console.displayUpdatePosition(servoIdx, &servoSettings[servoIdx]); 
            }
                    
        }
    }
}

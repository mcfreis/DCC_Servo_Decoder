#include <NmraDcc.h>

#include "DccServoDecoder.h"
#include "CV.h"


struct CVPair FactoryDefaultCVs[45] = 
{
    // These two CVs define the Long Accessory Address
    {CV_ACCESSORY_DECODER_ADDRESS_LSB, DEFAULT_ADDRESS & 0xFF},
    {CV_ACCESSORY_DECODER_ADDRESS_MSB, (DEFAULT_ADDRESS >> 8) & 0x07},

    {CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB, 0},
    {CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB, 0},
    // Accessory Decoder Short Address
    {CV_29_CONFIG, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE | CV29_F0_LOCATION},

    // {CV_DECODER_MASTER_RESET, 0},
    // {CV_To_Store_SET_CV_Address, SET_CV_Address&0xFF },   // LSB Set CV Address
    // {CV_To_Store_SET_CV_Address+1,(SET_CV_Address>>8)&0x3F },  //MSB Set CV Address

    // Servo-Einstellungen
    {CV_SERVO_OFFSET(CV_SERVO_CLOSE, 0), SERVO_DEFAULT_CLOSED}, // Servo close position
    {CV_SERVO_OFFSET(CV_SERVO_THROW, 0), SERVO_DEFAULT_THROWN}, // Servo throw position
    {CV_SERVO_OFFSET(CV_SERVO_POSITION, 0), SERVO_ANGLE_CENTER}, // Servo current position
    {CV_SERVO_OFFSET(CV_SERVO_SPEED, 0), SERVO_DEFAULT_SPEED}, // Servo speed
    {CV_SERVO_OFFSET(CV_SERVO_CONFIG, 0), 0},  // Servo config

    {CV_SERVO_OFFSET(CV_SERVO_CLOSE, 1), SERVO_DEFAULT_CLOSED}, // Servo close position
    {CV_SERVO_OFFSET(CV_SERVO_THROW, 1), SERVO_DEFAULT_THROWN}, // Servo throw position
    {CV_SERVO_OFFSET(CV_SERVO_POSITION, 1), SERVO_ANGLE_CENTER}, // Servo current position
    {CV_SERVO_OFFSET(CV_SERVO_SPEED, 1), SERVO_DEFAULT_SPEED}, // Servo speed
    {CV_SERVO_OFFSET(CV_SERVO_CONFIG, 1), 0},  // Servo config

    {CV_SERVO_OFFSET(CV_SERVO_CLOSE, 2), SERVO_DEFAULT_CLOSED}, // Servo close position
    {CV_SERVO_OFFSET(CV_SERVO_THROW, 2), SERVO_DEFAULT_THROWN}, // Servo throw position
    {CV_SERVO_OFFSET(CV_SERVO_POSITION, 2), SERVO_ANGLE_CENTER}, // Servo current position
    {CV_SERVO_OFFSET(CV_SERVO_SPEED, 2), SERVO_DEFAULT_SPEED}, // Servo speed
    {CV_SERVO_OFFSET(CV_SERVO_CONFIG, 2), 0},  // Servo config

    {CV_SERVO_OFFSET(CV_SERVO_CLOSE, 3), SERVO_DEFAULT_CLOSED}, // Servo close position
    {CV_SERVO_OFFSET(CV_SERVO_THROW, 3), SERVO_DEFAULT_THROWN}, // Servo throw position
    {CV_SERVO_OFFSET(CV_SERVO_POSITION, 3), SERVO_ANGLE_CENTER}, // Servo current position
    {CV_SERVO_OFFSET(CV_SERVO_SPEED, 3), SERVO_DEFAULT_SPEED}, // Servo speed
    {CV_SERVO_OFFSET(CV_SERVO_CONFIG, 3), 0},  // Servo config

    {CV_SERVO_OFFSET(CV_SERVO_CLOSE, 4), SERVO_DEFAULT_CLOSED}, // Servo close position
    {CV_SERVO_OFFSET(CV_SERVO_THROW, 4), SERVO_DEFAULT_THROWN}, // Servo throw position
    {CV_SERVO_OFFSET(CV_SERVO_POSITION, 4), SERVO_ANGLE_CENTER}, // Servo current position
    {CV_SERVO_OFFSET(CV_SERVO_SPEED, 4), SERVO_DEFAULT_SPEED}, // Servo speed
    {CV_SERVO_OFFSET(CV_SERVO_CONFIG, 4), 0},  // Servo config

    {CV_SERVO_OFFSET(CV_SERVO_CLOSE, 5), SERVO_DEFAULT_CLOSED}, // Servo close position
    {CV_SERVO_OFFSET(CV_SERVO_THROW, 5), SERVO_DEFAULT_THROWN}, // Servo throw position
    {CV_SERVO_OFFSET(CV_SERVO_POSITION, 5), SERVO_ANGLE_CENTER}, // Servo current position
    {CV_SERVO_OFFSET(CV_SERVO_SPEED, 5), SERVO_DEFAULT_SPEED}, // Servo speed
    {CV_SERVO_OFFSET(CV_SERVO_CONFIG, 5), 0},  // Servo config

    {CV_SERVO_OFFSET(CV_SERVO_CLOSE, 6), SERVO_DEFAULT_CLOSED}, // Servo close position
    {CV_SERVO_OFFSET(CV_SERVO_THROW, 6), SERVO_DEFAULT_THROWN}, // Servo throw position
    {CV_SERVO_OFFSET(CV_SERVO_POSITION, 6), SERVO_ANGLE_CENTER}, // Servo current position
    {CV_SERVO_OFFSET(CV_SERVO_SPEED, 6), SERVO_DEFAULT_SPEED}, // Servo speed
    {CV_SERVO_OFFSET(CV_SERVO_CONFIG, 6), 0},  // Servo config

    {CV_SERVO_OFFSET(CV_SERVO_CLOSE, 7), SERVO_DEFAULT_CLOSED}, // Servo close position
    {CV_SERVO_OFFSET(CV_SERVO_THROW, 7), SERVO_DEFAULT_THROWN}, // Servo throw position
    {CV_SERVO_OFFSET(CV_SERVO_POSITION, 7), SERVO_ANGLE_CENTER}, // Servo current position
    {CV_SERVO_OFFSET(CV_SERVO_SPEED, 7), SERVO_DEFAULT_SPEED}, // Servo speed
    {CV_SERVO_OFFSET(CV_SERVO_CONFIG, 7), 0},  // Servo config
};

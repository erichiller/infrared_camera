#ifndef ESP32
#define ESP32
#endif

// #define MLX90640_DEBUG


#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
// #include <Adafruit_MLX90640.h>
// #include "MLX90640_API.cpp"
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"
#include "gfx.h"
#include "util.h"
#include <Wire.h>


const byte MLX90640_address = 0x33;    //Default 7-bit unshifted address of the MLX90640

#define TA_SHIFT 8    //Default shift for MLX90640 in open air

static float   mlx90640To[768];
paramsMLX90640 mlx90640;


// Adafruit_MLX90640 mlx;
// float             frame[32 * 24];    // buffer for full frame of temperatures

//low range of the sensor (this will be blue on the screen)
#define MINTEMP 20

//high range of the sensor (this will be red on the screen)
#define MAXTEMP 35



TFT_eSPI tft = TFT_eSPI( );    // Invoke custom library with default width and height

// #define TFT_GREY 0x7BEF

uint32_t runTime = 0;


void setup( ) {
	// initialize built in LED pin as an output.
	pinMode( LED_BUILTIN, OUTPUT );
	// digitalWrite( LED_BUILTIN, HIGH );    // turn the LED on (HIGH is the voltage level)
	// randomSeed( analogRead( 0 ) );
	Serial.begin( 38400 );
	while( !Serial )
		;
	Serial.println( "Initializing..." );

	Wire.begin( 33, 34, 100000 );

	//   Serial.begin(38400);
	// Setup the LCD
	tft.init( );
	tft.setRotation( 3 );



	char text[4];
	// for( int i = 0; i < 4; i++ ) {
	// 	tft.setRotation( i );
	// 	tft.fillScreen( TFT_BLACK );
	// 	sprintf( text, "Rotation: %d", i );
	// 	DrawNotice( &tft, text );
	// 	sleep( 3 );
	// }

	for( int i = 0; i < 5; i++ ) {
		Serial.printf( "Starting in %i\n", 5-i );
		sprintf( text, "%d", 5 - i );
		DrawNotice( &tft, text );
        sleep(1);
	}

	// Wire.setPins( 33, 34 );
	// Wire.setClock( 400000 );    // max 1 MHz
	// Wire.setClock( 100000 );    // max 100,000KHz


	if( isConnected( ) == false ) {
		Serial.println( "MLX90640 not detected at default I2C address. Please check wiring. Freezing." );
		DrawNotice( &tft, "MLX90640 not found!" );
		digitalWrite( LED_BUILTIN, HIGH );    // turn the LED on (HIGH is the voltage level)
		while( 1 );
	}
	Serial.println( "MLX90640 is online." );

	// MLX90640_I2CWrite( 0x33, 0x800D, 6401 );


	//Get device parameters - We only have to do this once
	int      status;
	uint16_t eeMLX90640[832];
	status = MLX90640_DumpEE( MLX90640_address, eeMLX90640 );
	if( status != 0 ){
		digitalWrite( LED_BUILTIN, HIGH );    // turn the LED on (HIGH is the voltage level)
		Serial.printf( "Failed to load system parameters at address %X\n", MLX90640_address );
		while( 1 );
	}

    for( int i = 0; i < 832; i++ ){
		printhex(eeMLX90640[i]);
	}

	int error = CheckEEPROMValidLocal( eeMLX90640 );
	if( error != 0 ) {
		digitalWrite( LED_BUILTIN, HIGH );    // turn the LED on (HIGH is the voltage level)
		Serial.println( "EEPROM Check Failed" );
		DrawNotice( &tft, "EEPROM Check Failed" );
		while( 1 )
			;
	}

	status = MLX90640_ExtractParameters( eeMLX90640, &mlx90640 );
	if( status != 0 ) {
		digitalWrite( LED_BUILTIN, HIGH );    // turn the LED on (HIGH is the voltage level)
		Serial.println( "Parameter extraction failed" );
		DrawNotice( &tft, "Parameter extraction failed" );
		while( 1 );
	}

	// MLX90640_I2CWrite( 0x33, 0x800D, 6401 );


	// Serial.println( "Adafruit MLX90640 Camera" );
	// if( !mlx.begin( MLX90640_I2CADDR_DEFAULT, &Wire ) ) {
        if ( ! isConnected() ) {
		    digitalWrite( LED_BUILTIN, HIGH );    // turn the LED on (HIGH is the voltage level)
		    DrawNotice( &tft, "MLX90640 not found!" );
		    while( true ) {
			    sleep( 10 );
        }
	}
	Serial.println( "Found MLX90640" );

	// Serial.print( "Serial number: " );
	// Serial.print( mlx.serialNumber[0], HEX );
	// Serial.print( mlx.serialNumber[1], HEX );
	// Serial.println( mlx.serialNumber[2], HEX );

	// mlx.setMode( MLX90640_CHESS );
	// mlx.setResolution( MLX90640_ADC_18BIT );
	// mlx.setRefreshRate( MLX90640_2_HZ );
	// // mlx.setRefreshRate( MLX90640_8_HZ );
	// // Wire.setClock( 1000000 );    // max 1 MHz
	// Wire.setClock( 400000 );    // max 1 MHz
}

// void loop( ) {
// 	// put your main code here, to run repeatedly:
// 	digitalWrite( LED_BUILTIN, HIGH );    // turn the LED on (HIGH is the voltage level)
// 	delay( 1000 );                        // wait for a second
// 	digitalWrite( LED_BUILTIN, LOW );     // turn the LED off by making the voltage LOW
// 	delay( 1000 );                        // wait for a second
// 	    // graphicstest_kbv_loop();
// }

#define RECT_X 15
#define RECT_Y 13    // 13.3333 ; 8 pixels remaining.

float min( float f, int i ) {
	if( i < f ) {
		return i;
	}
	return f;
}
float max( float f, int i ) {
	if( i > f ) {
		return i;
	}
	return f;
}

void loop( ) {
	int buf[478];
	int x, x2;
	int y, y2;
	int r;

	runTime = millis( );
	// Clear the screen and draw the frame
	tft.fillScreen( TFT_BLACK );


	uint32_t timestamp = millis( );
	// if( mlx.getFrame( frame ) != 0 ) {
	// 	Serial.println( "Failed" );
	// 	return;
	// }


	for( byte x = 0; x < 2; x++ )    //Read both subpages
	{
		uint16_t mlx90640Frame[834];
		int      status = MLX90640_GetFrameData( MLX90640_address, mlx90640Frame );
		if( status < 0 ) {
			Serial.print( "GetFrame Error: " );
			Serial.println( status );
            sleep(10);
		}

		float vdd = MLX90640_GetVdd( mlx90640Frame, &mlx90640 );
		float Ta  = MLX90640_GetTa( mlx90640Frame, &mlx90640 );

		float tr         = Ta - TA_SHIFT;    //Reflected temperature based on the sensor ambient temperature
		float emissivity = 0.95;

		MLX90640_CalculateTo( mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To );
	}



	for( uint8_t h = 0; h < 24; h++ ) {
		for( uint8_t w = 0; w < 32; w++ ) {
			float t = mlx90640To[h * 32 + w];
			// float t = frame[h * 32 + w];
			// Serial.print(t, 1); Serial.print(" => ");
            // Serial.printf("0x%X\n", t);

			t = min( t, MAXTEMP );
			t = max( t, MINTEMP );

			uint8_t colorIndex = map( t, MINTEMP, MAXTEMP, 0, 255 );

			colorIndex = constrain( colorIndex, 0, 255 );

			// int col = 0;
			// for( int i = 1; i < 6; i++ ) {
			// 	switch( i ) {
			// 		case 1:
			// 			col = TFT_MAGENTA;
			// 			break;
			// 		case 2:
			// 			col = TFT_RED;
			// 			break;
			// 		case 3:
			// 			col = TFT_GREEN;
			// 			break;
			// 		case 4:
			// 			col = TFT_BLUE;
			// 			break;
			// 		case 5:
			// 			col = TFT_YELLOW;
			// 			break;
			// 	}
			tft.fillRect( w * RECT_X, h * RECT_Y, RECT_X, RECT_Y, tft.color16to24(camColors[colorIndex] ));
			// Serial.printf( "%i , %i Colors (%i) %X\n", w, h, colorIndex, tft.color16to24( camColors[colorIndex] ) );
		}
	}
	sleep( 1 );

	Serial.print( ( millis( ) - timestamp ) / 2 );
	Serial.println( " ms per frame (2 frames per display)" );
	Serial.print( 2000.0 / ( millis( ) - timestamp ) );
	Serial.println( " FPS (2 frames per display)" );

}


//Returns true if the MLX90640 is detected on the I2C bus
boolean isConnected( ) {
	Wire.beginTransmission( (uint8_t)MLX90640_address );
	if( Wire.endTransmission( ) != 0 )
		return ( false );    //Sensor did not ACK
	return ( true );
}

int CheckEEPROMValidLocal( uint16_t *eeData ) {
	int deviceSelect;
	deviceSelect = eeData[10] & 0x0040;
	if( deviceSelect == 0 ) {
		return 0;
	}

	return -7;
}

#ifndef ESP32
#define ESP32
#endif



#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Adafruit_MLX90640.h>
#include "gfx.h"



Adafruit_MLX90640 mlx;
float             frame[32 * 24];    // buffer for full frame of temperatures

//low range of the sensor (this will be blue on the screen)
#define MINTEMP 20

//high range of the sensor (this will be red on the screen)
#define MAXTEMP 35



TFT_eSPI tft = TFT_eSPI( );    // Invoke custom library with default width and height

#define TFT_GREY 0x7BEF

uint32_t runTime = 0;


void setup( ) {
	// initialize built in LED pin as an output.
	pinMode( LED_BUILTIN, OUTPUT );
	digitalWrite( LED_BUILTIN, HIGH );    // turn the LED on (HIGH is the voltage level)
	randomSeed( analogRead( 0 ) );
	Serial.begin( 38400 );
	// while( !Serial )
	// 	;

	//   Serial.begin(38400);
	// Setup the LCD
	tft.init( );
	tft.setRotation( 3 );



	char text[4];
	for( int i = 0; i < 4; i++ ) {
		tft.setRotation( i );
		tft.fillScreen( TFT_BLACK );
		sprintf( text, "Rotation: %d", i );
		DrawNotice( &tft, text );
		sleep( 2 );
	}

	for( int i = 0; i < 5; i++ ) {
		sprintf( text, "%d", 5 - i );
		DrawNotice( &tft, text );
        sleep(1);
	}

	Serial.println( "Adafruit MLX90640 Camera" );
	if( !mlx.begin( MLX90640_I2CADDR_DEFAULT, &Wire ) ) {
		DrawNotice( &tft, "MLX90640 not found!" );
        while(true){
            sleep(10);
        }
	}
	Serial.println( "Found Adafruit MLX90640" );

	Serial.print( "Serial number: " );
	Serial.print( mlx.serialNumber[0], HEX );
	Serial.print( mlx.serialNumber[1], HEX );
	Serial.println( mlx.serialNumber[2], HEX );

	mlx.setMode( MLX90640_CHESS );
	mlx.setResolution( MLX90640_ADC_18BIT );
	mlx.setRefreshRate( MLX90640_8_HZ );
	Wire.setClock( 1000000 );    // max 1 MHz
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
	if( mlx.getFrame( frame ) != 0 ) {
		Serial.println( "Failed" );
		return;
	}

	int colorTemp;
	for( uint8_t h = 0; h < 24; h++ ) {
		for( uint8_t w = 0; w < 32; w++ ) {
			float t = frame[h * 32 + w];
			// Serial.print(t, 1); Serial.print(", ");

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
			tft.fillRect( w * RECT_X, h * RECT_Y, RECT_X, RECT_Y, camColors[colorIndex] );
		}
	}
	Serial.print( ( millis( ) - timestamp ) / 2 );
	Serial.println( " ms per frame (2 frames per display)" );
	Serial.print( 2000.0 / ( millis( ) - timestamp ) );
	Serial.println( " FPS (2 frames per display)" );



	// tft.fillRect( 0, 0, 480, 13, TFT_RED );

	// tft.fillRect( 0, 305, 480, 320, TFT_GREY );
	// tft.setTextColor( TFT_BLACK, TFT_RED );

	// tft.drawCentreString( "* TFT_eSPI *", CENTRE, 3, 1 );
	// tft.setTextColor( TFT_YELLOW, TFT_GREY );
	// tft.drawCentreString( "Adapted by Bodmer", CENTRE, 309, 1 );

	// tft.drawRect( 0, 14, 479, 305 - 14, TFT_BLUE );

	// // Draw crosshairs
	// tft.drawLine( 239, 15, 239, 304, TFT_BLUE );
	// tft.drawLine( 1, 159, 478, 159, TFT_BLUE );
	// for( int i = 9; i < 470; i += 10 )
	// 	tft.drawLine( i, 157, i, 161, TFT_BLUE );
	// for( int i = 19; i < 220; i += 10 )
	// 	tft.drawLine( 237, i, 241, i, TFT_BLUE );

	// // Draw sin-, cos- and tan-lines
	// tft.setTextColor( TFT_CYAN );
	// tft.drawString( "Sin", 5, 15, 2 );
	// for( int i = 1; i < 478; i++ ) {
	// 	tft.drawPixel( i, 159 + ( sin( ( ( i * 1.13 ) * 3.14 ) / 180 ) * 95 ), TFT_CYAN );
	// }

	// tft.setTextColor( TFT_RED );
	// tft.drawString( "Cos", 5, 30, 2 );
	// for( int i = 1; i < 478; i++ ) {
	// 	tft.drawPixel( i, 159 + ( cos( ( ( i * 1.13 ) * 3.14 ) / 180 ) * 95 ), TFT_RED );
	// }

	// tft.setTextColor( TFT_YELLOW );
	// tft.drawString( "Tan", 5, 45, 2 );
	// for( int i = 1; i < 478; i++ ) {
	// 	tft.drawPixel( i, 159 + ( tan( ( ( i * 1.13 ) * 3.14 ) / 180 ) ), TFT_YELLOW );
	// }

	// delay( WAIT );

	// tft.fillRect( 1, 15, 478 - 1, 304 - 15, TFT_BLACK );
	// tft.drawLine( 239, 15, 239, 304, TFT_BLUE );
	// tft.drawLine( 1, 159, 478, 159, TFT_BLUE );

	// // Draw a moving sinewave
	// int col = 0;
	// x       = 1;
	// for( int i = 1; i < ( 477 * 15 ); i++ ) {
	// 	x++;
	// 	if( x == 478 )
	// 		x = 1;
	// 	if( i > 478 ) {
	// 		if( ( x == 239 ) || ( buf[x - 1] == 159 ) )
	// 			col = TFT_BLUE;
	// 		else
	// 			tft.drawPixel( x, buf[x - 1], TFT_BLACK );
	// 	}
	// 	y = 159 + ( sin( ( ( i * 0.7 ) * 3.14 ) / 180 ) * ( 90 - ( i / 100 ) ) );
	// 	tft.drawPixel( x, y, TFT_BLUE );
	// 	buf[x - 1] = y;
	// }

	// delay( WAIT );

	// tft.fillRect( 1, 15, 478 - 1, 304 - 15, TFT_BLACK );

	// // Draw some filled rectangles
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
	// 	tft.fillRect( 150 + ( i * 20 ), 70 + ( i * 20 ), 60, 60, col );
	// }

	// delay( WAIT );

	// tft.fillRect( 1, 15, 478 - 1, 304 - 15, TFT_BLACK );

	// // Draw some filled, rounded rectangles
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
	// 	tft.fillRoundRect( 270 - ( i * 20 ), 70 + ( i * 20 ), 60, 60, 3, col );
	// }

	// delay( WAIT );

	// tft.fillRect( 1, 15, 478 - 1, 304 - 15, TFT_BLACK );

	// // Draw some filled circles
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
	// 	tft.fillCircle( 180 + ( i * 20 ), 100 + ( i * 20 ), 30, col );
	// }

	// delay( WAIT );

	// tft.fillRect( 1, 15, 478 - 1, 304 - 15, TFT_BLACK );

	// // Draw some lines in a pattern

	// for( int i = 15; i < 304; i += 5 ) {
	// 	tft.drawLine( 1, i, ( i * 1.6 ) - 10, 303, TFT_RED );
	// }

	// for( int i = 304; i > 15; i -= 5 ) {
	// 	tft.drawLine( 477, i, ( i * 1.6 ) - 11, 15, TFT_RED );
	// }

	// for( int i = 304; i > 15; i -= 5 ) {
	// 	tft.drawLine( 1, i, 491 - ( i * 1.6 ), 15, TFT_CYAN );
	// }

	// for( int i = 15; i < 304; i += 5 ) {
	// 	tft.drawLine( 477, i, 490 - ( i * 1.6 ), 303, TFT_CYAN );
	// }

	// delay( WAIT );

	// tft.fillRect( 1, 15, 478 - 1, 304 - 15, TFT_BLACK );

	// // Draw some random circles
	// for( int i = 0; i < 100; i++ ) {
	// 	x = 32 + random( 416 );
	// 	y = 45 + random( 226 );
	// 	r = random( 30 );
	// 	tft.drawCircle( x, y, r, random( 0xFFFF ) );
	// }

	// delay( WAIT );

	// tft.fillRect( 1, 15, 478 - 1, 304 - 15, TFT_BLACK );

	// // Draw some random rectangles
	// for( int i = 0; i < 100; i++ ) {
	// 	x  = 2 + random( 476 );
	// 	y  = 16 + random( 289 );
	// 	x2 = 2 + random( 476 );
	// 	y2 = 16 + random( 289 );
	// 	if( x2 < x ) {
	// 		r  = x;
	// 		x  = x2;
	// 		x2 = r;
	// 	}
	// 	if( y2 < y ) {
	// 		r  = y;
	// 		y  = y2;
	// 		y2 = r;
	// 	}
	// 	tft.drawRect( x, y, x2 - x, y2 - y, random( 0xFFFF ) );
	// }

	// delay( WAIT );

	// tft.fillRect( 1, 15, 478 - 1, 304 - 15, TFT_BLACK );

	// // Draw some random rounded rectangles
	// for( int i = 0; i < 100; i++ ) {
	// 	x  = 2 + random( 476 );
	// 	y  = 16 + random( 289 );
	// 	x2 = 2 + random( 476 );
	// 	y2 = 16 + random( 289 );
	// 	if( x2 < x ) {
	// 		r  = x;
	// 		x  = x2;
	// 		x2 = r;
	// 	}
	// 	if( y2 < y ) {
	// 		r  = y;
	// 		y  = y2;
	// 		y2 = r;
	// 	}
	// 	tft.drawRoundRect( x, y, x2 - x, y2 - y, 3, random( 0xFFFF ) );
	// }

	// delay( WAIT );

	// tft.fillRect( 1, 15, 478 - 1, 304 - 15, TFT_BLACK );

	// for( int i = 0; i < 100; i++ ) {
	// 	x   = 2 + random( 476 );
	// 	y   = 16 + random( 289 );
	// 	x2  = 2 + random( 476 );
	// 	y2  = 16 + random( 289 );
	// 	col = random( 0xFFFF );
	// 	tft.drawLine( x, y, x2, y2, col );
	// }

	// delay( WAIT );

	// tft.fillRect( 1, 15, 478 - 1, 304 - 15, TFT_BLACK );

	// for( int i = 0; i < 10000; i++ ) {
	// 	tft.drawPixel( 2 + random( 476 ), 16 + random( 289 ), random( 0xFFFF ) );
	// }

	// delay( WAIT );

	// tft.fillRect( 0, 0, 480, 320, TFT_BLUE );

	// tft.fillRoundRect( 160, 70, 319 - 160, 169 - 70, 3, TFT_RED );

	// tft.setTextColor( TFT_WHITE, TFT_RED );
	// tft.drawCentreString( "That's it!", CENTRE, 93, 2 );
	// tft.drawCentreString( "Restarting in a", CENTRE, 119, 2 );
	// tft.drawCentreString( "few seconds...", CENTRE, 132, 2 );

	// tft.setTextColor( TFT_GREEN, TFT_BLUE );
	// tft.drawCentreString( "Runtime: (msecs)", CENTRE, 280, 2 );
	// tft.setTextDatum( TC_DATUM );
	// runTime = millis( ) - runTime;
	// tft.drawNumber( runTime, CENTRE, 300, 2 );
	// tft.setTextDatum( TL_DATUM );
	// delay( 10000 );
}

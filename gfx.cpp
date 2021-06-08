#include <TFT_eSPI.h>

#define CENTER_X 240
#define CENTER_Y 160


// void DrawTextBox( TFT_eSPI *tft, ) {
// 	tft->setTextColor( TFT_RED );
// 	tft->drawString( "Cos", 5, 30, 2 );

// 	// tft->fillRect( 150 + ( i * 20 ), 70 + ( i * 20 ), 60, 60, col );
// }

#define NOTICE_RECT_X_FRACTION 5
#define NOTICE_RECT_X 480 / NOTICE_RECT_X_FRACTION
#define NOTICE_RECT_Y 320 / 3
#define NOTICE_RECT_X_WIDTH NOTICE_RECT_X *( NOTICE_RECT_X_FRACTION - 2 )
void DrawNotice( TFT_eSPI *tft, const char * text ) {
	tft->setTextColor( TFT_WHITE );
	tft->setFreeFont( &FreeMono9pt7b );
    // tft->drawString( text, 5, 30 );

	// x    y   width   height  color
	tft->fillRect( NOTICE_RECT_X, NOTICE_RECT_Y, NOTICE_RECT_X_WIDTH, NOTICE_RECT_Y, TFT_RED );
	// tft->drawString( text, NOTICE_RECT_X, 30 );
	tft->drawCentreString( text, CENTER_X, CENTER_Y-4, 1 );
}


// int16_t TFT_eSPI::DrawNotice( const String &string, int32_t dX, int32_t poY, uint8_t font ) {
// 	int16_t len = string.length( ) + 2;
// 	char    buffer[len];
// 	string.toCharArray( buffer, len );
// 	return drawCentreString( buffer, dX, poY, font );
// }
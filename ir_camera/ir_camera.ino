// #include "./tft_espi_user_setup.h"
//#include <User_Setup_Select.h>
// #include <TFT_eSPI.h>

#include <Arduino.h>

#include <Adafruit_MLX90640.h>




/*  
 Test the tft.print() viz the libraries embedded write() function

 This sketch used font 2, 4, 7
 
 Make sure all the required fonts are loaded by editing the
 User_Setup.h file in the TFT_eSPI library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
 */

#include <SPI.h>

#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define TFT_GREY 0x5AEB // New colour




Adafruit_MLX90640 mlx;
float frame[32*24]; // buffer for full frame of temperatures

// uncomment *one* of the below
//#define PRINT_TEMPERATURES
#define PRINT_ASCIIART




//low range of the sensor (this will be blue on the screen)
#define MINTEMP 20.0
// float MINTEMP = 20;

//high range of the sensor (this will be red on the screen)
#define MAXTEMP 35.0
// float MAXTEMP = 35;

//the colors we will be using
const uint16_t camColors[] = {0x480F,
0x400F,0x400F,0x400F,0x4010,0x3810,0x3810,0x3810,0x3810,0x3010,0x3010,
0x3010,0x2810,0x2810,0x2810,0x2810,0x2010,0x2010,0x2010,0x1810,0x1810,
0x1811,0x1811,0x1011,0x1011,0x1011,0x0811,0x0811,0x0811,0x0011,0x0011,
0x0011,0x0011,0x0011,0x0031,0x0031,0x0051,0x0072,0x0072,0x0092,0x00B2,
0x00B2,0x00D2,0x00F2,0x00F2,0x0112,0x0132,0x0152,0x0152,0x0172,0x0192,
0x0192,0x01B2,0x01D2,0x01F3,0x01F3,0x0213,0x0233,0x0253,0x0253,0x0273,
0x0293,0x02B3,0x02D3,0x02D3,0x02F3,0x0313,0x0333,0x0333,0x0353,0x0373,
0x0394,0x03B4,0x03D4,0x03D4,0x03F4,0x0414,0x0434,0x0454,0x0474,0x0474,
0x0494,0x04B4,0x04D4,0x04F4,0x0514,0x0534,0x0534,0x0554,0x0554,0x0574,
0x0574,0x0573,0x0573,0x0573,0x0572,0x0572,0x0572,0x0571,0x0591,0x0591,
0x0590,0x0590,0x058F,0x058F,0x058F,0x058E,0x05AE,0x05AE,0x05AD,0x05AD,
0x05AD,0x05AC,0x05AC,0x05AB,0x05CB,0x05CB,0x05CA,0x05CA,0x05CA,0x05C9,
0x05C9,0x05C8,0x05E8,0x05E8,0x05E7,0x05E7,0x05E6,0x05E6,0x05E6,0x05E5,
0x05E5,0x0604,0x0604,0x0604,0x0603,0x0603,0x0602,0x0602,0x0601,0x0621,
0x0621,0x0620,0x0620,0x0620,0x0620,0x0E20,0x0E20,0x0E40,0x1640,0x1640,
0x1E40,0x1E40,0x2640,0x2640,0x2E40,0x2E60,0x3660,0x3660,0x3E60,0x3E60,
0x3E60,0x4660,0x4660,0x4E60,0x4E80,0x5680,0x5680,0x5E80,0x5E80,0x6680,
0x6680,0x6E80,0x6EA0,0x76A0,0x76A0,0x7EA0,0x7EA0,0x86A0,0x86A0,0x8EA0,
0x8EC0,0x96C0,0x96C0,0x9EC0,0x9EC0,0xA6C0,0xAEC0,0xAEC0,0xB6E0,0xB6E0,
0xBEE0,0xBEE0,0xC6E0,0xC6E0,0xCEE0,0xCEE0,0xD6E0,0xD700,0xDF00,0xDEE0,
0xDEC0,0xDEA0,0xDE80,0xDE80,0xE660,0xE640,0xE620,0xE600,0xE5E0,0xE5C0,
0xE5A0,0xE580,0xE560,0xE540,0xE520,0xE500,0xE4E0,0xE4C0,0xE4A0,0xE480,
0xE460,0xEC40,0xEC20,0xEC00,0xEBE0,0xEBC0,0xEBA0,0xEB80,0xEB60,0xEB40,
0xEB20,0xEB00,0xEAE0,0xEAC0,0xEAA0,0xEA80,0xEA60,0xEA40,0xF220,0xF200,
0xF1E0,0xF1C0,0xF1A0,0xF180,0xF160,0xF140,0xF100,0xF0E0,0xF0C0,0xF0A0,
0xF080,0xF060,0xF040,0xF020,0xF800,};

uint16_t displayPixelWidth, displayPixelHeight, leftMargin, topMargin;











uint16_t counter = 0;
uint16_t tftWidth = TFT_WIDTH;
uint16_t tftHeight = TFT_HEIGHT;
double frameMax = MINTEMP;
double frameMin = MAXTEMP;

void loop_ir (){
    uint32_t timestamp = millis();
    if (mlx.getFrame(frame) != 0) {
        Serial.println("Failed");
        return;
    }

    tft.startWrite();
    for (uint8_t h=0; h<24; h++) {
        break;
        for (uint8_t w=0; w<32; w++) {
        double t = frame[h*32 + w];

        frameMax = max( frameMax, t );
        frameMin = min( frameMin, t );

        t = min(t, MAXTEMP);
        t = max(t, MINTEMP); 
            
        uint8_t colorIndex = map(t, MINTEMP, MAXTEMP, 0, 255);
        
        colorIndex = constrain(colorIndex, 0, 255);
        //draw the pixels!
        tft.fillRect( leftMargin + (displayPixelWidth * w), topMargin + (displayPixelHeight * h),
                                displayPixelHeight, displayPixelWidth, 
                                camColors[colorIndex]);
        }
    }
    // KILL
    
    tft.fillRect( leftMargin, topMargin,
                            (displayPixelWidth * 32), (displayPixelHeight * 24), 
                            TFT_CYAN);

    if ( counter % 6 == 0 ){
        tft.setCursor(tftWidth - leftMargin, 20);
        tft.print(1000.0 / (millis()-timestamp));

        tft.setCursor(tftWidth - leftMargin - 0, tftHeight - (15 * 3) - 0);
        tft.print( frameMax );
        tft.setCursor(tftWidth - leftMargin - 0, tftHeight - (15 * 1) - 0);
        tft.print(frameMin);

        frameMax = MINTEMP;
        frameMin = MAXTEMP;
    }

    if ( counter == 60 ){
        Serial.print((millis()-timestamp) / 2); Serial.println(" ms per frame (2 frames per display)");
        Serial.print(2000.0 / (millis()-timestamp)); Serial.println(" FPS (2 frames per display)");
        Serial.print("Min: "); Serial.print(frameMin); Serial.print(" ; Max: "); Serial.println(frameMax);
        // diagnostics
        // tft.setCursor( 50, 40);
        // tft.println("TEST TEXT");
        // tft.setCursor( 50, 60);
        // tft.println(tftWidth - leftMargin);
        // tft.setCursor( 50, 80);
        // tft.println(tftHeight - (15 * 4));
        counter = 0;
    }
    tft.endWrite();
    counter++;
}


void setup_tft(void) {
    tft.init();
    tft.setRotation(3);
    if ( tft.getRotation() % 2 == 1 ){
        tftHeight = TFT_WIDTH;
        tftWidth = TFT_HEIGHT;
    }

    String status = "Setting up TFT_eSPI. Width: ";
    status += tft.width();
    Serial.print( status );
    //   Serial.print(tft.width(), DEC);
    Serial.print(" ; Height: ");
    Serial.print(tft.height(), DEC);
    Serial.print(" ; SPI Speed: ");
    Serial.print(SPI_FREQUENCY, DEC);

    displayPixelWidth = min(tft.height() / 24, tft.width() / 32);
    displayPixelHeight = displayPixelWidth;
    leftMargin = ( tft.width() - (displayPixelWidth * 32 ) ) / 2;
    topMargin = ( tft.height() - (displayPixelHeight * 24 ) ) / 2;

    // tft.fillScreen(random(0xFFFF));
    tft.fillScreen(0x9245);
    
    // Set "cursor" at top left corner of display (0,0) and select font 2
    // (cursor will move to next line automatically during printing with 'tft.println'
    //  or stay on the line is there is room for the text with tft.print)
    //   tft.setTextFont(4);
    tft.setTextFont(1);
    tft.setCursor(0, tft.height() / 2 - 10, 4);
    // Set the font colour to be white with a black background, set text size multiplier to 1
    tft.setTextColor(TFT_WHITE, TFT_BLACK);  tft.setTextSize(1);
    //   tft.setTextFont(GFXFF);
    tft.setFreeFont(&FreeMono12pt7b);
    // We can now plot text on screen using the "print" class
    tft.println("Starting display.");
    tft.printf("Display Size: %i x %i\n", displayPixelWidth, displayPixelHeight);
    tft.printf("Pixel Size: %i x %i\n", tft.width(), tft.height());
    tft.printf("Margin: %i\n", leftMargin);




    
  tft.setTextFont(NULL);     // Select the orginal small GLCD font by using NULL or GLCD
  tft.println();             // Move cursor down a line
  tft.print("Original GLCD font");    // Print the font name onto the TFT screen
  tft.println();
  tft.println();


  tft.setTextFont(1);     // Select the orginal small GLCD font by using NULL or GLCD
  tft.setFreeFont(&FreeMono12pt7b);
  tft.println();          // Free fonts plot with the baseline (imaginary line the letter A would sit on)
  // as the datum, so we must move the cursor down a line from the 0,0 position
  tft.print("FreeMono12pt7b");  // Print the font name onto the TFT screen



    delay(5000);
    tft.fillScreen(0x7cff);
    delay(1000);
    tft.fillScreen(0x0aaf);
    delay(1000);
    tft.fillScreen(0xf020);
    delay(1000);
    // tft.fillScreen(0xFFFF);
    tft.setFreeFont(&FreeMono9pt7b);   // Select Free Serif 9 point font, could use:
    tft.fillScreen(TFT_DARKGREY);

    
    //   tft.setTextFont(1);
    //   tft.setTextFont( *FreeMono12pt7bBitmaps );
    //   tft.setTextFont(NULL);     // Select the orginal small GLCD font by using NULL or GLCD
    for( int i = 1; i <= 24; i++ ){
        tft.setCursor(0, topMargin + i * displayPixelHeight);
        tft.println(i);
    }
    
    tft.setCursor(tftWidth - leftMargin, topMargin+displayPixelHeight);
    tft.print("FPS");
    
    tft.setCursor(tftWidth - leftMargin - 0, tftHeight - (15 * 2) - 0);
    tft.print("Min:");
    
    tft.setCursor(tftWidth - leftMargin - 0, tftHeight - (15 * 4) - 0);
    tft.print("Max:");
    tft.setTextFont(1);
}













void setup() {
        // Serial.begin
    //   while (!Serial) delay(10);
    Serial.begin(115200);
    delay(100);

    Serial.println("Adafruit MLX90640 Simple Test");
    if (! mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
        Serial.println("MLX90640 not found!");
        while (1) delay(10);
    }
    Serial.println("Found Adafruit MLX90640");

    //   Serial.println("Waiting for user input.");
        //Serial.setTimeout( 20000 );
    //   while (Serial.available() == 0){
    //     Serial.print(".");
    //     delay(1000);
    //   }
        // delay(5000);
    // read the incoming string:
    //   String incomingString = Serial.readString();
    // prints the received data
    //   Serial.print("Received input: ");
    //   Serial.println(incomingString);
    //   delay(10);



    Serial.print("Serial number: ");
    Serial.print(mlx.serialNumber[0], HEX);
    Serial.print(mlx.serialNumber[1], HEX);
    Serial.println(mlx.serialNumber[2], HEX);
    
    //mlx.setMode(MLX90640_INTERLEAVED);
    mlx.setMode(MLX90640_CHESS);
    mlx.setResolution(MLX90640_ADC_18BIT);
    mlx.setRefreshRate(MLX90640_32_HZ);
    // mlx.setRefreshRate(MLX90640_8_HZ);
    Wire.setClock(4000000); // max 1 MHz


    Serial.print("Current mode: ");
    if (mlx.getMode() == MLX90640_CHESS) {
        Serial.println("Chess");
    } else {
        Serial.println("Interleave");    
    }

    Serial.print("Current resolution: ");
    mlx90640_resolution_t res = mlx.getResolution();
    switch (res) {
        case MLX90640_ADC_16BIT: Serial.println("16 bit"); break;
        case MLX90640_ADC_17BIT: Serial.println("17 bit"); break;
        case MLX90640_ADC_18BIT: Serial.println("18 bit"); break;
        case MLX90640_ADC_19BIT: Serial.println("19 bit"); break;
    }

    Serial.print("Current frame rate: ");
    mlx90640_refreshrate_t rate = mlx.getRefreshRate();
    switch (rate) {
        case MLX90640_0_5_HZ: Serial.println("0.5 Hz"); break;
        case MLX90640_1_HZ: Serial.println("1 Hz"); break; 
        case MLX90640_2_HZ: Serial.println("2 Hz"); break;
        case MLX90640_4_HZ: Serial.println("4 Hz"); break;
        case MLX90640_8_HZ: Serial.println("8 Hz"); break;
        case MLX90640_16_HZ: Serial.println("16 Hz"); break;
        case MLX90640_32_HZ: Serial.println("32 Hz"); break;
        case MLX90640_64_HZ: Serial.println("64 Hz"); break;
    }

    setup_tft();
    //   delay(5000);
}

void loop() {
  loop_ir();
//   return;
//   delay(500);
//   if (mlx.getFrame(frame) != 0) {
//     Serial.println("Failed");
//     return;
//   }
//   Serial.println();
//   Serial.println();
//   for (uint8_t h=0; h<24; h++) {
//     for (uint8_t w=0; w<32; w++) {
//       float t = frame[h*32 + w];
// #ifdef PRINT_TEMPERATURES
//       Serial.print(t, 1);
//       Serial.print(", ");
// #endif
// #ifdef PRINT_ASCIIART
//       char c = '&';
//       if (t < 20) c = ' ';
//       else if (t < 23) c = '.';
//       else if (t < 25) c = '-';
//       else if (t < 27) c = '*';
//       else if (t < 29) c = '+';
//       else if (t < 31) c = 'x';
//       else if (t < 33) c = '%';
//       else if (t < 35) c = '#';
//       else if (t < 37) c = 'X';
//       Serial.print(c);
// #endif
//     }
//     Serial.println();
//   }
//   loop_tft();
}






// void loop_tft(void) {
//   Serial.println("looping for TFT_eSPI");

//   testFilledRects(TFT_YELLOW, TFT_MAGENTA);

//   delay(10000);
  
//   // Fill screen with random colour so we can see the effect of printing with and without 
//   // a background colour defined
//   tft.fillScreen(random(0xFFFF));
  
//   // Set "cursor" at top left corner of display (0,0) and select font 2
//   // (cursor will move to next line automatically during printing with 'tft.println'
//   //  or stay on the line is there is room for the text with tft.print)
//   tft.setCursor(0, 0, 2);
//   // Set the font colour to be white with a black background, set text size multiplier to 1
//   tft.setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(1);
//   // We can now plot text on screen using the "print" class
//   tft.println("Hello World!");
  
//   // Set the font colour to be yellow with no background, set to font 7
//   tft.setTextColor(TFT_YELLOW); tft.setTextFont(7);
//   tft.println(1234.56);
  
//   // Set the font colour to be red with black background, set to font 4
//   tft.setTextColor(TFT_RED,TFT_BLACK);    tft.setTextFont(4);
//   tft.println((long)3735928559, HEX); // Should print DEADBEEF

//   // Set the font colour to be green with black background, set to font 4
//   tft.setTextColor(TFT_GREEN,TFT_BLACK);
//   tft.setTextFont(4);
//   tft.println("Groop");
//   tft.println("I implore thee,");

//   // Change to font 2
//   tft.setTextFont(2);
//   tft.println(F("my foonting turlingdromes.")); // Can store strings in FLASH to save RAM
//   tft.println("And hooptiously drangle me");
//   tft.println("with crinkly bindlewurdles,");
//   // This next line is deliberately made too long for the display width to test
//   // automatic text wrapping onto the next line
//   tft.println("Or I will rend thee in the gobberwarts with my blurglecruncheon, see if I don't!");
  
//   // Test some print formatting functions
//   float fnumber = 123.45;
//    // Set the font colour to be blue with no background, set to font 4
//   tft.setTextColor(TFT_BLUE);    tft.setTextFont(4);
//   tft.print("Float = "); tft.println(fnumber);           // Print floating point number
//   tft.print("Binary = "); tft.println((int)fnumber, BIN); // Print as integer value in binary
//   tft.print("Hexadecimal = "); tft.println((int)fnumber, HEX); // Print as integer number in Hexadecimal
//   delay(10000);
// }



// unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
//   unsigned long start, t = 0;
//   int           n, i, i2,
//                 cx = tft.width()  / 2 - 1,
//                 cy = tft.height() / 2 - 1;

//   tft.fillScreen(TFT_BLACK);
//   n = min(tft.width(), tft.height());
//   for (i = n - 1; i > 0; i -= 6) {
//     i2    = i / 2;
//     start = micros();
//     tft.fillRect(cx - i2, cy - i2, i, i, color1);
//     t    += micros() - start;
//     // Outlines are not included in timing results
//     tft.drawRect(cx - i2, cy - i2, i, i, color2);
//   }

//   return t;
// }





// void setup() {
//   // put your setup code here, to run once:

// }

// void loop() {
//   // put your main code here, to run repeatedly:

// }

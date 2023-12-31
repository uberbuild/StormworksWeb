﻿#include <LiquidCrystal.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#define TFT_CS 10  //Chip select line
#define TFT_RST 8  // Reset line
#define TFT_DC 9   // Data/Command line
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
float kmh = 0;
float alt = 0;
float mph = 0;
float roll = 0;
float pitch = 0;
float up = 0;
String serialData = "";
float bng = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte shape[8] = {0b11100,0b10100,0b11100,0b00000,0b00000,0b00000,0b00000,0b00000};
int centreX = 64; // Adjust as needed
int centreY = 80; // Adjust as needed
int offset = 80;

class Point {
public:
    int X;
    int Y;

    Point(int x, int y) : X(x), Y(y) {}

    // Rotate the point around another point by a given angle in radians
    Point rotate(float angle, const Point& center) const {
        float s = sin(angle);
        float c = cos(angle);

        // Translate the point so that the center becomes the origin
        int translatedX = X - center.X;
        int translatedY = Y - center.Y;

        // Perform the rotation
        int newX = int(translatedX * c - translatedY * s);
        int newY = int(translatedX * s + translatedY * c);

        // Translate the point back to its original position
        newX += center.X;
        newY += center.Y;

        return Point(newX, newY);
    }
};
class Quad
{
public:
    Point P1;
    Point P2;
    Point P3;
    Point P4;

    Quad(Point p1, Point p2, Point p3, Point p4) : P1(p1), P2(p2), P3(p3), P4(p4) {}

    // Rotate the quadrilateral around a given point by a given angle in radians
    Quad rotate(float angle, const Point &center) const
    {
        return Quad(P1.rotate(angle, center), P2.rotate(angle, center), P3.rotate(angle, center), P4.rotate(angle, center));
    }

    void draw(uint16_t colour = ST7735_ORANGE)
    {
        tft.fillTriangle(P1.X, P1.Y, P2.X, P2.Y, P3.X, P3.Y, colour);
        tft.fillTriangle(P1.X, P1.Y, P3.X, P3.Y, P4.X, P4.Y, colour);
    }
};

float angle = 0;

//setup variables here, kmh, mph, bng and alt
void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.createChar(0, shape);
  Serial.setTimeout(100);
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_CYAN);
}

void loop() {
  if (Serial.available()) {
    serialData = Serial.readString(); // Read the incoming data as a string
    splitData(serialData);
  } else {
    delay(100);
  }
  String kmhst = String(kmh, 1) + "kmh";
  String altst = String(alt, 1) + "m";
  String mphst = String(mph, 1) + "mph";
  String bngst = String(bng, 0);

  int spacesforvar1 = 16 - kmhst.length() - altst.length();
  for (int i = 0; i< spacesforvar1; i++) {
    kmhst += " ";
  }
  int spacesforvar2 = 16 - mphst.length() - bngst.length() - 1;
  for (int i = 0; i< spacesforvar2; i++) {
    mphst += " ";
  }
  String line1 = kmhst + altst;
  String line2 = mphst + bngst ;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
  lcd.setCursor(15,1);
  lcd.write(byte(0));
  centreY = 80+(320*pitch);
  // angle += 0.2;
  Point p1(-offset, centreY);
  Point p2(-offset, centreY * 2 + offset);
  Point p3(2 * centreX + offset, centreY * 2 + offset);
  Point p4(2 * centreX + offset, centreY);
  Point p5(2 * centreX + offset, -offset);
  Point p6(-offset, -offset);
  Point p7(p1.X, p1.Y + 5);
  Point p8(p4.X, p4.Y + 5);
  Point midPoint(centreX, centreY);
  Quad ground = Quad(p1, p2, p3, p4);
  Quad sky = Quad(p1, p4, p5, p6);
  // Quad line = Quad(p1, p4, p7, p8);
  sky.rotate(roll, midPoint).draw(ST7735_CYAN);
  ground.rotate(roll, midPoint).draw();
//     line.rotate(angle, midPoint).draw(ST7735_BLACK);
}

void splitData(String data) {
  int separator1 = data.indexOf(':');
  int separator2 = data.indexOf(':', separator1 + 1);
  int separator3 = data.indexOf(':', separator2 + 1);
  int separator4 = data.indexOf(':', separator3 + 1);
  int separator5 = data.indexOf(':', separator4 + 1);
  int separator6 = data.indexOf(':', separator5 + 1);

  if (separator1 != -1 && separator2 != -1 && separator3 != -1 && separator4 != -1 && separator5 != -1 && separator6 != -1) {
    kmh = data.substring(0, separator1).toFloat();
    mph = data.substring(separator1 + 1, separator2).toFloat();
    alt = data.substring(separator2 + 1, separator3).toFloat();
    bng = data.substring(separator3 + 1, separator4).toFloat();
    pitch = data.substring(separator4 + 1, separator5).toFloat();
    roll = data.substring(separator5 + 1, separator6).toFloat();
    up = data.substring(separator6 + 1).toFloat();
  }
}
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
float kmh = 0;
float alt = 0;
float mph = 0;
String serialData = "";
float bng = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte shape[8] = {0b11100,0b10100,0b11100,0b00000,0b00000,0b00000,0b00000,0b00000};

//setup variables here, kmh, mph, bng and alt
void setup() {
    lcd.begin(16, 2);
    Serial.begin(9600);
    lcd.createChar(0, shape);
    Serial.setTimeout(100);
}

void loop() {
    if (Serial.available()) {
        serialData = Serial.readString(); // Read the incoming data as a string
        splitData(serialData);
    } else {
        delay(100);
    }
    String kmhst = String(kmh, 1) + "kmh";
    String altst = String(alt, 1) + "l";
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

}
void splitData(String data) {
    int separator1 = data.indexOf(':');
    int separator2 = data.indexOf(':', separator1 + 1);
    int separator3 = data.indexOf(':', separator2 + 1);

    if (separator1 != -1 && separator2 != -1 && separator3 != -1) {
        kmh = data.substring(0, separator1).toFloat();
        mph = data.substring(separator1 + 1, separator2).toFloat();
        alt = data.substring(separator2 + 1, separator3).toFloat();
        bng = data.substring(separator3 + 1).toFloat();
    }
}

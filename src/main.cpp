#include <Arduino.h>
#include <SoftwareSerial.h>
#include <JC_Button.h> // https://github.com/JChristensen/JC_Button

const int shootPin = PB0;
const int shootBtnPin = PB1;
const int chargePin = PB2;

const int rf1TxPin = PB3;
const int rf1RxPin = PB4;

const int shootDelay = 20;

SoftwareSerial HC12(rf1RxPin, rf1TxPin);

String hc12String;
String substring;

Button shootBtn(shootBtnPin);

#define BAUDRATE 115200
#define RF_BAUDRATE 4800

// function declaration
String midString(String str, String start, String finish);
void shoot();

void setup()
{
    pinMode(shootBtnPin, INPUT);
    pinMode(chargePin, OUTPUT);
    pinMode(shootPin, OUTPUT);

    HC12.begin(RF_BAUDRATE);
    shootBtn.begin();
}

void loop()
{

    shootBtn.read();

    digitalWrite(chargePin, HIGH);

    if (shootBtn.wasPressed())
    {
        shoot();
    }

    while (HC12.available())
    {
        hc12String = HC12.readString();
    }

    substring = midString(hc12String, "_", "_");

    if (substring == "BUTTON1")
    {
        HC12.print("_LED_");
        shoot();

    }

    // empty the string after processing the data
    hc12String = "";
    substring = "";
}

void shoot()
{

    digitalWrite(chargePin, LOW);

    digitalWrite(shootPin, HIGH);
    delay(shootDelay);
    digitalWrite(shootPin, LOW);

    delay(3000);
    digitalWrite(chargePin, HIGH);
}

/**
 * Strip substring between two delimiters
 */
String midString(String str, String start, String finish)
{
    int locStart = str.indexOf(start);
    if (locStart == -1)
    {
        return "";
    }

    locStart += start.length();
    int locFinish = str.indexOf(finish, locStart);

    if (locFinish == -1)
    {
        return "";
    }

    return str.substring(locStart, locFinish);
}
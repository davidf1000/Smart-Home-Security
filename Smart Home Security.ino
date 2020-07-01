/*********Tugas Besar PRD *********/
/*********Source Code Belongs to David Fauzi  *********/
/*********https://github.com/davidf1000  *********/
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
int buzz = 12; //d7
int val = 0;

//#include <ESP8266HTTPClient.h>

#define REED_SWITCH 14 //D5
int red = 4;
int green = 0;
int blue = 2;
int status = WL_IDLE_STATUS; //not required.
int smokepin = 13;           //d7
const char *ssid = "david";
const char *password = "12345678";
int doorClosed = 1;
int smoke = 1;
void setup()
{
    digitalWrite(blue, HIGH);
    pinMode(REED_SWITCH, INPUT_PULLUP);
    pinMode(smokepin, INPUT);
    pinMode(buzz, OUTPUT);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue, OUTPUT);
    Serial.begin(115200);

    setupWifi();

    get_http3();
}

void setupWifi()
{
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    Serial.println("program untuk menyambung ke wifi");
    delay(2000);
    WiFi.mode(WIFI_STA);
    status = WiFi.begin(ssid, password);
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Connected to wifi");
    digitalWrite(green, HIGH);
}

void loop()
{
    digitalWrite(blue, LOW);
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
    if (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(green, LOW);
        setupWifi();
    }
    val = digitalRead(REED_SWITCH);
    smoke = digitalRead(smokepin);
    Serial.println(val);
    Serial.println(smoke);

    delay(2500);
    Serial.println("iniloop");
    // put your main code here, to run repeatedly:
    if (digitalRead(REED_SWITCH) == HIGH and not(digitalRead(smokepin))) //SMOKE AND DOOR
    {
        digitalWrite(green, LOW);
        digitalWrite(red, HIGH);
        Serial.println("DOOR OPEN!! and SMOKE ALERT !");
        tone(buzz, 500); // Send 1KHz sound signal...
        get_http2();
        delay(500);
        tone(buzz, 500);
        delay(500);
        tone(buzz, 1000);
        delay(500);
        tone(buzz, 500);
        delay(4000);
        noTone(buzz);
    }
    else if ((not(digitalRead(REED_SWITCH) == HIGH)) and not(digitalRead(smokepin))) //SMOKE
    {
        digitalWrite(green, LOW);
        digitalWrite(red, HIGH);
        Serial.println("SMOKE ALERT !");
        tone(buzz, 500); // Send 1KHz sound signal...
        get_http1();
        delay(500);
        tone(buzz, 500);
        delay(500);
        tone(buzz, 1000);
        delay(500);
        tone(buzz, 500);
        delay(4000);
        noTone(buzz);
    }
    else if ((digitalRead(REED_SWITCH) == HIGH) and (digitalRead(smokepin))) //DOOR
    {
        digitalWrite(green, LOW);
        digitalWrite(red, HIGH);
        Serial.println("DOOR OPEN !");
        tone(buzz, 500); // Send 1KHz sound signal...
        get_http();
        delay(500);
        tone(buzz, 500);
        delay(500);
        tone(buzz, 1000);
        delay(500);
        tone(buzz, 500);
        delay(4000);
        noTone(buzz);
    }
    else
    {
        Serial.println("nothing happened!!");
    }
    Serial.println("check is running");
}

int get_http2() //SMOKE AND DOOR
{
    HTTPClient http;
    int ret = 0;
    Serial.print("[HTTP] begin...\n");
    // configure ifttt server and url  should be HTTP only..not https!!!  (http://)
    http.begin("http://maker.ifttt.com/trigger/smoke&door/with/key/elRjQawsd4tGh-upbaMZeMmCSPySp4dXW6Y3OHSqJ9i"); //HTTP
                                                                                                                  //If you have enabled value1 from iftt settings then uncomment below line to send value and comment out above line
                                                                                                                  //http.begin("http://maker.ifttt.com/trigger/smoke&door/with/key/your_key_from_Iftt/?value1="+state); //HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            Serial.println(payload);
        }
    }
    else
    {
        ret = -1;
        Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        delay(500); // wait for half sec before retry again
    }

    http.end();
}
int get_http1() //JUST SMOKE
{
    HTTPClient http;
    int ret = 0;
    Serial.print("[HTTP] begin...\n");
    // configure ifttt server and url  should be HTTP only..not https!!!  (http://)
    http.begin("http://maker.ifttt.com/trigger/smoke/with/key/elRjQawsd4tGh-upbaMZeMmCSPySp4dXW6Y3OHSqJ9i"); //HTTP
                                                                                                             //If you have enabled value1 from iftt settings then uncomment below line to send value and comment out above line
                                                                                                             //http.begin("http://maker.ifttt.com/trigger/door/with/key/your_key_from_Iftt/?value1="+state); //HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            Serial.println(payload);
        }
    }
    else
    {
        ret = -1;
        Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        delay(500); // wait for half sec before retry again
    }

    http.end();
}
int get_http() //DOOR
{
    HTTPClient http;
    int ret = 0;
    Serial.print("[HTTP] begin...\n");
    // configure ifttt server and url  should be HTTP only..not https!!!  (http://)
    http.begin("http://maker.ifttt.com/trigger/door/with/key/elRjQawsd4tGh-upbaMZeMmCSPySp4dXW6Y3OHSqJ9i"); //HTTPhttp://maker.ifttt.com/trigger/door/with/key/elRjQawsd4tGh-upbaMZeMmCSPySp4dXW6Y3OHSqJ9i
                                                                                                            //If you have enabled value1 from iftt settings then uncomment below line to send value and comment out above line
                                                                                                            //http.begin("http://maker.ifttt.com/trigger/door/with/key/your_key_from_Iftt/?value1="+state); //HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            Serial.println(payload);
        }
    }
    else
    {
        ret = -1;
        Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        delay(500); // wait for half sec before retry again
    }

    http.end();
}
int get_http3() //cek
{
    HTTPClient http;
    int ret = 0;
    Serial.print("[HTTP] begin...\n");
    // configure ifttt server and url  should be HTTP only..not https!!!  (http://)
    http.begin("http://maker.ifttt.com/trigger/cek/with/key/elRjQawsd4tGh-upbaMZeMmCSPySp4dXW6Y3OHSqJ9i"); //HTTPhttp://maker.ifttt.com/trigger/door/with/key/elRjQawsd4tGh-upbaMZeMmCSPySp4dXW6Y3OHSqJ9i
                                                                                                           //If you have enabled value1 from iftt settings then uncomment below line to send value and comment out above line
                                                                                                           //http.begin("http://maker.ifttt.com/trigger/door/with/key/your_key_from_Iftt/?value1="+state); //HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            Serial.println(payload);
        }
    }
    else
    {
        ret = -1;
        Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        delay(500); // wait for half sec before retry again
    }

    http.end();
}

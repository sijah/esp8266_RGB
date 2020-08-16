#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>

ESP8266WebServer server(80);
uint8_t led1 = D1; //RED
uint8_t led2 = D2; //GREEN
uint8_t led3 = D3; //BLUE

const char *ssid = "o2";    //CABIN1 //o2
const char *pass = "asdfghjkl"; //asdfghjkl

void handle_root()
{
    File p = LittleFS.open("/index.html", "r");
    server.streamFile(p, "text/html");
}
void handle_notfound()
{

    server.send(404, "text/html", "jango njan pettu");
}

void handle_rgb()
{
    String red = server.arg("r");
    String green = server.arg("g");
    String blue = server.arg("b");
    int r = red.toInt();
    int g = green.toInt();
    int b = blue.toInt();

    Serial.print("Red:");
    Serial.println(r);
    Serial.print("Green:");
    Serial.println(g);
    Serial.print("Blue:");
    Serial.println(b);

    server.send(302, "text/plane", "ok");

    int r1 = map(r, 0, 255, 0, 1024);
    int g1 = map(g, 0, 255, 0, 1024);
    int b1 = map(b, 0, 255, 0, 1024);
    analogWrite(led1, r1);
    analogWrite(led2, g1);
    analogWrite(led3, b1);
}

/*void setColor(int red, int green, int blue)
{
    analogWrite(led1, red);
    analogWrite(led2, green);
    analogWrite(led3, blue);
}

void color_change()
{
    
        setColor(255, 0, 0); // Red Color
        delay(1000);
        setColor(0, 255, 0); // Green Color
        delay(1000);
        setColor(0, 0, 255); // Blue Color
        delay(1000);
        setColor(255, 255, 255); // White Color
        delay(1000);
        setColor(170, 0, 255); // Purple Color
        delay(1000);
    
    server.send(200, "text/html", "ok");
}*/

void setup()
{
    Serial.begin(115200);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);

    if (LittleFS.begin())
    {
        Serial.print("littlefson");
    }
    else
    {
        Serial.print("littlefs no working");
    }
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }

    if (!MDNS.begin("esp8266"))
    {
        Serial.println("Error setting up MDNS responder!");
        while (1)
        {
            delay(1000);
        }
    }
    server.on("/", handle_root);
    server.on("/RGB", handle_rgb);
    //server.on("/colorchange", color_change);
    server.onNotFound(handle_notfound);
    server.begin();
}

void loop()
{
    server.handleClient();
    MDNS.update();
}
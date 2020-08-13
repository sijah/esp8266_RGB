#include <Arduino.h>
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<ESP8266mDNS.h>
#include<LittleFS.h>

ESP8266WebServer server(80);
uint8_t led1 = D1;
uint8_t led2 = D2;
uint8_t led3 = D3;

const char *ssid="o2";//CABIN1 //o2
const char *pass="asdfghjkl";//asdfghjkl

void handle_root() {
    File p = LittleFS.open("/index.html", "r");
    server.streamFile(p, "text/html");

}
void handle_notfound()
{

    server.send(404, "text/html", "jango njan pettu");
}

void handle_rgb() {
    String red = server.arg("r");
    String green = server.arg("g");
    String blue = server.arg("b");
    int r = red.toInt();
    int g = green.toInt();
    int b = blue.toInt();

    Serial.print("Red:");Serial.println(r);
    Serial.print("Green:");Serial.println(g);
    Serial.print("Blue:");Serial.println(b);

    server.send(200, "text/plane", red);

    int r1 =map(r, 0, 100, 0, 1024);
    int g1 =map(g, 0, 100, 0, 1024);
    int b1 =map(b, 0, 100, 0, 1024);
    analogWrite(led3, r1);
    analogWrite(led2, g1);
    analogWrite(led1, b1);
}



void setup() {
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
    server.onNotFound(handle_notfound);
    server.begin();



}

void loop() {
    server.handleClient();
    MDNS.update();
}
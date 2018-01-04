#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "FS.h"
#include <OTAHelper.h>

#include "SceneHelper.h"

#define LED_STRIP_PIN D1
#define LED_STRIP_TYPE NEO_GRB + NEO_KHZ800
#define PIXEL_COUNT 120
#define DEVICE_NAME "LED Strip"
#define ANIMATION_TIME 1000

OTAHelper otaHelper(DEVICE_NAME);
SceneHelper sceneHelper(DEVICE_NAME);
Adafruit_NeoPixel strip(PIXEL_COUNT, LED_STRIP_PIN, LED_STRIP_TYPE);
NeoPixelAnimator animations(1);
RgbColor startColor(0, 0, 0);
RgbColor targetColor(0, 0, 0);

void setup() {
    Serial.begin(57600);    

    SPIFFS.begin();
    
    if (!readConfigurationAndInit()) return;
    
    prepareLedStrip();
    otaHelper.setup();

    sceneHelper.onChange([&](uint8_t r, uint8_t g, uint8_t b) {        
        Serial.printf(
            "Scene changed, changing color of strip to rgb(%d,%d,%d).\n",
            r, g, b
        );

        startColor = targetColor;
        targetColor = RgbColor(r, g, b);

        animations.StartAnimation(0, ANIMATION_TIME, blendingAnimation);
    });
}

void loop() {
    otaHelper.handle();
    sceneHelper.handle();
    
    if (animations.IsAnimating())
    {
        animations.UpdateAnimations();
        strip.show();
    }
}

void blendingAnimation(const AnimationParam& param)
{
    RgbColor color = RgbColor::LinearBlend(
        startColor,
        targetColor,
        param.progress
    );

    for (uint16_t i = 0; i < PIXEL_COUNT; i++) {
        strip.setPixelColor(i, color.G, color.R, color.B);
    }
}

bool readConfigurationAndInit()
{
    File configFile = SPIFFS.open("/config.json", "r");
    
    if (!configFile) {
        Serial.println("Failed to open config file");
        return false;
    }

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);

    DynamicJsonBuffer jsonBuffer;
    JsonObject& config = jsonBuffer.parseObject(buf.get());
    
    connectWifi(config);
    loadScenes(config);
    
    return true;
}

void connectWifi(JsonObject& config)
{
    const char* wifi_ssid = config["wifi"]["ssid"];
    const char* wifi_key = config["wifi"]["key"];

    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_key);
    
    Serial.print("Connecting to wifi..");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    
    Serial.println(".. ok.");
}

void prepareLedStrip()
{
    strip.begin();
    strip.clear();
    strip.show();
    
    Serial.println("Cleared led strip.");
}

void loadScenes(JsonObject& config)
{
    JsonArray& scenes = config["scenes"];
    
    for (auto& sceneInfo : scenes) {
        char* name = strdup(sceneInfo["name"]);
        JsonArray& color = sceneInfo["rgb"];
        
        uint8_t r = color[0];
        uint8_t g = color[1];
        uint8_t b = color[2];
        
        Scene scene;
        scene.name = name;
        scene.r = r;
        scene.g = g;
        scene.b = b;
        
        sceneHelper.add(scene);
    }
}
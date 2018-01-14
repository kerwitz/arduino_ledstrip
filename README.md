Arduino LED strip Controller with Alexa support
===================

This Arduino project allows you to control a RGB LED strip via Amazon Alexa voice commands. It uses fauxmoESP to simulate wifi switches for every scene you configure, which can be used with Echo routines or the like. Different scenes will be faded into each other to blend in with Philips Hue lamps. The project supports OTA updates so you can provide new scenes and update the existing ones fairly easy.

Dependencies
------------
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
* [NeoPixelBus](https://github.com/Makuna/NeoPixelBus)
* [fauxmoESP](https://bitbucket.org/xoseperez/fauxmoesp)
* [OTAHelper](https://github.com/kerwitz/OTAHelper)

Configuration
------------
The project can be configured via `data\config.json`. Simply copy `data\config.example.json`, update it with your own values and [upload it to the Arduino](http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html#uploading-files-to-file-system) (but use a more recent version of the plugin than stated in the docs).

```
{
    "device": {
        "name": "LED Strip",
        "pixels": 120
    },
    "animation": {
        "duration": 1000
    },
    "wifi": {
        "ssid": "my fancy wifi",
        "key": "*******"
    },
    "scenes": [
        {
            "name": "Sunlight",
            "rgb": [255, 30, 0]
        },
        {
            "name": "Aquarium",
            "rgb": [0, 0, 255]
        },
        {
            "name": "Off",
            "isOffSwitch": true
        }
    ]
}
```

Assuming your device name to be "LED Strip", theses scenes will show up in the Alexa app as "LED Strip: Sunlight" and "LED Strip: Aquarium". You may mark scenes as off switches by setting `isOffSwitch` to true. These scenes will never be activated but instead trigger the strip to return to rgb(0,0,0) and reset the active scene.

### Configuring pin and strip type
By default the project assumes your strip to be connected to the **D1 pin** and to support **GRB + 800khz** mode. If your setup differs from these assumptions you can change the constants `LED_STRIP_TYPE` and `LED_STRIP_PIN` in the `StripConfig.h` header file.

Setup
----------
* **[Download the latest release](https://github.com/kerwitz/arduino_ledstrip/releases)**
* **Install all dependencies**

  Either through the IDE or manually into the `libraries` folder.
* **Deploy the project to your Arduino device**

That's it. Once you've configured everything and deployed the code you can discover the emulated network switches from within the Alexa app. Once your Echo device has discovered the switches you may use them to your liking, for example from within routines.

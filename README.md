Arduino LED strip Controller with Alexa support
===================

This Arduino project allows you to control a RGB LED strip via Amazon Alexa voice commands. It uses fauxmoESP to simulate network switches for every scene you configure, which can be used with Echo routines or the like. Different scenes will be faded into each other to blend in with Philips Hue lamps. The project supports OTA updates so you can provide new scenes and update the existing ones fairly easy.

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
            "rgb": [0,0,0]
        }
    ]
}
```

Assuming "LED Strip" as `DEVICE_NAME` (see setup below), theses scenes will show up in the Alexa app as "LED Strip: Sunlight" and "LED Strip: Aquarium". Please note that the third scene is currently assumed to be an off trigger (see [#1](https://github.com/kerwitz/arduino_ledstrip/issues/1)).

Setup
----------
* **[Download the latest release](https://github.com/kerwitz/arduino_ledstrip/releases)**
* **Install all dependencies**

  Either through the IDE or manually into the `libraries` folder.
* **Change constants according to your setup if needed**

  The project assumes your strip to be **GRB + 800khz** supportive. Change `LED_STRIP_TYPE` in `ledstrip.ino` if you are running a different type of led strip. Currently the strip will be located at **PIN D1** (Wemos D1 Mini). Update `LED_STRIP_PIN` if you have connected your stip to another pin.
 
That's it. Once you've configured everything and deployed the code to your Arduino device (such as a Wemos D1 Mini) you can discover the emulated network switches from within the Alexa app. Once your Echo device has discovered the switches you may use them to your liking, for example from within routines.

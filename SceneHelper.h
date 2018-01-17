#pragma once

#include <fauxmoESP.h>

typedef struct {
    char * name;
    unsigned char id;
    uint8_t r, g, b;
    bool isOffSwitch = false;
} Scene;

typedef std::function<void(uint8_t, uint8_t, uint8_t)> onChangeHandler;

class SceneHelper {
  public:
    SceneHelper();

    void
        setDeviceName(const char * deviceName),
        add(Scene scene),
        handle(),
        onChange(onChangeHandler callback) { _onChangeHandler = callback; }

  private:
    const char * _deviceName;
    std::vector<Scene> _scenes;
    unsigned char _activeScene = -1;
    onChangeHandler _onChangeHandler = NULL;
    fauxmoESP fauxmo;

    void
        _handleSceneSwitch(unsigned char sceneId, bool state),
        _fireChangeHandler(uint8_t r, uint8_t g, uint8_t b);
};

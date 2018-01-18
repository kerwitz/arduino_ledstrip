#include "SceneHelper.h"

SceneHelper::SceneHelper()
{
    fauxmo.onSetState([&](unsigned char sceneId, const char * sceneName, bool state) {
        _handleSceneSwitch(sceneId, state);
    });

    fauxmo.onGetState([&](unsigned char sceneId, const char * sceneName) {
        return _activeScene == sceneId;
    });
}

void SceneHelper::setDeviceName(const char* deviceName)
{
    _deviceName = deviceName;

    Serial.printf("[SceneHelper] _deviceName = %s\n\n", _deviceName);
}

/**
 * Adds a new scene to our collection.
 *
 * @param scene
 */
void SceneHelper::add(Scene scene)
{
    char sceneName[100];
    sprintf_P(sceneName, "%s: %s", _deviceName, scene.name);

    scene.id = fauxmo.addDevice(sceneName);

    if (scene.isOffSwitch) {
        Serial.printf("[SceneHelper] Adding off switch scene with name \"%s\".\n", sceneName);
    } else {
        Serial.printf(
            "[SceneHelper] Adding scene with name \"%s\" and color rgb(%d,%d,%d).\n",
            sceneName, scene.r, scene.g, scene.b
        );
    }

    _scenes.push_back(scene);
}

void SceneHelper::handle()
{
    fauxmo.handle();
}

void SceneHelper::_handleSceneSwitch(unsigned char sceneId, bool state)
{
    for (unsigned int i = 0; i < _scenes.size(); i++) {
        if (_scenes[i].id != sceneId) continue;

        if (!state || _scenes[i].isOffSwitch) {
            _activeScene = -1;
            _fireChangeHandler(0, 0, 0);
            return;
        }

        _activeScene = sceneId;
        _fireChangeHandler(_scenes[i].r, _scenes[i].g, _scenes[i].b);
    }
}

void SceneHelper::_fireChangeHandler(uint8_t r, uint8_t g, uint8_t b)
{
    if (_onChangeHandler) {
        _onChangeHandler(r, g, b);
    }
}

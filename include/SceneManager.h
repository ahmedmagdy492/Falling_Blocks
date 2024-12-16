#pragma once

#include <string>
#include <unordered_map>

#include "Scene.h"

class SceneManager {
private:
	Scene* currentActiveScene = nullptr;
	std::unordered_map<std::string, Scene*> scenes;

public:
	void AddScene(std::string sceneName, Scene* scene);
	void RenderCurrentScene();
	void SwitchScene(const std::string& sceneName);

	~SceneManager();
};
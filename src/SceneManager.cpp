
#include "../include/SceneManager.h"

void SceneManager::AddScene(std::string sceneName, Scene* scene) {
	if (scenes.find(sceneName) == scenes.end()) {
		scenes.insert({ sceneName, scene });

		if (!currentActiveScene) {
			currentActiveScene = scene;
		}
	}
}

void SceneManager::RenderCurrentScene() {
	if (currentActiveScene) {
		currentActiveScene->Render();
	}
}

void SceneManager::SwitchScene(const std::string& sceneName) {
	auto scene = scenes.find(sceneName);
	if (scene != scenes.end()) {
		currentActiveScene = scenes[sceneName];
	}
}

SceneManager::~SceneManager() {

}
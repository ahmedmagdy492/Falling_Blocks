#pragma once
extern "C" {
#include <raylib.h>
}

class TextDisplayUI {
private:
	Vector2 position;
	Vector2 size;
	const char* text;
	Color textColor;
	Font font;
	int fontSize;
	bool hasBackgroundOverlay;
	Vector2 overlayStart = {};
	Vector2 overlayEnd = {};
	Color overlayColor;

public:
	TextDisplayUI(const char* text, Vector2 position, Color textColor, Font font, int fontSize);

	inline void EnableHasOverlayBackground(Vector2 overlayStart, Vector2 overlayEnd, Color color) {
		hasBackgroundOverlay = true;
		this->overlayStart = overlayStart;
		this->overlayEnd = overlayEnd;
		this->overlayColor = color;
	}

	inline void DisableOverlay() {
		hasBackgroundOverlay = false;
	}

	inline void SetText(const char* newText) {
		text = newText;
		size = MeasureTextEx(font, text, fontSize, 0.0f);
	}

	void Draw();

	~TextDisplayUI();
};
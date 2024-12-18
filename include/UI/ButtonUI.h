#pragma once

extern "C" {
#include <raylib.h>
}

class ButtonUI {
private:
	Rectangle padding;
	Vector2 position;
	Vector2 size;
	void (*onClick)();
	Font font;
	const char* text;
	int fontSize;

public:
	ButtonUI(const char* text, Font font, int fontSize, Vector2 position, void (*onClick)());

	void Draw();

	inline void SetPadding(Rectangle newPadding) {
		padding = newPadding;
	}

	void PlaceBeneath(const ButtonUI& otherButton, float margin);

	void CenterButton(int screenWidth, int screenHeight);

	~ButtonUI();
};
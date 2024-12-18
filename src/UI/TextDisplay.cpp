
#include "../../include/UI/TextDisplay.h"

extern "C" {
#include <raylib.h>
}

TextDisplayUI::TextDisplayUI(const char* text, Vector2 position, Color textColor, Font font, int fontSize) :
	text(text), position(position), textColor(textColor), font(font), hasBackgroundOverlay(false), overlayColor(BLACK), fontSize(fontSize) {
	size = MeasureTextEx(font, text, fontSize, 0.0f);
}

void TextDisplayUI::Draw() {
	if (hasBackgroundOverlay) {
		DrawRectangleV(overlayStart, overlayEnd, overlayColor);
	}

	DrawTextEx(font, text, position, fontSize, 0.0f, textColor);
}

TextDisplayUI::~TextDisplayUI() {

}
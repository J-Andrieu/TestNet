#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "../includes/Button.h"

Button::Button(int x, int y, int h, int w, SDL_Surface* surface, void(*func)(void), SDL_Renderer* renderer, std::string buttonLabel, TTF_Font* buttonFont) {
	colors = new Uint32[numStates];
	position.x = x;
	position.y = y;
	position.h = h;
	position.w = w;
	drawSurface = surface;
	callbackFunction = func;
	label = buttonLabel;
	font = buttonFont;
	SDL_Color color = { 0,0,0 };
	text_surface = TTF_RenderText_Solid(font, label.c_str(), color);
}

Button::Button(int x, int y, int h, int w, SDL_Surface* surface, void(*func)(void), SDL_Renderer* renderer, std::string buttonLabel, std::string buttonFontLocation) {
	colors = new Uint32[numStates];
	position.x = x;
	position.y = y;
	position.h = h;
	position.w = w;
	drawSurface = surface;
	callbackFunction = func;
	label = buttonLabel;
}

Button::Button(SDL_Rect rect, SDL_Surface* surface, void(*func)(void), SDL_Renderer* renderer, std::string buttonLable, TTF_Font* buttonFont) {
	colors = new Uint32[numStates];
	position.x = rect.x;
	position.y = rect.y;
	position.h = rect.h;
	position.w = rect.w;
	drawSurface = surface;
	callbackFunction = func;
	label = buttonLable;
	font = buttonFont;
	SDL_Color color = { 0,0,0 };
	text_surface = TTF_RenderText_Solid(font, label.c_str(), color);
}

Button::Button(SDL_Rect rect, SDL_Surface* surface, void(*func)(void), SDL_Renderer* renderer, std::string buttonLable, std::string buttonFontLocation) {
	colors = new Uint32[numStates];
	position.x = rect.x;
	position.y = rect.y;
	position.h = rect.h;
	position.w = rect.w;
	drawSurface = surface;
	callbackFunction = func;
	label = buttonLable;
	//font = buttonFont;
}

Button::~Button() {
	delete[] colors;
}

SDL_Rect Button::getPos() {
	return position;
}

std::string Button::getLabel() {
	return label;
}

void Button::setFunction(void(*func)(void)) {
	callbackFunction = func;
}

void Button::setLabel(std::string newLabel) {
	label = newLabel;
}

void Button::setFont(TTF_Font* newFont) {
	font = newFont;
}

void Button::setFont(std::string fontPath) {
	font = TTF_OpenFont(fontPath.c_str(), position.h * .75);
}

void Button::setRect(SDL_Rect rect) {
	position.h = rect.h;
	position.w = rect.w;
	position.x = rect.x;
	position.y = rect.y;
}

void Button::setRect(int x, int y, int h, int w) {
	position.x = x;
	position.y = y;
	position.w = w;
	position.h = h;
}

void Button::setPosition(int x, int y) {
	position.x = x;
	position.y = y;
}

void Button::setSize(int h, int w) {
	position.h = h;
	position.w = w;
}

bool Button::isPointContained(int x, int y) {
	if (x > position.x && x < (position.x + position.w) && y > position.y && y < (position.y + position.h)) {
		return true;
	}
	return false;
}

void Button::setColor(int r, int g, int b) {
	colors[neutral] = SDL_MapRGB(drawSurface->format, r, g, b);
}

void Button::setHighlightColor(int r, int g, int b) {
	colors[highlighted] = SDL_MapRGB(drawSurface->format, r, g, b);
}

void Button::setClickColor(int r, int g, int b) {
	colors[clicked] = SDL_MapRGB(drawSurface->format, r, g, b);
}

void Button::setLockColor(int r, int g, int b) {
	colors[locked] = SDL_MapRGB(drawSurface->format, r, g, b);
}

void Button::setNeutral() {
	currentState = neutral;
}

void Button::setClicked() {
	currentState = clicked;
}

void Button::setHighlighted() {
	currentState = highlighted;
}

void Button::draw() {
	if (currentState < deactivated) {
		SDL_FillRect(drawSurface, &position, colors[currentState]);
		SDL_BlitSurface(text_surface, NULL, drawSurface, &position);
	}
}

void setActive(Button &button) {
	button.currentState = Button::neutral;
}

void setInactive(Button &button) {
	button.currentState = Button::deactivated;
}

void setLocked(Button &button) {
	button.currentState = Button::locked;
}

void setState(Button &button, int state) {
	if (state >= 0 && state < Button::numStates) {
		button.currentState = state;
	}
}

void setState(Button &button, int x, int y, bool mouseClicked) {
	if (x > button.position.x && x < (button.position.x + button.position.w) && y > button.position.y && y < (button.position, y + button.position.h)) {
		if (mouseClicked) {
			button.currentState = Button::clicked;
			return;
		}
		button.currentState = Button::highlighted;
		return;
	}
	button.currentState = Button::neutral;
}

void setState(Button &button, SDL_MouseMotionEvent &e) {
	if (!(e.state == SDL_BUTTON_LEFT)) {
		if (e.x > button.position.x && e.x < (button.position.x + button.position.w) && e.y > button.position.y && e.y < (button.position.y + button.position.h)) {
			button.currentState = Button::highlighted;
		} else {
			button.currentState = Button::neutral;
		}
	}
}

void setState(Button &button, SDL_MouseButtonEvent &e) {
	if (e.state == SDL_PRESSED) {
		if (e.x > button.position.x && e.x < (button.position.x + button.position.w) && e.y > button.position.y && e.y < (button.position.y + button.position.h)) {
			button.currentState = Button::clicked;
		}
	} else if (e.state == SDL_RELEASED) {
		if (button.currentState == Button::clicked) {
			button.callbackFunction();
		}
		if (e.x > button.position.x && e.x < (button.position.x + button.position.w) && e.y > button.position.y && e.y < (button.position.y + button.position.h)) {
			button.currentState = Button::highlighted;
		} else {
			button.currentState = Button::neutral;
		}
	}
}


int getState(Button &button) {
	return button.currentState;
}

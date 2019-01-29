#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Button {
public:
	enum state {
		neutral,
		clicked,
		highlighted,
		locked,
		deactivated,
		numStates
	};

	Button(int x, int y, int h, int w, SDL_Surface* surface, void(*func)(void), SDL_Renderer* renderer, std::string buttonLabel = std::string(""), TTF_Font* buttonFont = NULL);
	Button(int x, int y, int h, int w, SDL_Surface* surface, void(*func)(void), SDL_Renderer* renderer, std::string buttonLabel = std::string(""), std::string buttonFontLocation = std::string(""));
	Button(SDL_Rect rect, SDL_Surface* surface, void(*func)(void), SDL_Renderer* renderer, std::string buttonLable = std::string(""), TTF_Font* buttonFont= NULL);
	Button(SDL_Rect rect, SDL_Surface* surface, void(*func)(void), SDL_Renderer* renderer, std::string buttonLable = std::string(""), std::string buttonFontLocation = std::string(""));

	~Button();

	SDL_Rect getPos();
	std::string getLabel();


	void setFunction(void(*func)(void));
	void setLabel(std::string newLabel);
	void setFont(TTF_Font* newFont);
	void setFont(std::string fontPath);
	void setRect(SDL_Rect);
	void setRect(int x, int y, int h, int w);
	void setPosition(int x, int y);
	void setSize(int h, int w);
	bool isPointContained(int x, int y);

	void setColor(int r, int g, int b);
	void setClickColor(int r, int g, int b);
	void setHighlightColor(int r, int g, int b);
	void setLockColor(int r, int g, int b);

	void setNeutral();
	void setClicked();
	void setHighlighted();

	void draw();

	friend void setActive(Button &button);
	friend void setInactive(Button &button);
	friend void setLocked(Button &button);
	friend void setState(Button &button, int state);
	friend void setState(Button &button, int x, int y, bool mouseClicked);
	friend void setState(Button &button, SDL_MouseMotionEvent &e);
	friend void setState(Button &button, SDL_MouseButtonEvent &e);
	friend int getState(Button &button);

private:
	void(*callbackFunction)(void);

	int currentState;

	std::string label;

	SDL_Rect position;

	Uint32 *colors;

	SDL_Surface* drawSurface;
	SDL_Surface* text_surface;

	TTF_Font* font;
};
#pragma once
#include "ECS/ecs.hpp"
#include "ECS/Components.hpp"
#include "AssetManager.hpp"
#include "Game.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class UILabel : public Component
{
private:
	SDL_Rect labelRect = {};
	std::string labelText;
	std::string labelFont;
	SDL_Color labelBackgroundColor;
	SDL_Color labelTextColor;
	SDL_Texture* texture = nullptr;

public:
	UILabel(std::string text,std::string font)
	{
		labelText = text;
		labelFont = font;
		labelBackgroundColor = { 255, 255, 255, 0 };
		labelTextColor = { 0, 0, 0, 255 };
	}

	~UILabel()
	{
	}

	void init() override
	{
		auto transform = &entity->GetComponent<TransformComponent>();
		labelRect.x = static_cast<int>(transform->position.x);
		labelRect.y = static_cast<int>(transform->position.y);
		setLabelText(labelText);
	}

	void setLabelText(std::string labelText)
	{
		SDL_Surface* surface = TTF_RenderText_Blended(Game::assets->GetFont(labelFont), labelText.c_str(), labelTextColor);
		texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
		SDL_FreeSurface(surface);
		SDL_QueryTexture(texture, NULL, NULL, &labelRect.w, &labelRect.h);
	}

	void draw() override
	{
		SDL_RenderCopy(Game::renderer, texture, NULL, &labelRect);
		SDL_DestroyTexture(texture);
	}
};
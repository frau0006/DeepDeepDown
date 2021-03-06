#pragma once
#include "SFML/Graphics.hpp"

namespace Entity {
	enum class EntityType {
		None = 0,
		Player
	};

	class Entity {
	public:
		Entity(sf::Vector2f aPosition, sf::Vector2f aSize, const std::string& aTextureFilePath, bool aIsVisible = true);
		virtual ~Entity();

		virtual void Update(double& deltaTime);
		virtual void Draw(sf::RenderWindow* window);

		virtual void Move(sf::Vector2f aDistance, double& deltaTime);

		sf::Sprite* sprite;
		sf::Texture* texture;

		sf::FloatRect hitBox;
		sf::Vector2f position;

		bool isVisible;
	};

	Entity* CreateNewEntity(sf::Vector2f aPosition, EntityType aType);
}
#pragma once
#include "SFML/Graphics.hpp"
#include "StaticTile.hpp"
#include "../Animator.hpp"

namespace Tile {
	class AnimatedTile : public StaticTile {
	public:
		AnimatedTile(sf::Vector2f aPosition, const std::string& aTextureFileName, float aFrameRate, TileType aType, bool aCollidesWithEntities = false);
		~AnimatedTile() override;

		void Update(double& deltaTime) override;

	private:
		Animator* animator;
	};
}
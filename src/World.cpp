#include "World.hpp"

World::World() {
	cursor = new Cursor();

	player = new Player();

	chunks = new std::vector<Chunk*>;
	chunks->push_back(new Chunk(sf::Vector2f(0, 0)));

	// Starting Tiles
	chunks->at(0)->SetTileType(135, Tile::TileType::Stone);
	chunks->at(0)->SetTileType(137, Tile::TileType::Stone);
	chunks->at(0)->SetTileType(152, Tile::TileType::Stone);
	chunks->at(0)->SetTileType(120, Tile::TileType::Stone);

	chunks->at(0)->SetTileType(136, Tile::TileType::Floor);
}

World::~World() {
	delete player;

	for (unsigned int i = 0; i < chunks->size(); i++) {
		delete chunks->at(i);
	}
	chunks->clear();
	delete chunks;

	delete cursor;
}

void World::Update(double& deltaTime, sf::RenderWindow* window) {
	player->Update(deltaTime);
	cursor->Update(deltaTime, window);
	
	for (unsigned int i = 0; i < chunks->size(); i++) {
		chunks->at(i)->Update(deltaTime, player);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2f mousePos = sf::Vector2f(cursor->GetPosition());
		Tile::Tile* clickedTile = GetTileAt(mousePos);

		if (clickedTile != nullptr) {
			// Process OnLeftClick() events for that tile
			clickedTile->OnLeftClick(cursor);

			// If Stone is broken and there are adjacent Empty tiles, the Empty tiles become Stone tiles
			TurnSurroundingEmptyTilesToStone(clickedTile->position);
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		sf::Vector2f mousePos = sf::Vector2f(cursor->GetPosition());
		Tile::Tile* clickedTile = GetTileAt(mousePos);

		if (clickedTile != nullptr) {
			// Process OnLeftClick() events for that tile
			clickedTile->OnRightClick(cursor);
		}
	}

	// Player Movement
	// North (-y)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		player->Move(sf::Vector2f(0, -320), deltaTime);
		if (CheckTileCollision(player)) {
			player->Move(sf::Vector2f(0, 320), deltaTime);
		}
	}

	// South (+y)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player->Move(sf::Vector2f(0, 320), deltaTime);
		if (CheckTileCollision(player)) {
			player->Move(sf::Vector2f(0, -320), deltaTime);
		}
	}

	// West (-x)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player->Move(sf::Vector2f(-320, 0), deltaTime);
		if (CheckTileCollision(player)) {
			player->Move(sf::Vector2f(320, 0), deltaTime);
		}
	}

	// East (+x)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player->Move(sf::Vector2f(320, 0), deltaTime);
		if (CheckTileCollision(player)) {
			player->Move(sf::Vector2f(-320, 0), deltaTime);
		}
	}
}

void World::Draw(sf::RenderWindow* window) {	
	for (unsigned int i = 0; i < chunks->size(); i++) {
		chunks->at(i)->Draw(window);
	}

	player->Draw(window);
	cursor->Draw(window);
}

Chunk* World::GetChunkAt(sf::Vector2f aPosition) {
	for (unsigned int i = 0; i < chunks->size(); i++) {
		if (chunks->at(i)->chunkBorder.contains(aPosition)) {
			return chunks->at(i);
		}
	}

	return nullptr;
}

Tile::Tile* World::GetTileAt(sf::Vector2f aPosition) {
	Chunk* clickedChunk = GetChunkAt(aPosition);

	// If aPosition doesn't land in a chunk, skip this section
	if (clickedChunk != nullptr) {
		int clickedTileIndex = clickedChunk->GetTileIndexAt(aPosition);

		// If for some reason landed in a chunk, but there's no tile there, skip this section too
		if (clickedTileIndex != -1) {
			return clickedChunk->tiles->at(clickedTileIndex);
		}
	}

	return nullptr;
}

int World::GetTileIndexAt(sf::Vector2f aPosition) {
	for (unsigned int i = 0; i < chunks->size(); i++) {
		if (chunks->at(i)->GetTileAt(aPosition) != nullptr) {
			return chunks->at(i)->GetTileIndexAt(aPosition);
		}
	}

	return -1;
}

bool World::CheckTileCollision(Entity* entity) {
	sf::FloatRect entityHitBox = entity->hitBox;

	for (unsigned int i = 0; i < chunks->size(); i++) {
		if (chunks->at(i)->chunkBorder.intersects(entityHitBox)) {
			Chunk* chunkIntersectingEntity = chunks->at(i);

			for (unsigned int j = 0; j < chunkIntersectingEntity->tiles->size(); j++) {
				if (chunkIntersectingEntity->tiles->at(j)->hitBox.intersects(entityHitBox)) {
					if (chunkIntersectingEntity->tiles->at(j)->collidesWithEntities) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

void World::TurnSurroundingEmptyTilesToStone(sf::Vector2f aPosition) {
	Tile::Tile* tileToCheck = nullptr;

	// Check North (-y)
	tileToCheck = GetTileAt(aPosition + sf::Vector2f(0, -32));
	if (tileToCheck != nullptr && tileToCheck->type == Tile::TileType::Empty) {
		tileToCheck->OnWorldEvent();
	}

	// Check South (+y)
	tileToCheck = GetTileAt(aPosition + sf::Vector2f(0, +32));
	if (tileToCheck != nullptr && tileToCheck->type == Tile::TileType::Empty) {
		tileToCheck->OnWorldEvent();
	}

	// Check West (-x)
	tileToCheck = GetTileAt(aPosition + sf::Vector2f(-32, 0));
	if (tileToCheck != nullptr && tileToCheck->type == Tile::TileType::Empty) {
		tileToCheck->OnWorldEvent();
	}

	// Check East (+x)
	tileToCheck = GetTileAt(aPosition + sf::Vector2f(32, 0));
	if (tileToCheck != nullptr && tileToCheck->type == Tile::TileType::Empty) {
		tileToCheck->OnWorldEvent();
	}
}
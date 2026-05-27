#pragma once
#define ASSETS "../Assets/"

#include <SFML/Graphics/Texture.hpp>

class Assets {
public:
  inline static sf::Texture GrassTile;
  inline static sf::Texture DirtTile;
  inline static sf::Texture cropTile;
  inline static sf::Texture ToolTile;
  inline static sf::Texture BoxTile;
  inline static sf::Texture FurnitureTile;
  inline static sf::Texture UiTILe;
  inline static sf::Texture GrassBiomTile;

  Assets() {
    GrassTile.loadFromFile(ASSETS"Tilesets/Grass.png");
    DirtTile.loadFromFile(ASSETS"Tilesets/Tilled_Dirt_v2.png");
    cropTile.loadFromFile(ASSETS"Objects/Basic Plants.png");
    ToolTile.loadFromFile(ASSETS"Characters/Tools.png");
    BoxTile.loadFromFile(ASSETS"Objects/Chest.png");
    FurnitureTile.loadFromFile(ASSETS"Objects/Basic_Furniture.png");
    UiTILe.loadFromFile(ASSETS"UI/UI.png");
    GrassBiomTile.loadFromFile(ASSETS"/Objects/Basic Grass Biom things 1.png");
  }
};
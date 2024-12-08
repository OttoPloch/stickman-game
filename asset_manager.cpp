#include "asset_manager.h"

void AssetManager::load(std::string path) {
	if (!texture.loadFromFile(path)) { std::cout << "ERROR loading " << path << '\n'; }
}
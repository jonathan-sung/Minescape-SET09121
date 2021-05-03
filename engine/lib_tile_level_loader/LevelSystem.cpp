#include "LevelSystem.h"
#include <fstream>

using namespace std;
using namespace sf;

sf::Texture LevelSystem::spritesheet;
//remove
std::map<LevelSystem::Tile, sf::Color> LevelSystem::_colours{
	{WALL, Color::White}, {END, Color::Blue} };

sf::Color LevelSystem::getColor(LevelSystem::Tile t) {
	auto it = _colours.find(t);
	if (it == _colours.end()) {
		_colours[t] = Color::Transparent;
	}
	return _colours[t];
}

void LevelSystem::setColor(LevelSystem::Tile t, sf::Color c) {
	_colours[t] = c;
}

std::unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;

float LevelSystem::_tileSize(0.f);
//Vector2f LevelSystem::_offset(0.0f, 30.0f);
Vector2f LevelSystem::_offset(0, 0);
vector<std::unique_ptr<sf::Sprite>> LevelSystem::_sprites;

void LevelSystem::loadLevelFile(const std::string& path, float tileSize) {
	_tileSize = tileSize;
	size_t w = 0, h = 0;
	string buffer;

	if (!spritesheet.loadFromFile("res/spritesheet.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	else {
		std::cout << "Spritesheet load successful!" << std::endl;
	}
	//Jonathan();

	// Load in file to buffer
	ifstream f(path);
	if (f.good()) {
		f.seekg(0, std::ios::end);
		buffer.resize(f.tellg());
		f.seekg(0);
		f.read(&buffer[0], buffer.size());
		f.close();
	}
	else {
		throw string("Couldn't open level file: ") + path;
	}

	std::vector<Tile> temp_tiles;
	int widthCheck = 0;
	for (int i = 0; i < buffer.size(); ++i) {
		const char c = buffer[i];
		if (c == '\0')
			break;
		if (c == '\n') { // newline
			if (w == 0) {  // if we haven't written width yet
				w = i;       // set width
			}
			else if (w != (widthCheck - 1)) {
				throw string("non uniform width:" + to_string(h) + " ") + path;
			}
			widthCheck = 0;
			h++; // increment height
		}
		else {
			temp_tiles.push_back((Tile)c);
		}
		++widthCheck;
	}

	if (temp_tiles.size() != (w * h)) {
		throw string("Can't parse level file") + path;
	}
	_tiles = std::make_unique<Tile[]>(w * h);
	_width = w; // set static class vars
	_height = h;
	std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
	cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
	buildSprites();
}

void LevelSystem::buildSprites(bool optimise) {
	_sprites.clear();

	struct tp {
		sf::Vector2f p;
		sf::Vector2f s;
		sf::Color c;
		sf::Vector2i sprite_index;
	};
	vector<tp> tps;
	const auto tls = Vector2f(_tileSize, _tileSize);
	for (size_t y = 0; y < _height; ++y) {
		for (size_t x = 0; x < _width; ++x) {
			Tile t = EMPTY, up_t = EMPTY, down_t = EMPTY, left_t = EMPTY, right_t = EMPTY;
			t = getTile({ x, y });
			if (y + 1 < _height) up_t = getTile({ x, y + 1 });
			if (y > 0) {
				down_t = getTile({ x, y - 1 });
			}
			if (x > 0) left_t = getTile({ x - 1, y });
			if (x + 1 < _width) right_t = getTile({ x + 1, y });
			//if (t == EMPTY) continue;
			tps.push_back({ getTilePosition({x, y}), tls, getColor(t), getSpriteIndexFromTileWalls(up_t, down_t, left_t, right_t, Vector2i(1, 1)) });
		}
	}

	const auto nonempty = tps.size();

	for (auto& t : tps) {
		//std::cout << t.p << std::endl;
		//if (t.c != sf::Color::Transparent) {
		auto sp = make_unique<sf::Sprite>();
		sp->setPosition(t.p);
		sp->setTexture(spritesheet);
		if (t.c == Color::White) {
			sp->setTextureRect(sf::IntRect(t.sprite_index.x * _tileSize, t.sprite_index.y * _tileSize, _tileSize, _tileSize));
		}
		else if (t.c == Color::Blue) {
			sp->setTextureRect(sf::IntRect(0, 3 * _tileSize, _tileSize, _tileSize));
		}
		else {
			sp->setTextureRect(sf::IntRect(1 * _tileSize, 3 * _tileSize, _tileSize, _tileSize));
		}
		_sprites.push_back(move(sp));

		//}
	}

	cout << "Level with " << (_width * _height) << " Tiles, With " << nonempty
		<< " Not Empty, using: " << _sprites.size() << " Sprites\n";
}
/*
sf::IntRect getTileSprite(LevelSystem::tp tile) {

}
*/
sf::Vector2i LevelSystem::getSpriteIndexFromTileWalls(Tile up, Tile down, Tile left, Tile right, sf::Vector2i offset) {
	int x = offset.x, y = offset.y;
	y -= up == WALL;
	y += down == WALL;
	x += left == WALL;
	x -= right == WALL;
	return sf::Vector2i(x, y);
}

void LevelSystem::render(RenderWindow& window) {
	for (auto& t : _sprites) {
		window.draw(*t);
	}
}

LevelSystem::Tile LevelSystem::getTile(sf::Vector2ul p) {
	if (p.x > _width || p.y > _height) {
		throw string("Tile out of range: ") + to_string(p.x) + "," +
			to_string(p.y) + ")";
	}
	return _tiles[(p.y * _width) + p.x];
}

size_t LevelSystem::getWidth() { return _width; }

size_t LevelSystem::getHeight() { return _height; }

sf::Vector2f LevelSystem::getTilePosition(sf::Vector2ul p) {
	return (Vector2f(p.x, p.y) * _tileSize) + _offset;
}

std::vector<sf::Vector2ul> LevelSystem::findTiles(LevelSystem::Tile type) {
	auto v = vector<sf::Vector2ul>();
	for (size_t i = 0; i < _width * _height; ++i) {
		if (_tiles[i] == type) {
			v.push_back({ i % _width, i / _width });
		}
	}

	return v;
}

LevelSystem::Tile LevelSystem::getTileAt(Vector2f v) {
	auto a = v - _offset;
	if (a.x < 0) v.x = 0;
	if (a.y < 0) v.y = 0;
	return getTile(Vector2ul((v - _offset) / (_tileSize)));
}

bool LevelSystem::isOnGrid(sf::Vector2f v) {
	auto a = v - _offset;
	if (a.x < 0 || a.y < 0) {
		return false;
	}
	auto p = Vector2ul((v - _offset) / (_tileSize));
	if (p.x > _width || p.y > _height) {
		return false;
	}
	return true;
}

void LevelSystem::setOffset(const Vector2f& _offset) {
	LevelSystem::_offset = _offset;
	buildSprites();
}

void LevelSystem::unload() {
	cout << "LevelSystem unloading\n";
	_sprites.clear();
	_tiles.reset();
	_width = 0;
	_height = 0;
	_offset = { 0, 0 };
}

const Vector2f& LevelSystem::getOffset() { return _offset; }

float LevelSystem::getTileSize() { return _tileSize; }

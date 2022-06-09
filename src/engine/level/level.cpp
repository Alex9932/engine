/*
 * level.cpp
 *
 *  Created on: Feb 17, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT
#include <engine/level/level.h>

namespace Engine {

	static Level* level = NULL;

	Level::Level() {

	}

	Level::~Level() {

	}

	void Level::SpawnEntity(Entity* entity) {
		this->objects.push_back(entity);
	}

	Render::Model* Level::GetGeometry() {
		return this->geom;
	}

	void Level::SetGeometry(Render::Model* geom) {
		this->geom = geom;
	}

	Object* Level::GetObject(Uint32 id) {
		return this->objects[id];
	}

	Uint32 Level::GetObjectCount() {
		return this->objects.size();
	}


	void NewLevel() {
		if(level) {
			delete level;
		}

		level = new Level();
	}

	Level* GetLevel() {
		return level;
	}

}

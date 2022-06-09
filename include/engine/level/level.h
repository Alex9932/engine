/*
 * level.h
 *
 *  Created on: Feb 17, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_LEVEL_LEVEL_H_
#define ENGINE_LEVEL_LEVEL_H_

#include <engine/level/object.h>
#include <engine/level/entity.h>
#include <engine/render/model.h>
#include <vector>

namespace Engine {

	class RG_DECLSPEC Level {
		private:
			std::vector<Object*> objects;
			Render::Model* geom = NULL;

		public:
			Level();
			virtual ~Level();

			void SpawnEntity(Entity* entity);
			Render::Model* GetGeometry();
			void SetGeometry(Render::Model* geom);

			Object* GetObject(Uint32 id);
			Uint32 GetObjectCount();
	};


	RG_DECLSPEC void NewLevel();
	RG_DECLSPEC Level* GetLevel();
}


#endif /* ENGINE_LEVEL_LEVEL_H_ */

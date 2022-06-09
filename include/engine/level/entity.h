/*
 * entity.h
 *
 *  Created on: Feb 17, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_LEVEL_ENTITY_H_
#define ENGINE_LEVEL_ENTITY_H_

#include <engine/level/object.h>

namespace Engine {

	class RG_DECLSPEC Entity: public Object {
		protected:
			Math::vec3 velocity;

		public:
			Entity(const Math::vec3& pos);
			virtual ~Entity();
			virtual void Update();
			Math::vec3& GetVelocity();
			void SetVelocity(const Math::vec3& pos);
	};

}

#endif /* ENGINE_LEVEL_ENTITY_H_ */

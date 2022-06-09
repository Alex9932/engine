/*
 * object.h
 *
 *  Created on: Feb 17, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_OBJECT_H_
#define ENGINE_CORE_OBJECT_H_

#include <engine/engine.h>
#include <engine/core/math/math.h>

namespace Engine {

	class RG_DECLSPEC Object {
		protected:
			Math::vec3 position;
			Math::vec3 rotation;
			Math::mat4 transform;
			Uint32 model_id;
			Math::vec3 scale;

		public:
			Object(const Math::vec3& pos);
			virtual ~Object();
			virtual void Update();
			Math::vec3& GetPosition();
			Math::vec3& GetRotation();
			Math::mat4& GetTransform();
			void SetPosition(const Math::vec3& pos);
			void SetRotation(const Math::vec3& rot);
			Math::vec3& GetScale();
			void SetScale(float scale);
			void SetScale(const Math::vec3& scale);

			Uint32 GetModelID();
			void SetModelID(Uint32 id);
	};

}


#endif /* ENGINE_CORE_OBJECT_H_ */

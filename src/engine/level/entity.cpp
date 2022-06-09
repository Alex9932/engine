/*
 * entity.cpp
 *
 *  Created on: Feb 17, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/engine.h>
#include <engine/level/entity.h>
#include <engine/core/math/matrix.h>

namespace Engine {

	Entity::Entity(const Math::vec3& pos): Object(pos) {
		velocity.x = 0;
		velocity.y = 0;
		velocity.z = 0;
		Math::mat4_identity(&transform);
		position = pos;
	}

	Entity::~Entity() {}

	void Entity::Update() {

		position.x += velocity.x * GetDeltaTime();
		position.y += velocity.y * GetDeltaTime();
		position.z += velocity.z * GetDeltaTime();

		Object::Update(); // Update matrix
	}

	Math::vec3& Entity::GetVelocity() {
		return this->velocity;
	}

	void Entity::SetVelocity(const Math::vec3& pos) {
		this->velocity.x = pos.x;
		this->velocity.y = pos.y;
		this->velocity.z = pos.z;
	}

} /* namespace Engine */

/*
 * object.cpp
 *
 *  Created on: Feb 17, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/level/object.h>
#include <engine/core/math/matrix.h>

namespace Engine {

	Object::Object(const Math::vec3& pos) {
		this->position.x = pos.x;
		this->position.y = pos.y;
		this->position.z = pos.z;
		this->rotation.x = 0;
		this->rotation.y = 0;
		this->rotation.z = 0;
		this->model_id = 0;
		this->scale.x = 1;
		this->scale.y = 1;
		this->scale.z = 1;
	}

	Object::~Object() {}

	void Object::Update() {
		Math::mat4_model(&this->transform, this->position, this->rotation, this->scale);
	}

	Math::vec3& Object::GetPosition() {
		return this->position;
	}

	Math::vec3& Object::GetRotation() {
		return this->rotation;
	}

	Math::mat4& Object::GetTransform() {
		return this->transform;
	}

	void Object::SetPosition(const Math::vec3& pos) {
//		this->position = pos;
		this->position.x = pos.x;
		this->position.y = pos.y;
		this->position.z = pos.z;
	}

	void Object::SetRotation(const Math::vec3& rot) {
//		this->rotation = rot;
		this->rotation.x = rot.x;
		this->rotation.y = rot.y;
		this->rotation.z = rot.z;
	}

	Math::vec3& Object::GetScale() {
		return scale;
	}

	void Object::SetScale(float scale) {
		this->scale.x = scale;
		this->scale.y = scale;
		this->scale.z = scale;
	}

	void Object::SetScale(const Math::vec3& scale) {
		this->scale = scale;
	}

	Uint32 Object::GetModelID() {
		return this->model_id;
	}

	void Object::SetModelID(Uint32 id) {
		this->model_id = id;
	}

}

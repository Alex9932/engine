/*
 * camera.cpp
 *
 *  Created on: Mar 14, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/input.h>
#include <engine/core/math/matrix.h>
#include <engine/core/math/vector.h>
#include <engine/engine.h>
#include <engine/level/camera.h>
#include <SDL2/SDL_scancode.h>

namespace Engine {

	static void math_getForward(Engine::Math::vec3* vec, const Engine::Math::vec2& angles) {
		vec->x = (float) SDL_sin(angles.x);
		vec->y = (float)-SDL_tan(angles.y);
		vec->z = (float)-SDL_cos(angles.x);
		Engine::Math::vec3_normalize(vec, *vec);
	}

	Camera::Camera(const Math::vec3& pos, float fov, float near, float far, float aspect): Entity(pos) {
		this->fov = fov;
		this->near = near;
		this->far = far;
		this->aspect = aspect;
		this->UpdateProjection();
	}

	Camera::~Camera() {

	}

	void Camera::UpdateProjection() {
		Math::mat4_frustum(&projection, aspect, fov, near, far);
	}


	void Camera::Update() {
//		if(Input::IsButtonDown(1)) {
//			rotation.x += Input::GetMouseDY() * 0.01;
//			rotation.y += Input::GetMouseDX() * 0.01;
//		}

		double delta = GetDeltaTime();

		Math::vec3 fwd;
		Math::vec3 vec;
		Math::vec3 up = {0, 1, 0};
		math_getForward(&fwd, {rotation.y, rotation.x});
		Math::vec3_cross(&vec, fwd, up);
		Math::vec3_normalize(&vec, vec);

		float speed = speed_default;

		if(Input::IsKeyDown(SDL_SCANCODE_LCTRL)) {
			speed = speed_fast;
		}
		if(Input::IsKeyDown(SDL_SCANCODE_LSHIFT)) {
			speed = speed_slow;
		}

		if(Input::IsKeyDown(SDL_SCANCODE_W)) {
			position.x += fwd.x * speed * delta;
			position.y += fwd.y * speed * delta;
			position.z += fwd.z * speed * delta;
		}
		if(Input::IsKeyDown(SDL_SCANCODE_S)) {
			position.x -= fwd.x * speed * delta;
			position.y -= fwd.y * speed * delta;
			position.z -= fwd.z * speed * delta;
		}
		if(Input::IsKeyDown(SDL_SCANCODE_A)) {
			position.x -= vec.x * speed * delta;
			position.y -= vec.y * speed * delta;
			position.z -= vec.z * speed * delta;
		}
		if(Input::IsKeyDown(SDL_SCANCODE_D)) {
			position.x += vec.x * speed * delta;
			position.y += vec.y * speed * delta;
			position.z += vec.z * speed * delta;
		}

		Math::mat4_view(&transform, position, rotation);
	}

	Math::mat4& Camera::GetProjection() { return projection; }
	void Camera::SetFov(float fov) { this->fov = fov; }
	void Camera::SetNear(float near) { this->near = near; }
	void Camera::SetFar(float far) { this->far = far; }
	void Camera::SetAspect(float aspect) { this->aspect = aspect; }
	float Camera::GetFov() { return this->fov; }
	float Camera::GetNear() { return this->near; }
	float Camera::GetFar() { return this->far; }
	float Camera::GetAspect() { return this->aspect; }

	void Camera::SetSpeed(const Math::vec3& speed) {
		speed_default = speed.x;
		speed_slow = speed.y;
		speed_fast = speed.z;
	}

	LookATCamera::LookATCamera(const Math::vec3& point, float dist, float fov, float near, float far, float aspect)
	: Camera({0, 0, 0}, fov, near, far, aspect) {
		collision = true;
		SetPoint(point);
		SetDistance(dist);
		calculateTransformation();
	}

	LookATCamera::~LookATCamera() {}

	void LookATCamera::Update() {
		calculateTransformation();
	}

	Engine::Math::vec3 LookATCamera::calculatePos(float dist) {
		float h = dist * SDL_cos(rotation.x);
		float v = dist * SDL_sin(rotation.x);
		float off_x = h * SDL_sin(rotation.y);
		float off_z = h * SDL_cos(rotation.y);
		Engine::Math::vec3 p;
		Engine::Math::vec3_add(&p, point, {off_x, v, off_z});
//		Engine::Math::mat4_view(&transform, p, {rotation.x, -rotation.y});
		return p;
	}

	float LookATCamera::findDistance() {
		float dist = this->dist;
		Engine::Math::vec3 p = calculatePos(dist);
		if(!collision) { return dist; }

		for (Uint32 i = 0; p.y <= 0.2; ++i) {
			dist -= 0.01;
			p = calculatePos(dist);
			if(i > 10000) { break; }
		}
		return dist;
	}

	void LookATCamera::calculateTransformation() {
		Engine::Math::vec3 p = calculatePos(findDistance());
		position = p;
		Engine::Math::mat4_view(&transform, p, {rotation.x, -rotation.y, rotation.z});
	}

	void LookATCamera::SetPoint(const Math::vec3& point) { this->point = point; }
	void LookATCamera::SetDistance(float dist) { this->dist = dist; }
	Math::vec3& LookATCamera::GetPoint() { return point; }
	float LookATCamera::GetDistance() { return dist; }
	void LookATCamera::DisableCollision() { collision = false; }
	void LookATCamera::EnableCollision() { collision = false; }
}

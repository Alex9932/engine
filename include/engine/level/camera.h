/*
 * camera.h
 *
 *  Created on: Mar 14, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_LEVEL_CAMERA_H_
#define ENGINE_LEVEL_CAMERA_H_

#include <engine/level/entity.h>

#undef near
#undef far

namespace Engine {

	class RG_DECLSPEC Camera: public Entity {
		private:
			float fov;
			float near;
			float far;
			float aspect;
			Math::mat4 projection;

		protected:
			float speed_default = 2;
			float speed_slow = 1;
			float speed_fast = 10;


		public:
			Camera(const Math::vec3& pos, float fov, float near, float far, float aspect);
			virtual ~Camera();
			virtual void Update();
			void UpdateProjection();
			Math::mat4& GetProjection();

			void SetFov(float fov);
			void SetNear(float near);
			void SetFar(float far);
			void SetAspect(float aspect);
			float GetFov();
			float GetNear();
			float GetFar();
			float GetAspect();

			void SetSpeed(const Math::vec3& speed);
	};


	class RG_DECLSPEC LookATCamera: public Camera {
		private:
			Math::vec3 point;
			float dist = 0;
			bool collision;

			Engine::Math::vec3 calculatePos(float dist);
			float findDistance();
			void calculateTransformation();

		public:
			LookATCamera(const Math::vec3& point, float dist, float fov, float near, float far, float aspect);
			virtual ~LookATCamera();
			virtual void Update();

			void SetPoint(const Math::vec3& point);
			void SetDistance(float dist);
			Math::vec3& GetPoint();
			float GetDistance();
			void DisableCollision();
			void EnableCollision();
	};

}

#endif /* ENGINE_LEVEL_CAMERA_H_ */

/*
 * matrix.h
 *
 *  Created on: Feb 10, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_MATH_MATRIX_H_
#define ENGINE_CORE_MATH_MATRIX_H_

#include <engine/core/math/math.h>
#include <engine/engine.h>

namespace Engine {
	namespace Math {

		RG_DECLSPEC void mat3_identity(mat3* mat, float scale);
		RG_FORCE_INLINE void mat3_identity(mat3* mat) { mat3_identity(mat, 1); }
		RG_DECLSPEC void mat3_mul(vec3* dest, vec3* left, mat3* right);
		RG_DECLSPEC void mat3_mul(mat3* dest, mat3* left, mat3* right);
		RG_DECLSPEC void mat3_rotx(mat3* mat, float angle);
		RG_DECLSPEC void mat3_roty(mat3* mat, float angle);
		RG_DECLSPEC void mat3_rotz(mat3* mat, float angle);
		RG_DECLSPEC void mat3_rotate(mat3* mat, const vec3& angles);

		RG_DECLSPEC void mat4_identity(mat4* mat, float scale);
		RG_FORCE_INLINE void mat4_identity(mat4* mat) { mat4_identity(mat, 1); }
		RG_DECLSPEC void mat4_identity(mat4* mat, const vec3& scale);
		RG_DECLSPEC void mat4_mul(vec4* dest, vec4* left, mat4* right);
		RG_DECLSPEC void mat4_mul(mat4* dest, mat4* left, mat4* right);
		RG_FORCE_INLINE void mat4_mul(vec4* dest, const vec4& left, const mat4& right) { mat4_mul(dest, (vec4*)&left.x, (mat4*)&right.m00); }
		RG_FORCE_INLINE void mat4_mul(mat4* dest, const mat4& left, const mat4& right) { mat4_mul(dest, (mat4*)&left.m00, (mat4*)&right.m00); }

		RG_DECLSPEC void mat4_rotx(mat4* mat, float angle);
		RG_DECLSPEC void mat4_roty(mat4* mat, float angle);
		RG_DECLSPEC void mat4_rotz(mat4* mat, float angle);
		RG_DECLSPEC void mat4_rotate(mat4* mat, const vec3& angles);
		RG_DECLSPEC void mat4_quat(mat4* mat, const quat& quat);
		RG_FORCE_INLINE void mat4_translate(mat4* mat, const vec3& pos) { mat->m03 = pos.x; mat->m13 = pos.y; mat->m23 = pos.z; }

		RG_DECLSPEC void mat4_ortho(mat4* mat, float r, float l, float t, float b, float f, float n );
		RG_DECLSPEC void mat4_frustum(mat4* mat, float aspect, float fov, float near_plane, float far_plane);
		RG_DECLSPEC void mat4_lookAt(mat4* mat, const vec3& pos, const vec3& center, const vec3& up);
		RG_DECLSPEC void mat4_view(mat4* mat, const vec3& pos, const vec3& angles);
		RG_DECLSPEC void mat4_model(mat4* mat, const vec3& pos, const vec3& angles, float scale);
		RG_DECLSPEC void mat4_model(mat4* mat, const vec3& pos, const vec3& angles, const vec3& scale);
		RG_FORCE_INLINE void mat4_model(mat4* mat, const vec3& pos, const vec3& angles) { mat4_model(mat, pos, angles, 1); }

		RG_DECLSPEC void mat4_inverse(mat4* dest, mat4* mat);
		RG_DECLSPEC float mat4_determinant(const Engine::Math::mat4& m);

		RG_DECLSPEC void mat4_decompose(vec3* position, quat* quaternion, vec3* scale, const mat4& matrix);
	}
}


#endif /* ENGINE_CORE_MATH_MATRIX_H_ */

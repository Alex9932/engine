/*
 * vector.h
 *
 *  Created on: Feb 10, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_MATH_VECTOR_H_
#define ENGINE_CORE_MATH_VECTOR_H_

#include <engine/core/math/math.h>
#include <engine/engine.h>

namespace Engine {
	namespace Math {

		RG_FORCE_INLINE void vec2_clone(vec2* dest, const vec2& src) { dest->x = src.x; dest->y = src.y; }
		RG_FORCE_INLINE void vec2_add(vec2* dest, const vec2& a, const vec2& b) { dest->x = a.x + b.x; dest->y = a.y + b.y; }
		RG_FORCE_INLINE void vec2_add(vec2* dest, const vec2& a, float b) { dest->x = a.x + b; dest->y = a.y + b; }
		RG_FORCE_INLINE void vec2_mul(vec2* dest, const vec2& a, const vec2& b) { dest->x = a.x * b.x; dest->y = a.y * b.y; }
		RG_FORCE_INLINE void vec2_mul(vec2* dest, const vec2& a, float b) { dest->x = a.x * b; dest->y = a.y * b; }
		RG_FORCE_INLINE void vec2_normalize(vec2* dest, const vec2& a) { float inv_length = invsqrt(a.x * a.x + a.y * a.y); dest->x = a.x * inv_length; dest->y = a.y * inv_length; }
		RG_FORCE_INLINE float vec2_length(vec2* a) { return SDL_sqrt(a->x * a->x + a->y * a->y); }
		RG_FORCE_INLINE float vec2_dot(vec2* a, const vec2& b) { return a->x*b.x + a->y*b.y; }
		RG_FORCE_INLINE void vec2_negate(vec2* dest, const vec2& a) { dest->x = -a.x; dest->y = -a.y; }
		RG_FORCE_INLINE void vec2_lerp(vec2* dest, const vec2& a, const vec2& b, double delta) { dest->x = a.x * (1.0 - delta) + b.x * delta; dest->y = a.y * (1.0 - delta) + b.y * delta; }

//		void vec2_clone(vec2* dest, const vec2& src);
//		void vec2_add(vec2* dest, const vec2& a, const vec2& b);
//		void vec2_add(vec2* dest, const vec2& a, float b);
//		void vec2_mul(vec2* dest, const vec2& a, const vec2& b);
//		void vec2_mul(vec2* dest, const vec2& a, float b);
//		void vec2_normalize(vec2* dest, const vec2& a);
//		float vec2_length(vec2* a);
//		float vec2_dot(vec2* a, const vec2& b);
//		void vec2_negate(vec2* dest, const vec2& a);
//		void vec2_lerp(vec2* dest, const vec2& a, const vec2& b, double delta);

		// vec3
//		void vec3_clone(vec3* dest, const vec3& src);
//		void vec3_add(vec3* dest, const vec3& a, const vec3& b);
//		void vec3_add(vec3* dest, const vec3& a, float b);
//		void vec3_mul(vec3* dest, const vec3& a, const vec3& b);
//		void vec3_mul(vec3* dest, const vec3& a, float b);
		RG_DECLSPEC void vec3_normalize(vec3* dest, const vec3& a);
		RG_DECLSPEC void vec3_cross(vec3* dest, const vec3& a, const vec3& b);
//		float vec3_length(vec3* a);
//		float vec3_dot(const vec3& a, const vec3& b);
//		void vec3_negate(vec3* dest, const vec3& a);
		RG_DECLSPEC void vec3_lerp(vec3* dest, const vec3& a, const vec3& b, double delta);

		RG_FORCE_INLINE void vec3_clone(vec3* dest, const vec3& src) { dest->x = src.x; dest->y = src.y; dest->z = src.z; }
		RG_FORCE_INLINE void vec3_add(vec3* dest, const vec3& a, const vec3& b) { dest->x = a.x + b.x; dest->y = a.y + b.y; dest->z = a.z + b.z; }
		RG_FORCE_INLINE void vec3_add(vec3* dest, const vec3& a, float b) { dest->x = a.x + b; dest->y = a.y + b; dest->z = a.z + b; }
		RG_FORCE_INLINE void vec3_mul(vec3* dest, const vec3& a, const vec3& b) { dest->x = a.x * b.x; dest->y = a.y * b.y; dest->z = a.z * b.z; }
		RG_FORCE_INLINE void vec3_mul(vec3* dest, const vec3& a, float b) { dest->x = a.x * b; dest->y = a.y * b; dest->z = a.z * b; }
		RG_FORCE_INLINE float vec3_length(vec3* a) { return SDL_sqrt(a->x * a->x + a->y * a->y + a->z * a->z); }
		RG_FORCE_INLINE float vec3_dot(const vec3& a, const vec3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
		RG_FORCE_INLINE void vec3_negate(vec3* dest, const vec3& a) { dest->x = -a.x; dest->y = -a.y; dest->z = -a.z; }

		RG_DECLSPEC void vec3_mul(vec3* dest, const vec3& v, const quat& q);

		// vec4
		RG_DECLSPEC void vec4_axisAngle(vec4* dest, const vec3& a);
//		void vec4_clone(vec4* dest, const vec4& src);
//		void vec4_add(vec4* dest, const vec4& a, const vec4& b);
//		void vec4_add(vec4* dest, const vec4& a, float b);
//		void vec4_mul(vec4* dest, const vec4& a, const vec4& b);
//		void vec4_mul(vec4* dest, const vec4& a, float b);
		RG_DECLSPEC void vec4_normalize(vec4* dest, const vec4& a);
//		float vec4_length(vec4* a);
//		float vec4_dot(vec4* a, const vec4& b);
//		void vec4_negate(vec4* dest, const vec4& a);
		RG_DECLSPEC void vec4_lerp(vec4* dest, const vec4& a, const vec4& b, double delta);

		RG_FORCE_INLINE void vec4_clone(vec4* dest, const vec4& src) { dest->x = src.x; dest->y = src.y; dest->z = src.z; dest->w = src.w; }
		RG_FORCE_INLINE void vec4_add(vec4* dest, const vec4& a, const vec4& b) { dest->x = a.x + b.x; dest->y = a.y + b.y; dest->z = a.z + b.z; dest->w = a.w + b.w; }
		RG_FORCE_INLINE void vec4_add(vec4* dest, const vec4& a, float b) { dest->x = a.x + b; dest->y = a.y + b; dest->z = a.z + b; dest->w = a.w + b; }
		RG_FORCE_INLINE void vec4_mul(vec4* dest, const vec4& a, const vec4& b) { dest->x = a.x * b.x; dest->y = a.y * b.y; dest->z = a.z * b.z; dest->w = a.w * b.w; }
		RG_FORCE_INLINE void vec4_mul(vec4* dest, const vec4& a, float b) { dest->x = a.x * b; dest->y = a.y * b; dest->z = a.z * b; dest->w = a.w * b; }

//		void vec4_normalize(vec4* dest, const vec4& a) {
//			float inv_length = invsqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
//			dest->x = a.x * inv_length;
//			dest->y = a.y * inv_length;
//			dest->z = a.z * inv_length;
//			dest->z = a.w * inv_length;
//		}

		RG_FORCE_INLINE float vec4_length(vec4* a) { return SDL_sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w); }
		RG_FORCE_INLINE float vec4_dot(vec4* a, const vec4& b) { return a->x*b.x + a->y*b.y + a->z*b.z + a->w*b.w; }
		RG_FORCE_INLINE void vec4_negate(vec4* dest, const vec4& a) { dest->x = -a.x; dest->y = -a.y; dest->z = -a.z; dest->w = -a.w; }



		// quat
		RG_DECLSPEC void quat_slerp(quat* dest, const quat& q1, const quat& q2, double delta);
		RG_DECLSPEC void quat_mul(quat* dest, const quat& q1, const quat& q2);
		RG_FORCE_INLINE void quat_invert(quat* dest, const quat& q1) { dest->x = -q1.x; dest->y = -q1.y; dest->z = -q1.z; dest->w = q1.w; }
		RG_DECLSPEC void quat_mat4(quat* dest, const mat4& matrix);
		RG_DECLSPEC void quat_axisAngle(quat* dest, const vec4& axis_angle);
	}
}


#endif /* ENGINE_CORE_MATH_VECTOR_H_ */

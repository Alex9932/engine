/*
 * vector.cpp
 *
 *  Created on: Feb 10, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#define RG_FORSE_GLM_FUNCTIONS

#include <engine/core/math/vector.h>
#include <engine/core/math/matrix.h>
#include <SDL2/SDL.h>

#ifdef RG_FORSE_GLM_FUNCTIONS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#endif

namespace Engine {
	namespace Math {
//		void vec2_clone(vec2* dest, const vec2& src) {
//			dest->x = src.x;
//			dest->y = src.y;
//		}
//
//		void vec2_add(vec2* dest, const vec2& a, const vec2& b) {
//			dest->x = a.x + b.x;
//			dest->y = a.y + b.y;
//		}
//
//		void vec2_add(vec2* dest, const vec2& a, float b) {
//			dest->x = a.x + b;
//			dest->y = a.y + b;
//		}
//
//		void vec2_mul(vec2* dest, const vec2& a, const vec2& b) {
//			dest->x = a.x * b.x;
//			dest->y = a.y * b.y;
//		}
//
//		void vec2_mul(vec2* dest, const vec2& a, float b) {
//			dest->x = a.x * b;
//			dest->y = a.y * b;
//		}
//
//		void vec2_normalize(vec2* dest, const vec2& a) {
//			float inv_length = invsqrt(a.x * a.x + a.y * a.y);
//			dest->x = a.x * inv_length;
//			dest->y = a.y * inv_length;
//		}
//
//		float vec2_length(vec2* a) {
//			return SDL_sqrt(a->x * a->x + a->y * a->y);
//		}
//
//		float vec2_dot(vec2* a, const vec2& b) {
//			float q  = a->x * b.x;
//				  q += a->y * b.y;
//			return q;
//		}
//
//		void vec2_negate(vec2* dest, const vec2& a) {
//			dest->x = -a.x;
//			dest->y = -a.y;
//		}
//
//		void vec2_lerp(vec2* dest, const vec2& a, const vec2& b, double delta) {
//			dest->x = a.x * (1.0 - delta) + b.x * delta;
//			dest->y = a.y * (1.0 - delta) + b.y * delta;
//		}

//		void vec3_clone(vec3* dest, const vec3& src) {
//			dest->x = src.x;
//			dest->y = src.y;
//			dest->z = src.z;
//		}
//
//		void vec3_add(vec3* dest, const vec3& a, const vec3& b) {
//			dest->x = a.x + b.x;
//			dest->y = a.y + b.y;
//			dest->z = a.z + b.z;
//		}
//
//		void vec3_add(vec3* dest, const vec3& a, float b) {
//			dest->x = a.x + b;
//			dest->y = a.y + b;
//			dest->z = a.z + b;
//		}
//
//		void vec3_mul(vec3* dest, const vec3& a, const vec3& b) {
//			dest->x = a.x * b.x;
//			dest->y = a.y * b.y;
//			dest->z = a.z * b.z;
//		}
//
//		void vec3_mul(vec3* dest, const vec3& a, float b) {
//			dest->x = a.x * b;
//			dest->y = a.y * b;
//			dest->z = a.z * b;
//		}

		void vec3_normalize(vec3* dest, const vec3& a) {
			float inv_length = invsqrt(a.x * a.x + a.y * a.y + a.z * a.z);
			dest->x = a.x * inv_length;
			dest->y = a.y * inv_length;
			dest->z = a.z * inv_length;
//			float length = SDL_sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
//			if(length < 0.0001) { length = 0.0001; }
//			dest->x = a.x / length;
//			dest->y = a.y / length;
//			dest->z = a.z / length;
		}

//		void vec3_cross(vec3* dest, const vec3& a, const vec3& b) {
//			dest->x = a.y * b.z - a.z * b.y;
//			dest->y = a.z * b.x - a.x * b.z;
//			dest->z = a.x * b.y - a.y * b.x;
//		}

		union SIMD_VECTOR4 {
			vec4 vec;
			__m128 m;
		};

		// SIMD
		void vec3_cross(vec3* dest, const vec3& a, const vec3& b) {

			SIMD_VECTOR4 _a1;
			SIMD_VECTOR4 _a2;
			_a1.vec.x = a.y;
			_a1.vec.y = a.z;
			_a1.vec.z = a.x;
			_a2.vec.x = a.z;
			_a2.vec.y = a.x;
			_a2.vec.z = a.y;
			SIMD_VECTOR4 _b1;
			SIMD_VECTOR4 _b2;
			_b1.vec.x = b.z;
			_b1.vec.y = b.x;
			_b1.vec.z = b.y;
			_b2.vec.x = b.y;
			_b2.vec.y = b.z;
			_b2.vec.z = b.x;

			__m128 _a = _mm_mul_ps(_a1.m, _b1.m);
			__m128 _b = _mm_mul_ps(_a2.m, _b2.m);

			SIMD_VECTOR4 _dest;
			_dest.m = _mm_sub_ps(_a, _b);

			dest->x = _dest.vec.x;
			dest->y = _dest.vec.y;
			dest->z = _dest.vec.z;
		}

//		float vec3_length(vec3* a) {
//			return SDL_sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
//		}
//
//		float vec3_dot(const vec3& a, const vec3& b) {
//			float q  = a.x * b.x;
//				  q += a.y * b.y;
//				  q += a.z * b.z;
//			return q;
//		}

//		void vec3_negate(vec3* dest, const vec3& a) {
//			dest->x = -a.x;
//			dest->y = -a.y;
//			dest->z = -a.z;
//		}

		void vec3_lerp(vec3* dest, const vec3& a, const vec3& b, double delta) {
			dest->x = a.x * (1.0 - delta) + b.x * delta;
			dest->y = a.y * (1.0 - delta) + b.y * delta;
			dest->z = a.z * (1.0 - delta) + b.z * delta;
		}

		void vec3_mul(vec3* dest, const vec3& v, const quat& q) {
			float x = v.x;
			float y = v.y;
			float z = v.z;
			float qx = q.x;
			float qy = q.y;
			float qz = q.z;
			float qw = q.w;

			// calculate quat * vector
			float ix =  qw * x + qy * z - qz * y;
			float iy =  qw * y + qz * x - qx * z;
			float iz =  qw * z + qx * y - qy * x;
			float iw = -qx * x - qy * y - qz * z;

			// calculate result * inverse quat
			dest->x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
			dest->y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
			dest->z = iz * qw + iw * -qz + ix * -qy - iy * -qx;
		}

		void vec4_axisAngle(vec4* dest, const vec3& a) {
			float c1 = SDL_cosf(a.x / 2);
			float s1 = SDL_sinf(a.x / 2);
			float c2 = SDL_cosf(a.y / 2);
			float s2 = SDL_sinf(a.y / 2);
			float c3 = SDL_cosf(a.z / 2);
			float s3 = SDL_sinf(a.z / 2);
			dest->z = c1*c2*s3 + s1*s2*c3;
			dest->x = s1*c2*c3 + c1*s2*s3;
			dest->y = c1*s2*c3 - s1*c2*s3;
			dest->w = 2 * SDL_acosf(c1*c2*c3 - s1*s2*s3);

			float norm = dest->x*dest->x + dest->y*dest->y + dest->z*dest->z;
			if (norm < 0.001) {
				dest->x = 1;
				dest->y = 0;
				dest->z = 0;
			} else {
				norm = SDL_sqrtf(norm);
				dest->x /= norm;
				dest->y /= norm;
				dest->z /= norm;
			}
		}

//		void vec4_clone(vec4* dest, const vec4& src) {
//			dest->x = src.x;
//			dest->y = src.y;
//			dest->z = src.z;
//			dest->w = src.w;
//		}
//
//		void vec4_add(vec4* dest, const vec4& a, const vec4& b) {
//			dest->x = a.x + b.x;
//			dest->y = a.y + b.y;
//			dest->z = a.z + b.z;
//			dest->w = a.w + b.w;
//		}
//
//		void vec4_add(vec4* dest, const vec4& a, float b) {
//			dest->x = a.x + b;
//			dest->y = a.y + b;
//			dest->z = a.z + b;
//			dest->w = a.w + b;
//		}
//
//		void vec4_mul(vec4* dest, const vec4& a, const vec4& b) {
//			dest->x = a.x * b.x;
//			dest->y = a.y * b.y;
//			dest->z = a.z * b.z;
//			dest->w = a.w * b.w;
//		}
//
//		void vec4_mul(vec4* dest, const vec4& a, float b) {
//			dest->x = a.x * b;
//			dest->y = a.y * b;
//			dest->z = a.z * b;
//			dest->w = a.w * b;
//		}

		void vec4_normalize(vec4* dest, const vec4& a) {
			float inv_length = invsqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
			dest->x = a.x * inv_length;
			dest->y = a.y * inv_length;
			dest->z = a.z * inv_length;
			dest->z = a.w * inv_length;
		}

//		float vec4_length(vec4* a) {
//			return SDL_sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
//		}

//		float vec4_dot(vec4* a, const vec4& b) {
//			float q  = a->x * b.x;
//				  q += a->y * b.y;
//				  q += a->z * b.z;
//				  q += a->w * b.w;
//			return q;
//		}
//
//		void vec4_negate(vec4* dest, const vec4& a) {
//			dest->x = -a.x;
//			dest->y = -a.y;
//			dest->z = -a.z;
//			dest->w = -a.w;
//		}

		void vec4_lerp(vec4* dest, const vec4& a, const vec4& b, double delta) {
			dest->x = a.x * (1.0 - delta) + b.x * delta;
			dest->y = a.y * (1.0 - delta) + b.y * delta;
			dest->z = a.z * (1.0 - delta) + b.z * delta;
			dest->w = a.w * (1.0 - delta) + b.w * delta;
		}

		void quat_slerp(quat* dest, const quat& q1, const quat& q2, double lambda) {

#ifdef RG_FORSE_GLM_FUNCTIONS
			// TODO temporary using glm's slerp function
			glm::quat _q1;
			glm::quat _q2;
			_q1.x = q1.x;
			_q1.y = q1.y;
			_q1.z = q1.z;
			_q1.w = q1.w;
			_q2.x = q2.x;
			_q2.y = q2.y;
			_q2.z = q2.z;
			_q2.w = q2.w;
			glm::quat q = glm::slerp(_q1, _q2, (float)lambda);
			dest->x = q.x;
			dest->y = q.y;
			dest->z = q.z;
			dest->w = q.w;
#else

			float dot = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
			float blendI = 1.0f - lambda;
			if (dot < 0) {
				dest->w = blendI * q1.w + lambda * -q2.w;
				dest->x = blendI * q1.x + lambda * -q2.x;
				dest->y = blendI * q1.y + lambda * -q2.y;
				dest->z = blendI * q1.z + lambda * -q2.z;
			} else {
				dest->w = blendI * q1.w + lambda * q2.w;
				dest->x = blendI * q1.x + lambda * q2.x;
				dest->y = blendI * q1.y + lambda * q2.y;
				dest->z = blendI * q1.z + lambda * q2.z;
			}

#endif
		}

		void quat_mul(quat* dest, const quat& q1, const quat& q2) {
			float qax = q1.x; float qay = q1.y; float qaz = q1.z; float qaw = q1.w;
			float qbx = q2.x; float qby = q2.y; float qbz = q2.z; float qbw = q2.w;
			dest->x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
			dest->y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
			dest->z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
			dest->w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;
		}

//		void quat_invert(quat* dest, const quat& q1) {
//			dest->x = -q1.x;
//			dest->y = -q1.y;
//			dest->z = -q1.z;
//			dest->w = q1.w;
//		}

		void quat_mat4(quat* dest, const mat4& matrix) {

#ifdef RG_FORSE_GLM_FUNCTIONS
			// TODO temporary using glm's quat_cast function
			glm::mat4 mat;
			SDL_memcpy(&mat[0][0], &matrix, sizeof(mat4));
			glm::quat q = glm::quat_cast(mat);
			dest->x = q.x;
			dest->y = q.y;
			dest->z = q.z;
			dest->w = q.w;
#else

			float w, x, y, z;
			float diagonal = matrix.m00 + matrix.m11 + matrix.m22;

			if (diagonal > 0) {
				float w4 = (float) (SDL_sqrtf(diagonal + 1) * 2);
				w = w4 / 4;
				x = (matrix.m21 - matrix.m12) / w4;
				y = (matrix.m02 - matrix.m20) / w4;
				z = (matrix.m10 - matrix.m01) / w4;
			} else if ((matrix.m00 > matrix.m11) && (matrix.m00 > matrix.m22)) {
				float x4 = (float) (SDL_sqrtf(1 + matrix.m00 - matrix.m11 - matrix.m22) * 2);
				w = (matrix.m21 - matrix.m12) / x4;
				x = x4 / 4;
				y = (matrix.m01 + matrix.m10) / x4;
				z = (matrix.m02 + matrix.m20) / x4;
			} else if (matrix.m11 > matrix.m22) {
				float y4 = (float) (SDL_sqrtf(1 + matrix.m11 - matrix.m00 - matrix.m22) * 2);
				w = (matrix.m02 - matrix.m20) / y4;
				x = (matrix.m01 + matrix.m10) / y4;
				y = y4 / 4;
				z = (matrix.m12 + matrix.m21) / y4;
			} else {
				float z4 = (float) (SDL_sqrtf(1 + matrix.m22 - matrix.m00 - matrix.m11) * 2);
				w = (matrix.m10 - matrix.m01) / z4;
				x = (matrix.m02 + matrix.m20) / z4;
				y = (matrix.m12 + matrix.m21) / z4;
				z = z4 / 4;
			}

			dest->x = x;
			dest->y = y;
			dest->z = z;
			dest->w = w;

#endif
		}

		void quat_axisAngle(quat* dest, const vec4& axis_angle) {
			float half = axis_angle.w / 2.0;
			float s = SDL_sinf(half);
			dest->x = axis_angle.x * s;
			dest->y = axis_angle.y * s;
			dest->z = axis_angle.z * s;
			dest->w = SDL_cosf(half);
		}
	}
}

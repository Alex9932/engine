/*
 * matrix.cpp
 *
 *  Created on: Feb 10, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/math/matrix.h>
#include <engine/core/math/vector.h>
#include <SDL2/SDL.h>

#undef far
#undef near

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

namespace Engine {
	namespace Math {

		void mat3_identity(mat3* mat, float scale) {
			mat->m00 = scale; mat->m01 = 0;     mat->m02 = 0;
			mat->m10 = 0;     mat->m11 = scale; mat->m12 = 0;
			mat->m20 = 0;     mat->m21 = 0;     mat->m22 = scale;
		}

		void mat3_mul(vec3* dest, vec3* left, mat3* right) {
			dest->x = left->x * right->m00 + left->y * right->m01 + left->z * right->m02;
			dest->y = left->x * right->m10 + left->y * right->m11 + left->z * right->m12;
			dest->z = left->x * right->m20 + left->y * right->m21 + left->z * right->m22;
		}

		void mat3_mul(mat3* dest, mat3* left, mat3* right) {
			float* a = (float*)dest;
			float* b = (float*)left;
			float* c = (float*)right;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					float sum = 0.0;
					for (int k = 0; k < 3; k++) {
						sum = sum + b[i * 3 + k] * c[k * 3 + j];
					}
					a[i * 3 + j] = sum;
				}
			}
		}

		void mat3_rotx(mat3* mat, float angle) {
			mat->m00 = 1; mat->m01 = 0;              mat->m02 = 0;
			mat->m10 = 0; mat->m11 = SDL_cos(angle); mat->m12 = -SDL_sin(angle);
			mat->m20 = 0; mat->m21 = SDL_sin(angle); mat->m22 =  SDL_cos(angle);
		}

		void mat3_roty(mat3* mat, float angle) {
			mat->m00 =  SDL_cos(angle); mat->m01 = 0; mat->m02 = SDL_sin(angle);
			mat->m10 = 0;               mat->m11 = 1; mat->m12 = 0;
			mat->m20 = -SDL_sin(angle); mat->m21 = 0; mat->m22 = SDL_cos(angle);
		}

		void mat3_rotz(mat3* mat, float angle) {
			mat->m00 = SDL_cos(angle); mat->m01 = -SDL_sin(angle); mat->m02 = 0;
			mat->m10 = SDL_sin(angle); mat->m11 =  SDL_cos(angle); mat->m12 = 0;
			mat->m20 = 0;              mat->m21 = 0;               mat->m22 = 1;
		}

		void mat3_rotate(mat3* mat, const vec3& angles) {
			mat3 rx, ry, rz, ryz;
			mat3_rotx(&rx, angles.x);
			mat3_roty(&ry, angles.y);
			mat3_rotz(&rz, angles.z);
			mat3_mul(&ryz, &rz, &ry);
			mat3_mul(mat, &rx, &ryz);
		}


		void mat4_identity(mat4* mat, const vec3& scale) {
			mat->m00 = scale.x; mat->m01 = 0;       mat->m02 = 0;       mat->m03 = 0;
			mat->m10 = 0;       mat->m11 = scale.y; mat->m12 = 0;       mat->m13 = 0;
			mat->m20 = 0;       mat->m21 = 0;       mat->m22 = scale.z; mat->m23 = 0;
			mat->m30 = 0;       mat->m31 = 0;       mat->m32 = 0;       mat->m33 = 1;
		}

		void mat4_identity(mat4* mat, float scale) {
			mat->m00 = scale; mat->m01 = 0;     mat->m02 = 0;     mat->m03 = 0;
			mat->m10 = 0;     mat->m11 = scale; mat->m12 = 0;     mat->m13 = 0;
			mat->m20 = 0;     mat->m21 = 0;     mat->m22 = scale; mat->m23 = 0;
			mat->m30 = 0;     mat->m31 = 0;     mat->m32 = 0;     mat->m33 = 1;
		}

		void mat4_mul(vec4* dest, vec4* left, mat4* right) {
			dest->x = left->x * right->m00 + left->y * right->m01 + left->z * right->m02 + left->w * right->m03;
			dest->y = left->x * right->m10 + left->y * right->m11 + left->z * right->m12 + left->w * right->m13;
			dest->z = left->x * right->m20 + left->y * right->m21 + left->z * right->m22 + left->w * right->m23;
			dest->w = left->x * right->m30 + left->y * right->m31 + left->z * right->m32 + left->w * right->m33;
		}

#ifdef RG_SIMD_MATH
		static RG_FORCE_INLINE __m128 MulVecMatSSE(__m128 V, const mat4_simd& M){

		    __m128 vX = _mm_shuffle_ps(V, V, 0x00);
		    __m128 vY = _mm_shuffle_ps(V, V, 0x55);
		    __m128 vZ = _mm_shuffle_ps(V, V, 0xAA);
		    __m128 vW = _mm_shuffle_ps(V, V, 0xFF);

		    __m128 Result = _mm_mul_ps(vX, M.rows[0]);
		    Result = _mm_add_ps(Result, _mm_mul_ps(vY, M.rows[1]));
		    Result = _mm_add_ps(Result, _mm_mul_ps(vZ, M.rows[2]));
		    Result = _mm_add_ps(Result, _mm_mul_ps(vW, M.rows[3]));

		    return(Result);
		}
#endif

		void mat4_mul(mat4* dest, mat4* left, mat4* right) {

#ifdef RG_SIMD_MATH

			mat4_simd R;
			mat4_simd A;
			mat4_simd B;
			R.matrix = *dest;
			A.matrix = *right;
			B.matrix = *left;

			R.rows[0] = MulVecMatSSE(A.rows[0], B);
			R.rows[1] = MulVecMatSSE(A.rows[1], B);
			R.rows[2] = MulVecMatSSE(A.rows[2], B);
			R.rows[3] = MulVecMatSSE(A.rows[3], B);

			*dest = R.matrix;

#else
			float* a = (float*)dest;
			float* b = (float*)right;
			float* c = (float*)left;
			for (Sint32 i = 0; i < 4; ++i) {
				for (Sint32 j = 0; j < 4; ++j) {
					*a = b[0]*c[0 + j] + b[1]*c[4 + j] + b[2]*c[8 + j] + b[3]*c[12 + j];
					a++;
				}
				b += 4;
			}
#endif
		}

		void mat4_rotx(mat4* mat, float angle) {
			mat->m00 = 1; mat->m01 = 0;              mat->m02 = 0;               mat->m03 = 0;
			mat->m10 = 0; mat->m11 = SDL_cos(angle); mat->m12 = -SDL_sin(angle); mat->m13 = 0;
			mat->m20 = 0; mat->m21 = SDL_sin(angle); mat->m22 =  SDL_cos(angle); mat->m23 = 0;
			mat->m30 = 0; mat->m31 = 0;              mat->m32 = 0;               mat->m33 = 1;
		}

		void mat4_roty(mat4* mat, float angle) {
			mat->m00 =  SDL_cos(angle); mat->m01 = 0; mat->m02 = SDL_sin(angle); mat->m03 = 0;
			mat->m10 = 0;               mat->m11 = 1; mat->m12 = 0;              mat->m13 = 0;
			mat->m20 = -SDL_sin(angle); mat->m21 = 0; mat->m22 = SDL_cos(angle); mat->m23 = 0;
			mat->m30 = 0;               mat->m31 = 0; mat->m32 = 0;              mat->m33 = 1;
		}

		void mat4_rotz(mat4* mat, float angle) {
			mat->m00 = SDL_cos(angle); mat->m01 = -SDL_sin(angle); mat->m02 = 0; mat->m03 = 0;
			mat->m10 = SDL_sin(angle); mat->m11 =  SDL_cos(angle); mat->m12 = 0; mat->m13 = 0;
			mat->m20 = 0;              mat->m21 = 0;               mat->m22 = 1; mat->m23 = 0;
			mat->m30 = 0;              mat->m31 = 0;               mat->m32 = 0; mat->m33 = 1;
		}

		void mat4_rotate(mat4* mat, const vec3& angles) {

//			TODO
//			const c = Math.cos( angle );
//			const s = Math.sin( angle );
//			const t = 1 - c;
//			const x = axis.x, y = axis.y, z = axis.z;
//			const tx = t * x, ty = t * y;
//
//			this.set(
//
//				tx * x + c, tx * y - s * z, tx * z + s * y, 0,
//				tx * y + s * z, ty * y + c, ty * z - s * x, 0,
//				tx * z - s * y, ty * z + s * x, t * z * z + c, 0,
//				0, 0, 0, 1
//
//			);

			mat4 rx, ry, rz, ryz;
			mat4_rotx(&rx, angles.x);
			mat4_roty(&ry, angles.y);
			mat4_rotz(&rz, angles.z);
			mat4_mul(&ryz, &rz, &ry);
			mat4_mul(mat, &rx, &ryz);
		}

		void mat4_quat(mat4* mat, const quat& quat) {
			mat->m00 = 1.0f - 2.0f * (quat.y*quat.y + quat.z*quat.z);
			mat->m10 =        2.0f * (quat.x*quat.y + quat.w*quat.z);
			mat->m20 =        2.0f * (quat.x*quat.z - quat.w*quat.y);
			mat->m30 = 0.0f;
			mat->m01 =        2.0f * (quat.x*quat.y - quat.w*quat.z);
			mat->m11 = 1.0f - 2.0f * (quat.x*quat.x + quat.z*quat.z);
			mat->m21 =        2.0f * (quat.y*quat.z + quat.w*quat.x);
			mat->m31 = 0.0f;
			mat->m02 =        2.0f * (quat.x*quat.z + quat.w*quat.y);
			mat->m12 =        2.0f * (quat.y*quat.z - quat.w*quat.x);
			mat->m22 = 1.0f - 2.0f * (quat.x*quat.x + quat.y*quat.y);
			mat->m32 = 0.0f;
			mat->m03 = 0.0f;
			mat->m13 = 0.0f;
			mat->m23 = 0.0f;
			mat->m33 = 1.0f;
		}

		void mat4_ortho(mat4* mat, float r, float l, float t, float b, float f, float n) {
			mat->m00 = 2 / (r - l); mat->m01 = 0;           mat->m02 = 0;           mat->m03 = -((r + l) / (r - l));
			mat->m10 = 0;           mat->m11 = 2 / (t - b); mat->m12 = 0;           mat->m13 = -((t + b) / (t - b));
			mat->m20 = 0;           mat->m21 = 0;           mat->m22 = 2 / (f - n); mat->m23 = -((f + n) / (f - n));
			mat->m30 = 0;           mat->m31 = 0;           mat->m32 = 0;           mat->m33 = 1;
		}

		void mat4_frustum(mat4* mat, float aspect, float fov, float near, float far) {
			float y_scale = (float)((1 / SDL_tan(fov / 2)));
			float x_scale = y_scale / aspect;
			float frustum_length = far - near;
			mat->m00 = x_scale; mat->m01 = 0;       mat->m02 = 0;                                mat->m03 = 0;
			mat->m10 = 0;       mat->m11 = y_scale; mat->m12 = 0;                                mat->m13 = 0;
			mat->m20 = 0;       mat->m21 = 0;       mat->m22 = -((far + near) / frustum_length); mat->m23 = -((2 * near * far) / frustum_length);
			mat->m30 = 0;       mat->m31 = 0;       mat->m32 = -1;                               mat->m33 = 0;
		}

		void mat4_lookAt(mat4* mat, const vec3& pos, const vec3& center, const vec3& up) {

			vec3 _fwd;
			vec3 _right;
			vec3 _up;
			vec3 pc = {pos.x - center.x, pos.y - center.y, pos.z - center.z};
			vec3 nu;
			vec3_normalize(&_fwd, pc);
			vec3_normalize(&nu, up);
			vec3_cross(&_right, nu, _fwd);
			vec3_cross(&_up, _fwd, _right);

		    mat->m00 = _right.x;
		    mat->m10 = _right.y;
		    mat->m20 = _right.z;
		    mat->m01 = _up.x;
		    mat->m11 = _up.y;
		    mat->m21 = _up.z;
		    mat->m02 = _fwd.x;
		    mat->m12 = _fwd.y;
		    mat->m22 = _fwd.z;
		    mat->m03 = pos.x;
		    mat->m13 = pos.y;
		    mat->m23 = pos.z;

//		    glm::mat4 m = glm::lookAt(
//		    		glm::vec3(pos.x, pos.y, pos.z),
//					glm::vec3(center.x, center.y, center.z),
//					glm::vec3(up.x, up.y, up.z));
//
//		    memcpy(mat, &m[0][0], sizeof(mat4));

		}

		void mat4_view(mat4* mat, const vec3& pos, const vec2& angles) {
			mat4 rx;
			mat4 ry;
			mat4 translation;
			mat4 rotation;
			vec3 neg;
			vec3_negate(&neg, pos);
			mat4_identity(&translation);
			mat4_roty(&rx, angles.x);
			mat4_rotx(&ry, angles.y);
			mat4_translate(&translation, neg);
			mat4_mul(&rotation, &ry, &rx);
			mat4_mul(mat, &rotation, &translation);
		}

		void mat4_view(mat4* mat, const vec3& pos, const vec3& angles) {
			mat4 translation;
			mat4 rotation;
			vec3 neg;
			vec3_negate(&neg, pos);
			mat4_identity(&translation);
			mat4_rotate(&rotation, angles);
			mat4_translate(&translation, neg);
			mat4_mul(mat, &rotation, &translation);
		}

		void mat4_model(mat4* mat, const vec3& pos, const vec3& angles, float scale) {
			mat4_model(mat, pos, angles, {scale, scale, scale});
		}

		void mat4_model(mat4* mat, const vec3& pos, const vec3& angles, const vec3& scale) {
//			mat4 rotation;
//			mat4 translation;
//			mat4_identity(&translation, scale);
//			mat4_rotate(&rotation, angles);
//			mat4_translate(&translation, pos);
//			mat4_mul(mat, &translation, &rotation);
			mat4 m_scale = IDENTITY_MAT4;
			mat4 m_rotation = IDENTITY_MAT4;
			mat4 m_translation = IDENTITY_MAT4;
			mat4 m_tmp = IDENTITY_MAT4;
			mat4_identity(&m_scale, scale);
			mat4_rotate(&m_rotation, angles);
			mat4_translate(&m_translation, pos);
			mat4_mul(&m_tmp, m_translation, m_rotation);
			mat4_mul(mat, m_tmp, m_scale);
//			mat4_mul(mat, m_scale, m_tmp);
		}

		void mat4_inverse(mat4* dest, mat4* src) {
			float* m = (float*)src;
			float* _dest = (float*)dest;
			float inv[16];
			inv[0]  =  m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
			inv[4]  = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
			inv[8]  =  m[4] * m[9]  * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
			inv[12] = -m[4] * m[9]  * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
			inv[1]  = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
			inv[5]  =  m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
			inv[9]  = -m[0] * m[9]  * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
			inv[13] =  m[0] * m[9]  * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
			inv[2]  =  m[1] * m[6]  * m[15] - m[1] * m[7]  * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7]  - m[13] * m[3] * m[6];
			inv[6]  = -m[0] * m[6]  * m[15] + m[0] * m[7]  * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7]  + m[12] * m[3] * m[6];
			inv[10] =  m[0] * m[5]  * m[15] - m[0] * m[7]  * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7]  - m[12] * m[3] * m[5];
			inv[14] = -m[0] * m[5]  * m[14] + m[0] * m[6]  * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6]  + m[12] * m[2] * m[5];
			inv[3]  = -m[1] * m[6]  * m[11] + m[1] * m[7]  * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9]  * m[2] * m[7]  + m[9]  * m[3] * m[6];
			inv[7]  =  m[0] * m[6]  * m[11] - m[0] * m[7]  * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8]  * m[2] * m[7]  - m[8]  * m[3] * m[6];
			inv[11] = -m[0] * m[5]  * m[11] + m[0] * m[7]  * m[9]  + m[4] * m[1] * m[11] - m[4] * m[3] * m[9]  - m[8]  * m[1] * m[7]  + m[8]  * m[3] * m[5];
			inv[15] =  m[0] * m[5]  * m[10] - m[0] * m[6]  * m[9]  - m[4] * m[1] * m[10] + m[4] * m[2] * m[9]  + m[8]  * m[1] * m[6]  - m[8]  * m[2] * m[5];

			float det = 1.0 / (m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12]);

			for (size_t i = 0; i < 16; i++) {
				_dest[i] = inv[i] * det;
			}
		}

		float mat4_determinant(const Engine::Math::mat4& m) {

//			glm::mat4 mat;
//			SDL_memcpy(&mat[0][0], &m, sizeof(mat4));
//			return glm::determinant(mat);

			return
			m.m03*m.m12*m.m21*m.m30 - m.m02*m.m13*m.m21*m.m30 - m.m03*m.m11*m.m22*m.m30 + m.m01*m.m13*m.m22*m.m30+
			m.m02*m.m11*m.m23*m.m30 - m.m01*m.m12*m.m23*m.m30 - m.m03*m.m12*m.m20*m.m31 + m.m02*m.m13*m.m20*m.m31+
			m.m03*m.m10*m.m22*m.m31 - m.m00*m.m13*m.m22*m.m31 - m.m02*m.m10*m.m23*m.m31 + m.m00*m.m12*m.m23*m.m31+
			m.m03*m.m11*m.m20*m.m32 - m.m01*m.m13*m.m20*m.m32 - m.m03*m.m10*m.m21*m.m32 + m.m00*m.m13*m.m21*m.m32+
			m.m01*m.m10*m.m23*m.m32 - m.m00*m.m11*m.m23*m.m32 - m.m02*m.m11*m.m20*m.m33 + m.m01*m.m12*m.m20*m.m33+
			m.m02*m.m10*m.m21*m.m33 - m.m00*m.m12*m.m21*m.m33 - m.m01*m.m10*m.m22*m.m33 + m.m00*m.m11*m.m22*m.m33;
		}

		void mat4_decompose(vec3* position, quat* quaternion, vec3* scale, const mat4& matrix) {
			vec3 _x = {matrix.m00, matrix.m10, matrix.m20};
			vec3 _y = {matrix.m01, matrix.m11, matrix.m21};
			vec3 _z = {matrix.m02, matrix.m12, matrix.m22};
			float sx = vec3_length(&_x);
			float sy = vec3_length(&_y);
			float sz = vec3_length(&_z);

			float det = mat4_determinant(matrix);

			if(det < 0) {sx = -sx; }

			position->x = matrix.m03;
			position->y = matrix.m13;
			position->z = matrix.m23;

			mat4 _m;
			SDL_memcpy(&_m, &matrix, sizeof(mat4));

			float invSX = 1.0 / sx;
			float invSY = 1.0 / sy;
			float invSZ = 1.0 / sz;

			_m.m00 *= invSX;
			_m.m10 *= invSX;
			_m.m20 *= invSX;

			_m.m01 *= invSY;
			_m.m11 *= invSY;
			_m.m21 *= invSY;

			_m.m02 *= invSZ;
			_m.m12 *= invSZ;
			_m.m22 *= invSZ;

			quat_mat4(quaternion, _m);

			scale->x = sx;
			scale->y = sy;
			scale->z = sz;
		}

	}
}

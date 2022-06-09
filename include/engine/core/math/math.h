/*
 * math.h
 *
 *  Created on: Feb 10, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_MATH_MATH_H_
#define ENGINE_CORE_MATH_MATH_H_

#include <engine/engine.h>

#define RG_PI       3.1415926535
#define RG_HALF_PI  1.57079632675
#define RG_EPSILONF FLT_EPSILON

#define RG_SIMD_MATH

#ifdef RG_SIMD_MATH
#include <xmmintrin.h>
#endif

namespace Engine {
	namespace Math {
		/*
		 * Angles in radians (anglex, angley, anglez and fov)
		 *
		 * OpenGL matrix
		 * | 0 4  8 12 |  | m00 m01 m02 m03 |
		 * | 1 5  9 13 |  | m10 m11 m12 m13 |
		 * | 2 6 10 14 |  | m20 m21 m22 m23 |
		 * | 3 7 11 15 |  | m30 m31 m32 m33 |
		 */

		typedef struct mat3 { float m00; float m10; float m20;
							  float m01; float m11; float m21;
							  float m02; float m12; float m22; } mat3;

		typedef struct mat4 { float m00; float m10; float m20; float m30;
							  float m01; float m11; float m21; float m31;
							  float m02; float m12; float m22; float m32;
							  float m03; float m13; float m23; float m33; } mat4;

		extern RG_DECLSPEC const mat4 IDENTITY_MAT3;
		extern RG_DECLSPEC const mat4 IDENTITY_MAT4;


#ifdef RG_SIMD_MATH
		typedef struct mat4_simd {
			union {
				mat4 matrix;
				__m128 rows[4];
			};
		} mat4_simd;
#endif

		typedef struct vec2 { float x; float y; } vec2;
		typedef struct vec3 { float x; float y; float z; } vec3;
		typedef struct vec4 { float x; float y; float z; float w; } vec4;
		typedef struct ivec2 { int x; int y; } ivec2;
		typedef struct ivec3 { int x; int y; int z; } ivec3;
		typedef struct ivec4 { int x; int y; int z; int w; } ivec4;
		typedef vec4 quat;


		RG_DECLSPEC double toradians(double angle);
		RG_DECLSPEC double todegrees(double angle);

		RG_DECLSPEC double invsqrt(double a);

	}
}


#endif /* ENGINE_CORE_MATH_MATH_H_ */

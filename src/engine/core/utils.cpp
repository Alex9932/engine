/*
 * utils.cpp
 *
 *  Created on: Feb 22, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/utils.h>
#include <engine/core/logger.h>

namespace Engine {
	namespace Helper {

		void PrintMatrix(Engine::Math::mat4* m) {
			rgLogInfo(RG_LOG_GAME, " ~ ~ ~ ~ ~ ~ ~ ");
			rgLogInfo(RG_LOG_GAME, "%f %f %f %f", m->m00, m->m01, m->m02, m->m03);
			rgLogInfo(RG_LOG_GAME, "%f %f %f %f", m->m10, m->m11, m->m12, m->m13);
			rgLogInfo(RG_LOG_GAME, "%f %f %f %f", m->m20, m->m21, m->m22, m->m23);
			rgLogInfo(RG_LOG_GAME, "%f %f %f %f", m->m30, m->m31, m->m32, m->m33);
		}

	}
}

/*
 * loader.h
 *
 *  Created on: Feb 23, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_LOADER_H_
#define ENGINE_CORE_LOADER_H_

#include <engine/engine.h>
#include <engine/sound/sound_loader.h>
#include <engine/core/geometry.h>
#include <GL/glew.h>
#include <AL/al.h>

namespace Engine {
	namespace Loader {

		RG_DECLSPEC GLuint LoadTexture(String path);
		RG_DECLSPEC void FreeTexture(GLuint t);

		RG_DECLSPEC Sound::SoundData LoadSound(Sound::SoundLoader* loader, String path);
		RG_DECLSPEC void FreeSound(Sound::SoundData s);

		RG_DECLSPEC Geometry* LoadGeometry(GeometryLoader* loader, String path);
		RG_DECLSPEC void FreeGeometry(Geometry* geom);

	}
}


#endif /* ENGINE_CORE_LOADER_H_ */

/*
 * render.h
 *
 *  Created on: Feb 13, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_RENDER_H_
#define ENGINE_RENDER_RENDER_H_

#include <engine/core/allocator.h>
#include "engine/core/math/math.h"
#include "engine/render/model.h"
#include <GL/glew.h>

namespace Engine {
	namespace Render {

		RG_DECLSPEC void Initialize();
		RG_DECLSPEC void Destroy();
		RG_DECLSPEC void Clear();
		RG_DECLSPEC void Render();
		RG_DECLSPEC void ShowProfiler();
		RG_DECLSPEC void HideProfiler();
		RG_DECLSPEC void ShowConsole();
		RG_DECLSPEC void HideConsole();

		RG_DECLSPEC void ImGuiBegin();
		RG_DECLSPEC void ImGuiEnd();

		RG_DECLSPEC Allocator* GetGeomAllocator();

		RG_DECLSPEC void ClearColor(const Math::vec4& color);

		RG_DECLSPEC void EnableDepthTest();
		RG_DECLSPEC void DisableDepthTest();

		RG_DECLSPEC void EnableBlend(GLenum sfactor, GLenum dfactor);
		RG_DECLSPEC void DisableBlend();

		RG_DECLSPEC Uint32 RegisterModel(Model* model);
		RG_DECLSPEC Model* GetRegisteredModel(Uint32 id);
		RG_DECLSPEC Uint32 RegisterRiggedModel(RiggedModel* model);
		RG_DECLSPEC RiggedModel* GetRegisteredRiggedModel(Uint32 id);
	}
}


#endif /* ENGINE_RENDER_RENDER_H_ */

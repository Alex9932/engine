/*
 * overlay.h
 *
 *  Created on: Feb 21, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_OVERLAY_H_
#define ENGINE_RENDER_OVERLAY_H_

#include "engine/render/font.h"

namespace Engine {
	namespace Render {

		RG_DECLSPEC void InitOverlay();
		RG_DECLSPEC void RenderOverlay();
		RG_DECLSPEC void DestroyOverlay();
		RG_DECLSPEC Font* GetDefaultFont();

	}
}


#endif /* ENGINE_RENDER_OVERLAY_H_ */

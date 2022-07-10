/*
 * r_shader.h
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_R_SHADER_H_
#define ENGINE_RENDER_R_SHADER_H_

#include <engine/engine.h>

namespace Engine {
	namespace Render {

		typedef struct ShaderInfo {
			String vertex_path;
			String fragment_path;
		} ShaderInfo;

		// TODO rewrite this
		class RG_DECLSPEC Shader {

			protected:

			public:
				Shader(const ShaderInfo& info) {}
				virtual ~Shader() {}

		};

	}
}

#endif /* ENGINE_RENDER_R_SHADER_H_ */

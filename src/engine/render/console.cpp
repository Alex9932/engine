/*
 * console.cpp
 *
 *  Created on: Apr 3, 2022
 *      Author: alex9932
 */

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT

#include <engine/core/command/command.h>
#include <engine/core/math/math.h>
#include <engine/core/logger.h>
#include <engine/core/event.h>
#include <engine/core/utf8.h>
#include <engine/render/console.h>
#include <engine/render/render2d.h>
#include <engine/render/fontrenderer.h>
#include <engine/render/window.h>
#include <engine/render/overlay.h>

#define CONSOLE_HEIGHT 360
#define COMMAND_LENGTH 1024
#define BACKSPACE_TIME 1.2
#define BS_DELAY 10

/*
 *
 * Add append function
 * Implement cursor functionality
 *
 */

namespace Engine {
	namespace Render {
		static double bs_time = 0;
		static bool backspace = false;
		static bool console_shown = false;
		static Uint16 command_buffer[COMMAND_LENGTH];

		static Uint32 cursor = 0;
		static UTF8Decoder* utf8_decoder;
		static UTF8Encoder* utf8_encoder;

		static Math::vec4 colors[] = {
				{1, 0, 0, 1},
				{1, 0.3, 0.3, 1},
				{0.9, 0.8, 0, 1},
				{0.2, 1, 0.4, 1},
				{0.3, 0.4, 0.9, 1},
				{1, 1, 1, 1}
		};

		static Math::vec4* GetColor(SDL_LogPriority p) {
			switch(p) {
				case SDL_LOG_PRIORITY_VERBOSE:  return &colors[4];
				case SDL_LOG_PRIORITY_DEBUG:    return &colors[5];
				case SDL_LOG_PRIORITY_INFO:     return &colors[3];
				case SDL_LOG_PRIORITY_WARN:     return &colors[2];
				case SDL_LOG_PRIORITY_ERROR:    return &colors[1];
				case SDL_LOG_PRIORITY_CRITICAL: return &colors[0];
				default:                        return &colors[4];
			}
		}

		static Uint32 Strlen16(Uint16* str) {
			Uint32 i = 0;
			for(;; i++) {
				Uint16 c = str[i];
				if(c == 0 || c == 10) { break; }
			}
			return i;
		}

		static void Backspace() {
			if(cursor > 0) {
				Uint32 cb_len = Strlen16(command_buffer);
				Uint32 i = 0;
				for (i = cursor - 1; i < cb_len + 1; ++i) {
					command_buffer[i] = command_buffer[i + 1];
				}
				cursor--;
			}
		}

		static Uint32 Append(Uint32 pointer, String str) {
			utf8_decoder->DecodeString(str);
			Uint16* res = utf8_decoder->GetResult();
			Uint32 len = Strlen16(res);
			Uint32 cb_len = Strlen16(command_buffer);
//			rgLogWarn(RG_LOG_SYSTEM, "cb_len: %d, len %d, ptr %d, Str: %s", cb_len, len, pointer, str);

			for (Sint32 i = cb_len + 1; i >= (Sint32)pointer; i--) {
				command_buffer[i + len] = command_buffer[i];
			}

			SDL_memcpy(&command_buffer[pointer], res, sizeof(Uint16) * len);

			return len;
		}

		static bool Input(SDL_Event* event) {
			if(!console_shown) { return true; }

			if(event->type == SDL_TEXTINPUT) {
				cursor += Append(cursor, event->text.text);
			} else if(event->type == SDL_KEYDOWN) {
				switch (event->key.keysym.scancode) {
					case SDL_SCANCODE_RETURN: { // Execute command
						utf8_encoder->EncodeString(command_buffer);
						String result = utf8_encoder->GetResult();
						rgLogInfo(RG_LOG_GAME, "@ %s\n", result);
						Command::ExecuteCommand(result);
						SDL_memset(command_buffer, 0, 1024);
						cursor = 0;
						break;
					}
					case SDL_SCANCODE_BACKSPACE: {
						Backspace();
						backspace = true;
						bs_time = BACKSPACE_TIME;
						break;
					}
					case SDL_SCANCODE_LEFT: {
						if(cursor > 0) { cursor--; }
						break;
					}
					case SDL_SCANCODE_RIGHT: {
						if(cursor < Strlen16(command_buffer)) { cursor++; }
						break;
					}
					case SDL_SCANCODE_HOME: {
						cursor = 0;
						break;
					}
					case SDL_SCANCODE_END: {
						cursor = Strlen16(command_buffer);
						break;
					}
					default:
						break;
				}
			} else if(event->type == SDL_KEYUP) {
				switch (event->key.keysym.scancode) {
					case SDL_SCANCODE_BACKSPACE:
						backspace = false;
						bs_time = 0;
						break;
					default:
						break;
				}
			}

			return true;
		}

		void InitConsole() {
			utf8_decoder = GetUTF8Decoder();
			utf8_encoder = GetUTF8Encoder();
			Event::RegisterEventHandler(Input);
		}

		void RenderConsole() {
			if(backspace) {
				bs_time -= GetDeltaTime();
				bool need_back = ((Uint32)(bs_time * BS_DELAY) % BS_DELAY) == 0;
				if(bs_time < 0 && need_back) {
					Backspace();
				}
			}

			R2D_BindTexture(0);
			R2D_SetColor({0.2, 0.2, 0.2, 0.80f});
			Math::vec2 size;
			Window::GetWindowSize(&size);
			float y_offset = size.y - CONSOLE_HEIGHT;
			R2D_DrawQuad({0, y_offset}, {size.x, CONSOLE_HEIGHT});

//			rgLogInfo(RG_LOG_SYSTEM, "Console pos: %f %f (%f %d)", size.x, size.y, y_offset, CONSOLE_HEIGHT);

			Uint32 lines = Logger::GetLines();
			char l[COMMAND_LENGTH];

			sprintf(l, "[%d]", cursor);
			RenderString(GetDefaultFont(), l, {size.x - 120, y_offset + 7}, {1, 1, 1, 1}, 0.25);

			sprintf(l, "[%d]", Logger::GetMessages());
			RenderString(GetDefaultFont(), l, {size.x - 70, y_offset + 7}, {0.8, 0.7, 0.1, 1}, 0.27);

			RenderString(GetDefaultFont(), ">", {5, y_offset + 5}, {1, 1, 1, 1}, 0.25);
			RenderRawString(GetDefaultFont(), command_buffer, {17, y_offset + 5}, {1, 1, 1, 1}, 0.25);
			if((Uint32)GetUptime() % 2) {
				float len = GetDefaultFont()->GetRawStringLength(command_buffer, 0.25, cursor);
				RenderString(GetDefaultFont(), "|", {17 + len, y_offset + 5}, {1, 1, 1, 1}, 0.25);
			}

			for (Uint32 i = 0; i < lines; ++i) {
//				sprintf(l, "%s", Logger::GetLine(i));
				Math::vec4 color = *GetColor(Logger::GetLinePriority(i));
				RenderString(GetDefaultFont(), Logger::GetLine(i), {5, y_offset + 5 + 17 + (i * 17)}, color, 0.25);
			}
		}

		void DestroyConsole() {}

		void SetConsoleShown(bool s) {
			console_shown = s;
		}

	}
}

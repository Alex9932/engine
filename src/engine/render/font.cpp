/*
 * font.cpp
 *
 *  Created on: Feb 17, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/render/font.h>
#include <engine/core/filesystem.h>
#include <engine/core/logger.h>
#include <engine/core/utf8.h>
#include <engine/core/allocator.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Engine {
	namespace Render {

		static FT_Library ft = NULL;
		static Uint32 counter = 0;
		static UTF8Decoder* utf_decoder;

		static STDAllocator* allocator;

		Font::Font(String file, Uint32 scale) {
			if(ft == NULL || counter == 0) {
				rgLogInfo(RG_LOG_DEBUG, "Init library");
//				rgLogInfo(RG_LOG_RENDER, "Init library");
				if (FT_Init_FreeType(&ft)) {
					rgLogError(RG_LOG_RENDER, "Could not initialize freetype library!");
					return;
				}

				allocator = new STDAllocator("FT_ALLOCATOR");
				utf_decoder = Engine::GetUTF8Decoder();
			}

			this->scale = scale;
			FT_Face face;

			Engine::Filesystem::Resource* f_res = Engine::Filesystem::GetResource(file);

			rgLogInfo(RG_LOG_RENDER, "Loading font: %s", file);
			//if (FT_New_Face(ft, file, 0, &face)) {
			if (FT_New_Memory_Face(ft, (const FT_Byte*)f_res->data, f_res->length, 0, &face)) {
				SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Could not open font!");
			}

			if(face->charmap->encoding != 0x756e6963) {
				RG_ASSERT_MSG(NULL, "Font encoding must be a unicode!");
			}
//			rgLogInfo(RG_LOG_RENDER, "Font encoding: 0x%x", );

			FT_Set_Pixel_Sizes(face, 0, this->scale);
//			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// TODO rewrite this (use texture)

			glGenTextures(1, &this->font_atlas);
			glBindTexture(GL_TEXTURE_2D, this->font_atlas);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->scale * F_ATLAS_WIDTH, this->scale * F_ATLAS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			for (wchar_t i = 0; i < F_ATLAS_WIDTH * F_ATLAS_HEIGHT; i++) {
				if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
					SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to load Glyph!");
				}

				this->chars[i].advance_x = face->glyph->advance.x;
				this->chars[i].advance_y = face->glyph->advance.y;
				this->chars[i].bearing_x = face->glyph->bitmap_left;
				this->chars[i].bearing_y = face->glyph->bitmap_top;
				this->chars[i].size_x = face->glyph->bitmap.width;
				this->chars[i].size_y = face->glyph->bitmap.rows;

				int xoffset = i % F_ATLAS_WIDTH;
				int yoffset = i / F_ATLAS_WIDTH;

				FT_Bitmap* bitmap = &face->glyph->bitmap;
				Uint8* new_buffer = (Uint8*)allocator->Allocate(bitmap->width*bitmap->rows * 4);
				for (Uint32 j = 0; j < bitmap->width*bitmap->rows; ++j) {
					new_buffer[j*4 + 0] = bitmap->buffer[j];
					new_buffer[j*4 + 1] = bitmap->buffer[j];
					new_buffer[j*4 + 2] = bitmap->buffer[j];
					new_buffer[j*4 + 3] = bitmap->buffer[j];
				}

				glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset * this->scale, yoffset * this->scale,
					face->glyph->bitmap.width, face->glyph->bitmap.rows,
					GL_RGBA, GL_UNSIGNED_BYTE, new_buffer);

				allocator->Deallocate(new_buffer);
			}
			FT_Done_Face(face);

			Engine::Filesystem::FreeResource(f_res);
			counter++;
			rgLogInfo(RG_LOG_DEBUG, "Counter: %d", counter);
		}

		Font::~Font() {
			glDeleteTextures(1, &this->font_atlas);
			counter--;
			rgLogInfo(RG_LOG_DEBUG, "~Counter: %d", counter);

			if(counter == 0) {
				FT_Done_FreeType(ft);
				rgLogInfo(RG_LOG_DEBUG, "~Destroy library");
			}
		}

		GLuint Font::GetAtlas() {
			return this->font_atlas;
		}

		Uint32 Font::GetScale() {
			return this->scale;
		}

		Glyph* Font::GetGlyphs() {
			return this->chars;
		}



		float Font::GetRawStringLength(const Uint16* str, float scale) {
			return GetRawStringLength(str, scale, -1);
		}

		float Font::GetStringLength(String str, float scale) {
			return GetStringLength(str, scale, -1);
		}

		float Font::GetRawStringLength(const Uint16* str, float scale, Uint32 len) {
			float x = 0;
			Uint16 c = 0;
			for(Uint32 i = 0; ((c = str[i]) != 0 && i < len); i++) {
				x += (chars[c].advance_x >> 6) * scale;
			}
			return x;
		}

		float Font::GetStringLength(String str, float scale, Uint32 len) {
			utf_decoder->DecodeString(str);
			Uint16* result = utf_decoder->GetResult();
			return GetRawStringLength(result, scale, len);
		}

	}
}

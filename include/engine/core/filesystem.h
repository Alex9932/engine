/*
 * filesystem.h
 *
 *  Created on: Feb 13, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_FILESYSTEM_H_
#define ENGINE_CORE_FILESYSTEM_H_

#include <engine/engine.h>
#include <engine/core/math/math.h>
#include <SDL2/SDL.h>
#include <cstdio>

#define RG_FREAD(ptr, size, count, handle)  if(fread(ptr, size, count, handle) != count) { \
												rgLogError(RG_LOG_SYSTEM, "IO Error!");    \
											}

namespace Engine {
	namespace Filesystem {

		typedef struct rg_fs_header {
			char magic[4];
			Uint32 files;
		} rg_fs_header_t;

		typedef struct rg_fs_file {
			char name[256];
			Uint32 offset;
			Uint32 length;
		} rg_fs_file_t;

		typedef struct rg_filesystem {
			FILE* file_stream;
			rg_fs_header header;
			rg_fs_file* files;
		} rg_filesystem_t;

		typedef struct Resource {
			Uint32 length;
			void* data;
		} Resource_t;

		typedef struct ResourceStream {
			FILE* handle;
			rg_filesystem* fs; // If !!NOT NULL!! - Reading from RFS package
			Uint32 file_length;
			Uint32 file_offset; // Be 0 if 'fs' field == NULL
			Uint32 offset;
		} ResourceStream_t;

		RG_DECLSPEC void Initialize();
		RG_DECLSPEC void Destroy();
		RG_DECLSPEC Sint32 Mount(String file);
		RG_DECLSPEC void Umount(Sint32 fs_uid);
		RG_DECLSPEC Resource* GetResource(String file);
		RG_DECLSPEC void FreeResource(Resource* res);

		RG_DECLSPEC ResourceStream* OpenResourceStream(String file);
		RG_DECLSPEC void CloseResourceStream(ResourceStream* res);
		RG_DECLSPEC size_t ReadResourceStream(void* ptr, size_t length, ResourceStream* res);
		RG_DECLSPEC bool EofResourceStream(ResourceStream* res);

		class RG_DECLSPEC FSReader {
			private:
				ResourceStream* stream = NULL;
				size_t readed_blocks = 0;

			public:
				FSReader(String file);
				FSReader(ResourceStream* stream);
				virtual ~FSReader();
				virtual void Read(void* ptr, size_t len);
				virtual size_t GetOffset();

				RG_FORCE_INLINE Uint8 ReadU8() {
					Uint8 tmp;
					Read(&tmp, sizeof(Uint8));
					return tmp;
				}
				RG_FORCE_INLINE Sint8 ReadS8() {
					Sint8 tmp;
					Read(&tmp, sizeof(Sint8));
					return tmp;
				}
				RG_FORCE_INLINE Uint16 ReadU16() {
					Uint16 tmp;
					Read(&tmp, sizeof(Uint16));
					return tmp;
				}
				RG_FORCE_INLINE Uint16 ReadS16() {
					Sint16 tmp;
					Read(&tmp, sizeof(Sint16));
					return tmp;
				}
				RG_FORCE_INLINE Uint32 ReadU32() {
					Uint32 tmp;
					Read(&tmp, sizeof(Uint32));
					return tmp;
				}
				RG_FORCE_INLINE Uint32 ReadS32() {
					Sint32 tmp;
					Read(&tmp, sizeof(Sint32));
					return tmp;
				}
				RG_FORCE_INLINE Uint64 ReadU64() {
					Uint64 tmp;
					Read(&tmp, sizeof(Uint64));
					return tmp;
				}
				RG_FORCE_INLINE Sint64 ReadS64() {
					Sint64 tmp;
					Read(&tmp, sizeof(Sint64));
					return tmp;
				}
				RG_FORCE_INLINE float ReadF32() {
					float tmp;
					Read(&tmp, sizeof(float));
					return tmp;
				}
				RG_FORCE_INLINE double ReadF64() {
					double tmp;
					Read(&tmp, sizeof(double));
					return tmp;
				}
				RG_FORCE_INLINE void Read2F32(Math::vec2& v) { Read(&v, sizeof(Math::vec2)); }
				RG_FORCE_INLINE void Read3F32(Math::vec3& v) { Read(&v, sizeof(Math::vec3)); }
				RG_FORCE_INLINE void Read4F32(Math::vec4& v) { Read(&v, sizeof(Math::vec4)); }
				RG_FORCE_INLINE ResourceStream* GetStream() { return stream; }
				RG_FORCE_INLINE size_t GetReadedBlocks() { return readed_blocks; }
				RG_FORCE_INLINE bool EndOfStream() { return EofResourceStream(stream); }
		};

	}
}


#endif /* ENGINE_CORE_FILESYSTEM_H_ */

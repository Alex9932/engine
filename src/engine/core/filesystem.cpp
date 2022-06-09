/*
 * filesystem.cpp
 *
 *  Created on: Feb 13, 2022
 *      Author: alex9932
 */

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT

#include <engine/core/filesystem.h>
#include <engine/core/strings.h>
#include <engine/core/logger.h>
#include <map>

// TODO: Create and use I/O Allocator

namespace Engine {
	namespace Filesystem {

		struct _file_fs {
			rg_filesystem* filesystem;
			rg_fs_file file;
		};

		static std::map<Uint32, rg_filesystem*> filesystems;
		static Sint32 fs_id;
		static char string_buffer[128];

		static void _fs_find(_file_fs* ffs, String file) {
			ffs->filesystem = NULL;
			for (std::map<Uint32, rg_filesystem*>::iterator it = filesystems.begin(); it != filesystems.end(); ++it) {
				rg_filesystem* filesystem = it->second;
				for (Uint32 i = 0; i < filesystem->header.files; ++i) {
					rg_fs_file f = filesystem->files[i];
					if(Engine::Helper::streql(f.name, file)) {
						ffs->file = f;
						ffs->filesystem = filesystem;
						return;
					}
				}
			}
		}

		static Resource* GetResourceInFS(String file) {
			_file_fs ffs;
			_fs_find(&ffs, file);
			if(ffs.filesystem == NULL) { return NULL;}
			Resource* res = (Resource*)malloc(sizeof(Resource));
			res->length = ffs.file.length;
			res->data = malloc(ffs.file.length + 1);
			((char*)res->data)[ffs.file.length] = '\0';
			fseek(ffs.filesystem->file_stream, ffs.file.offset, SEEK_SET);
			RG_FREAD(res->data, 1, ffs.file.length, ffs.filesystem->file_stream);
			return res;
		}

		static ResourceStream* GetResourceStreamFS(String file) {
			_file_fs ffs;
			_fs_find(&ffs, file);
			if(ffs.filesystem == NULL) { return NULL; }
			ResourceStream* res = (ResourceStream*)malloc(sizeof(ResourceStream));
			res->file_length = ffs.file.length;
			res->file_offset = ffs.file.offset;
			res->fs = ffs.filesystem;
			res->handle = ffs.filesystem->file_stream;
			res->offset = 0;
			return res;
		}

		void Initialize() {
			filesystems.clear();
			fs_id = -1;
		}

		void Destroy() {
			filesystems.clear();
		}

		Sint32 Mount(String file) {
			FILE* f = fopen(file, "rb");
			if(f) {
				rg_filesystem* fs = (rg_filesystem*)malloc(sizeof(rg_filesystem));
				fs->file_stream = f;
				RG_FREAD(&fs->header, sizeof(rg_fs_header), 1, fs->file_stream);

				if(fs->header.magic[0] != 'r' || fs->header.magic[1] != 'f' || fs->header.magic[2] != 's') {
					SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Invalid header at %s!", file);
					RG_ASSERT(NULL);
				}

				SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Mounting %s, %d files mapped.", file, fs->header.files);
				fs->files = (rg_fs_file*)malloc(sizeof(rg_fs_file) * fs->header.files);
				RG_FREAD(fs->files, sizeof(rg_fs_file), fs->header.files, fs->file_stream);
				fs_id++;
				filesystems[fs_id] = fs;
				return fs_id;
			}
			return -1;
		}

		void Umount(Sint32 fs_uid) {
			if(fs_uid < 0) {
				rgLogError(RG_LOG_SYSTEM, "Unable to unmount %d filesystem!", fs_uid);
				return;
			}
			rg_filesystem* fs = filesystems[fs_uid];
			fclose(fs->file_stream);
			free(fs->files);
			free(fs);
			filesystems[fs_uid] = NULL;
		}

		Resource* GetResource(String file) {
			Resource* res = GetResourceInFS(file);
			if(res != NULL) { return res; }

			FILE* fptr = fopen(file, "rb");
			sprintf(string_buffer, "FILE NOT FOUND => %s", file);
			RG_ASSERT_MSG(fptr, string_buffer);

			res = (Resource*)malloc(sizeof(Resource));
			fseek(fptr, 0, SEEK_END);
			res->length = ftell(fptr);
			rewind(fptr);
			res->data = malloc(res->length + 1);
			((char*)res->data)[res->length] = '\0';
			memset(res->data, ' ', res->length);
			Uint32 readed = fread(res->data, 1, res->length, fptr);
			if(readed != res->length && !feof(fptr)) {
				rgLogError(RG_LOG_SYSTEM, "Error while reading file! Length/Readed: %d, %d", res->length, readed);
			}
			fclose(fptr);
			return res;
		}

		void FreeResource(Resource* res) {
			free(res->data);
			free(res);
		}

		ResourceStream* OpenResourceStream(String file) {
			ResourceStream* stream = GetResourceStreamFS(file);
			if(stream != NULL) { return stream; }

			FILE* fptr = fopen(file, "rb");
			sprintf(string_buffer, "FILE NOT FOUND => %s", file);
			RG_ASSERT_MSG(fptr, string_buffer);

			stream = (ResourceStream*)malloc(sizeof(ResourceStream));
			fseek(fptr, 0, SEEK_END);
			stream->file_length = ftell(fptr);
			rewind(fptr);
			stream->file_offset = 0;
			stream->fs = NULL;
			stream->handle = fptr;
			stream->offset = 0;

			return stream;
		}

		void CloseResourceStream(ResourceStream* stream) {
			if(stream->fs == NULL) { fclose(stream->handle); }
			free(stream);
		}

		size_t ReadResourceStream(void* ptr, size_t length, ResourceStream* stream) {
			Uint32 s_offset = 0;
			if(stream->fs == NULL) { s_offset = stream->offset; }
			else { s_offset = stream->file_offset + stream->offset; }
			fseek(stream->handle, s_offset, SEEK_SET);
			stream->offset += length;
			return fread(ptr, 1, length, stream->handle);
		}

		bool EofResourceStream(ResourceStream* stream) {
			bool eof = stream->offset >= stream->file_length;
			Uint32 s_offset = 0;
			if(stream->fs == NULL) { s_offset = stream->offset; }
			else { s_offset = stream->file_offset + stream->offset; }
			fseek(stream->handle, s_offset, SEEK_SET);
			return feof(stream->handle) || eof;
		}


		// FSReader class

		FSReader::FSReader(String file) { this->stream = OpenResourceStream(file); }
		FSReader::FSReader(ResourceStream* stream) { this->stream = stream; }
		FSReader::~FSReader() { CloseResourceStream(stream); }
		void FSReader::Read(void* ptr, size_t len) {
			readed_blocks = ReadResourceStream(ptr, len, stream);
		}

		size_t FSReader::GetOffset() {
			return stream->offset;
		}

	}
}

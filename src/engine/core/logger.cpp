/*
 * logger.cpp
 *
 *  Created on: Feb 10, 2022
 *      Author: alex9932
 */

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT

#include <engine/core/logger.h>
#include <engine/core/strings.h>
#include <engine/engine.h>
#include <SDL2/SDL.h>
#include <cstdio>

#define LOGGER_BUFFER_LENGTH 2048
#define LOGGER_LINES 20

namespace Engine {
	namespace Logger {
		static FILE* file_stream = NULL;
		static String LOG_PATH = "last.log";
		static char BUFFER[LOGGER_BUFFER_LENGTH];
		static bool LOGGING_ENABLED = true;
		static Uint32 MESSAGES = 0;

		static char LINES_BUFFER[LOGGER_LINES][LOGGER_BUFFER_LENGTH];
		static SDL_LogPriority PRIORITY_BUFFER[LOGGER_LINES];
		static Sint32 buffer_index = LOGGER_LINES - 1;

#ifdef RG_PLATFORM_LINUX
		RG_FORCE_INLINE static String getColor(SDL_LogPriority priority) {
			switch(priority) {
				case SDL_LOG_PRIORITY_VERBOSE:  return "1;34";
				case SDL_LOG_PRIORITY_DEBUG:    return "1;30";
				case SDL_LOG_PRIORITY_INFO:     return "1;32";
				case SDL_LOG_PRIORITY_WARN:     return "1;33";
				case SDL_LOG_PRIORITY_ERROR:    return "1;31";
				case SDL_LOG_PRIORITY_CRITICAL: return "0;41";
				default:                        return "1;34";
			}
		}
#endif

#ifdef RG_PLATFORM_WINDOWS
		RG_FORCE_INLINE static Uint8 getColor(SDL_LogPriority priority) {
			switch(priority) {
				case SDL_LOG_PRIORITY_VERBOSE:  return 1;
				case SDL_LOG_PRIORITY_DEBUG:    return 8;
				case SDL_LOG_PRIORITY_INFO:     return 2;
				case SDL_LOG_PRIORITY_WARN:     return 6;
				case SDL_LOG_PRIORITY_ERROR:    return 4;
				case SDL_LOG_PRIORITY_CRITICAL: return 64;
				default:                        return 1;
			}
		}
#endif

		RG_FORCE_INLINE static void printLine(SDL_LogPriority priority, String cat, String line) {
			String p = Engine::Helper::getPriority(priority);
			MESSAGES++;

			if(buffer_index < 0) {
				buffer_index = 0;
				for (Uint32 i = LOGGER_LINES - 1; i > 0; i--) {
					SDL_memcpy(LINES_BUFFER[i], LINES_BUFFER[i - 1], LOGGER_BUFFER_LENGTH);
					PRIORITY_BUFFER[i] = PRIORITY_BUFFER[i - 1];
				}
			}

			sprintf(LINES_BUFFER[buffer_index], "%s [%s] %s\n", p, cat, line);
			PRIORITY_BUFFER[buffer_index] = priority;

//			printf("%s [%s] %s\n", p, cat, line);
#if defined(RG_PLATFORM_WINDOWS)
			HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(console_handle, getColor(priority));
			OutputDebugStringA(LINES_BUFFER[buffer_index]);
			Uint64 length = strlen(LINES_BUFFER[buffer_index]);
			LPDWORD number_written = 0;
			//WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
			WriteConsoleA(console_handle, LINES_BUFFER[buffer_index], (DWORD)length, number_written, 0);
#elif defined(RG_PLATFORM_LINUX)
			printf("\033[%sm%s [%s] %s\033[0m\n", getColor(priority), p, cat, line);
#else
			printf("%s [%s] %s\n", p, cat, line);
#endif

			buffer_index--;

			if(file_stream) {
				fprintf(file_stream, "%s [%s] %s\n", p, cat, line);
			}
		}

		static Uint32 findEnd(Uint32 start, String str) {
			for (Uint32 i = start; i < strlen(str); ++i) {
				if(str[i] == '\n') {
					return i;
				}
			}
			return strlen(str);
		}

		static void logger(void* userdata, int category, SDL_LogPriority priority, String message) {
			if(!LOGGING_ENABLED) {
				return;
			}

			if(category == RG_LOG_DEBUG && !Engine::IsDebug()) {
				return;
			}

			String cat = Engine::Helper::getCategory(category);

			Uint32 start = 0;
			Uint32 end = 0;
			Uint32 len = strlen(message);

			do {
				start = end;
				if(start != 0) start++;
				end = findEnd(start, message);
				SDL_memset(BUFFER, 0, LOGGER_BUFFER_LENGTH);
				SDL_memcpy(BUFFER, &message[start], end - start);
				printLine(priority, cat, BUFFER);
//				printf("%d %d %d\n", start, end, len);
			} while(end != len);
		}

		void Initialize() {
			file_stream = fopen(LOG_PATH, "w");
			SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
			SDL_LogSetOutputFunction(logger, NULL);
		}

		void Destroy() {
			fclose(file_stream);
		}

		String GetLine(Uint32 i) {
			return LINES_BUFFER[i];
		}

		SDL_LogPriority GetLinePriority(Uint32 i) {
			return PRIORITY_BUFFER[i];
		}

		Uint32 GetLines() {
			return LOGGER_LINES;
		}

		Uint32 GetMessages() {
			return MESSAGES;
		}
	}
}

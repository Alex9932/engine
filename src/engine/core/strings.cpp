/*
 * strings.cpp
 *
 *  Created on: Feb 10, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/strings.h>
#include <engine/core/logger.h>
#include <cstring>

namespace Engine {
	namespace Helper {
		String getCategory(int category) {
			switch(category) {
				case RG_LOG_GAME:   return " game ";
				case RG_LOG_SYSTEM: return "system";
				case RG_LOG_RENDER: return "render";
				case RG_LOG_ERROR:  return "error";
				case RG_LOG_ASSERT: return "assert";
				case RG_LOG_AUDIO:  return "audio";
				case RG_LOG_DEBUG:  return "DEBUG";
				default:            return "app";
			}
		}

		String getPriority(SDL_LogPriority priority) {
			switch(priority) {
				case SDL_LOG_PRIORITY_VERBOSE:  return "Verb";
				case SDL_LOG_PRIORITY_DEBUG:    return "DBG ";
				case SDL_LOG_PRIORITY_INFO:     return "Info";
				case SDL_LOG_PRIORITY_WARN:     return "Warn";
				case SDL_LOG_PRIORITY_ERROR:    return "Err ";
				case SDL_LOG_PRIORITY_CRITICAL: return "Crit";
				default:                        return "Msg ";
			}
		}

		String getCpuName(int cores) {
			switch(cores) {
				case 0:  return "WTF?";
				case 1:  return "Single-core";
				case 2:  return "Dual-core";
				case 3:  return "Triple-core";
				case 4:  return "Quad-core";
				case 5:  return "5 cores";
				case 6:  return "6 cores";
				case 7:  return "7 cores";
				case 8:  return "Octal-core";
				case 9:  return "9 cores";
				case 10: return "10 cores";
				case 11: return "11 cores";
				case 12: return "12 cores";
				case 13: return "13 cores";
				case 14: return "14 cores";
				case 15: return "15 cores";
				case 16: return "16 cores";
				case 17: return "17 cores";
				case 18: return "18 cores";
				case 19: return "19 cores";
				case 20: return "20 cores";
				default: return "Unknown";
			}
		}

		bool streql(String left, String right) {
			size_t lenpre = SDL_strlen(left);
			size_t lenstr = SDL_strlen(right);
			return ( (lenstr == lenpre) && (SDL_memcmp(left, right, lenpre) == 0) ) ? true : false;
		}

		bool strstw(String left, String right) {
			size_t lenstr = SDL_strlen(right);
			for (Uint32 i = 0; i < lenstr; ++i) {
				if(left[i] != right[i]) {
					return false;
				}
			}
			return true;
		}
	}
}

/*
 * utf8.h
 *
 *  Created on: Apr 12, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_UTF8_H_
#define ENGINE_CORE_UTF8_H_

#include <engine/engine.h>

#define RG_UTF8_BUFFER_LENGTH 2048
#define RG_UTF8_1BYTE 0b10000000
#define RG_UTF8_2BYTE 0b11000000
#define RG_UTF8_3BYTE 0b11100000
#define RG_UTF8_4BYTE 0b11110000

namespace Engine {

	class RG_DECLSPEC UTF8Decoder {
		private:
			Uint16 buffer[RG_UTF8_BUFFER_LENGTH];

		public:
			UTF8Decoder();
			virtual ~UTF8Decoder();
			void DecodeString(String str);
			Uint32 CharAt(String str, Uint32 char_pos);
			Uint16* GetResult();
	};


	class RG_DECLSPEC UTF8Encoder {
		private:
			char buffer[RG_UTF8_BUFFER_LENGTH];

		public:
			UTF8Encoder();
			virtual ~UTF8Encoder();
			void EncodeString(const Uint16* str);
			String GetResult();
	};


}


#endif /* ENGINE_CORE_UTF8_H_ */

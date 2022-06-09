/*
 * utf8.cpp
 *
 *  Created on: Apr 12, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/utf8.h>

// TODO: Create decode function
//#define RG_UTF8_1BYTE_CHAR 0x0001
//#define RG_UTF8_2BYTE_CHAR 0x0002
//#define RG_UTF8_3BYTE_CHAR 0x0004
//#define RG_UTF8_4BYTE_CHAR 0x0008

namespace Engine {

	UTF8Decoder::UTF8Decoder() {}
	UTF8Decoder::~UTF8Decoder() {}

	void UTF8Decoder::DecodeString(String str) {
		SDL_memset(buffer, 0, sizeof(Uint16) * RG_UTF8_BUFFER_LENGTH);

		Uint32 k = 0;
//		printf("Str:");
		Uint32 len = strlen(str);
		for (Uint32 i = 0; i < len; ++i) {
			Uint16 code = 0;
			Uint8 c = str[i];
			if(c >> 7 == 1) { // Multi-byte char
				if((c & RG_UTF8_2BYTE) == RG_UTF8_2BYTE) { // 2-byte char
					Uint16 c1 = (c ^ RG_UTF8_2BYTE) << 6;
					Uint8 c2 = str[i+1] ^ RG_UTF8_1BYTE;
					code = c1 + c2;
					i++;
				}
				if((c & RG_UTF8_3BYTE) == RG_UTF8_3BYTE) { // 3-byte char
					Uint16 c1 = (c ^ RG_UTF8_3BYTE) << 12;
					Uint16 c2 = (str[i+1] ^ RG_UTF8_1BYTE) << 6;
					Uint8 c3 = str[i+2] ^ RG_UTF8_1BYTE;
					code = c3 + c2 + c1;
					i += 2;
				}
				if((c & RG_UTF8_4BYTE) == RG_UTF8_4BYTE) { // 4-byte char
					Uint16 c1 = (c ^ RG_UTF8_4BYTE) << 18;
					Uint16 c2 = (str[i+1] ^ RG_UTF8_1BYTE) << 12;
					Uint16 c3 = (str[i+2] ^ RG_UTF8_1BYTE) << 6;
					Uint8 c4 = str[i+3] ^ RG_UTF8_1BYTE;
					code = c4 + c3 + c2 + c1;
					i += 3;
				}
			} else { // Single-byte char
				code = c;
			}

			buffer[k] = code;
//			printf(" %x", buffer[k]);
			k++;
		}
//		printf("\n");
	}

	Uint32 UTF8Decoder::CharAt(String str, Uint32 char_pos) {
		Uint32 k = 0;
		Uint32 cp = 0;
		Uint32 len = strlen(str);
		for (Uint32 i = 0; i < len; ++i) {
			Uint8 c = str[i];
			if(c >> 7 == 1) { // Multi-byte char
				if((c & RG_UTF8_2BYTE) == RG_UTF8_2BYTE) { // 2-byte char
					k += 2;
					i += 1;
				}
				if((c & RG_UTF8_3BYTE) == RG_UTF8_3BYTE) { // 3-byte char
					k += 3;
					i += 2;
				}
				if((c & RG_UTF8_4BYTE) == RG_UTF8_4BYTE) { // 4-byte char
					k += 4;
					i += 3;
				}
			} else { // Single-byte char
				k++;
			}

			cp++;

			if(cp > char_pos) {
				break;
			}
		}

		return k;
	}

	Uint16* UTF8Decoder::GetResult() {
		return buffer;
	}


	UTF8Encoder::UTF8Encoder() {}
	UTF8Encoder::~UTF8Encoder() {}

	static Uint16 Encode(char* str, Uint16 c) {
		Uint32 c_length = 0;

		if(c >= 0 && c < 0x80) { c_length = 1; }
		else if(c >= 0x80 && c < 0x800) { c_length = 2; }
		else if(c >= 0x800 && c < 0x10000) { c_length = 3; }
		else if(c >= 0x10000 && c < 0x110000) { c_length = 4; }

		switch (c_length) {
			case 1: {
				str[0] = (char)c;
				break;
			}
			case 2: {
				Uint16 b1 = c & 0x003F;
				Uint16 b2 = (c >> 6) & 0x003F;
				str[1] = b1 ^ RG_UTF8_1BYTE;
				str[0] = b2 ^ RG_UTF8_2BYTE;
				break;
			}
			case 3: {
				Uint16 b1 = c & 0x003F;
				Uint16 b2 = (c >> 6) & 0x003F;
				Uint16 b3 = (c >> 12) & 0x003F;
				str[2] = b1 ^ RG_UTF8_1BYTE;
				str[1] = b2 ^ RG_UTF8_1BYTE;
				str[0] = b3 ^ RG_UTF8_3BYTE;
				break;
			}
			case 4: {
				Uint16 b1 = c & 0x003F;
				Uint16 b2 = (c >> 6) & 0x003F;
				Uint16 b3 = (c >> 12) & 0x003F;
				Uint16 b4 = (c >> 18) & 0x003F;
				str[3] = b1 ^ RG_UTF8_1BYTE;
				str[2] = b2 ^ RG_UTF8_1BYTE;
				str[1] = b3 ^ RG_UTF8_1BYTE;
				str[0] = b4 ^ RG_UTF8_4BYTE;
				break;
			}
			default:
				break;
		}

		return c_length;
	}

	void UTF8Encoder::EncodeString(const Uint16* str) {
		SDL_memset(buffer, 0, sizeof(char) * RG_UTF8_BUFFER_LENGTH);

		Uint32 i = 0;
		Uint16 ptr = 0;
		Uint16 c = str[i];
		while(c != 0) {
			ptr += Encode(&buffer[ptr], c);
			i++;
			c = str[i];
		}

	}

	String UTF8Encoder::GetResult() {
		return buffer;
	}

}

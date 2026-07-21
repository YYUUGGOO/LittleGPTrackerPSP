#include "FontConfig.h"

unsigned char font[8*8*128]= {
	#include "Resources/font.h"
};

void FontConfig() {
	// The PSP-only build embeds the original compact font at compile time.
}

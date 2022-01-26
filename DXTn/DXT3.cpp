#include "DXTn.h"

/*
	Modified versions of VTFLib's DXTn decompression functions
	All credit goes to https://github.com/NeilJed/VTFLib
*/

void DXTn::DecompressDXT3(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height)
{
	uint32_t               x, y, i, j, k, Select;
	uint8_t*               Temp;
	Colour565*             color_0, * color_1;
	Colour8888             colours[4], * col;
	uint32_t               bitmask, Offset;
	uint16_t               word;
	DXTAlphaBlockExplicit* alpha;

	uint8_t nBpp = 4;                    // bytes per pixel (4 channels (RGBA))
	uint8_t nBpc = 1;                    // bytes per channel (1 byte per channel)
	uint32_t iBps = nBpp * nBpc * width; // bytes per scanline

	Temp = src;

	for (y = 0; y < height; y += 4) {
		for (x = 0; x < width; x += 4) {
			alpha = (DXTAlphaBlockExplicit*)Temp;
			Temp += 8;
			color_0 = ((Colour565*)Temp);
			color_1 = ((Colour565*)(Temp + 2));
			bitmask = ((uint32_t*)Temp)[1];
			Temp += 8;

			colours[0].r = color_0->nRed << 3;
			colours[0].g = color_0->nGreen << 2;
			colours[0].b = color_0->nBlue << 3;
			colours[0].a = 0xFF;

			colours[1].r = color_1->nRed << 3;
			colours[1].g = color_1->nGreen << 2;
			colours[1].b = color_1->nBlue << 3;
			colours[1].a = 0xFF;

			// Four-color block: derive the other two colors.    
			// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
			// These 2-bit codes correspond to the 2-bit fields 
			// stored in the 64-bit block.
			colours[2].b = (2 * colours[0].b + colours[1].b + 1) / 3;
			colours[2].g = (2 * colours[0].g + colours[1].g + 1) / 3;
			colours[2].r = (2 * colours[0].r + colours[1].r + 1) / 3;
			colours[2].a = 0xFF;

			colours[3].b = (colours[0].b + 2 * colours[1].b + 1) / 3;
			colours[3].g = (colours[0].g + 2 * colours[1].g + 1) / 3;
			colours[3].r = (colours[0].r + 2 * colours[1].r + 1) / 3;
			colours[3].a = 0xFF;

			k = 0;
			for (j = 0; j < 4; j++) {
				for (i = 0; i < 4; i++, k++) {
					Select = (bitmask & (0x03 << k * 2)) >> k * 2;
					col = &colours[Select];

					if (((x + i) < width) && ((y + j) < height)) {
						Offset = (y + j) * iBps + (x + i) * nBpp;
						dst[Offset + 0] = col->r;
						dst[Offset + 1] = col->g;
						dst[Offset + 2] = col->b;
					}
				}
			}

			for (j = 0; j < 4; j++) {
				word = alpha->row[j];
				for (i = 0; i < 4; i++) {
					if (((x + i) < width) && ((y + j) < height)) {
						Offset = (y + j) * iBps + (x + i) * nBpp + 3;
						dst[Offset] = word & 0x0F;
						dst[Offset] = dst[Offset] | (dst[Offset] << 4);
					}

					word >>= 4;
				}
			}
		}
	}
}

#include "DXTn.h"

/*
	Modified versions of VTFLib's DXTn decompression functions
	All credit goes to https://github.com/NeilJed/VTFLib
*/

void DXTn::DecompressDXT5(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height)
{
	uint32_t         x, y, i, j, k, Select;
	uint8_t*         Temp;
	Colour565*       color_0, * color_1;
	Colour8888       colours[4], * col;
	uint32_t         bitmask, Offset;
	uint8_t          alphas[8];
	uint8_t*         alphamask;
	uint32_t         bits;

	uint8_t nBpp = 4;                    // bytes per pixel (4 channels (RGBA))
	uint8_t nBpc = 1;                    // bytes per channel (1 byte per channel)
	uint32_t iBps = nBpp * nBpc * width; // bytes per scanline

	Temp = src;

	for (y = 0; y < height; y += 4) {
		for (x = 0; x < width; x += 4) {
			//if (y >= uiHeight || x >= uiWidth)
			//		break;

			alphas[0] = Temp[0];
			alphas[1] = Temp[1];
			alphamask = Temp + 2;
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

					// only put pixels out < width or height
					if (((x + i) < width) && ((y + j) < height)) {
						Offset = (y + j) * iBps + (x + i) * nBpp;
						dst[Offset + 0] = col->r;
						dst[Offset + 1] = col->g;
						dst[Offset + 2] = col->b;
					}
				}
			}

			// 8-alpha or 6-alpha block?    
			if (alphas[0] > alphas[1]) {
				// 8-alpha block:  derive the other six alphas.    
				// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
				alphas[2] = (6 * alphas[0] + 1 * alphas[1] + 3) / 7;	// bit code 010
				alphas[3] = (5 * alphas[0] + 2 * alphas[1] + 3) / 7;	// bit code 011
				alphas[4] = (4 * alphas[0] + 3 * alphas[1] + 3) / 7;	// bit code 100
				alphas[5] = (3 * alphas[0] + 4 * alphas[1] + 3) / 7;	// bit code 101
				alphas[6] = (2 * alphas[0] + 5 * alphas[1] + 3) / 7;	// bit code 110
				alphas[7] = (1 * alphas[0] + 6 * alphas[1] + 3) / 7;	// bit code 111  
			} else {
				// 6-alpha block.    
				// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
				alphas[2] = (4 * alphas[0] + 1 * alphas[1] + 2) / 5;	// Bit code 010
				alphas[3] = (3 * alphas[0] + 2 * alphas[1] + 2) / 5;	// Bit code 011
				alphas[4] = (2 * alphas[0] + 3 * alphas[1] + 2) / 5;	// Bit code 100
				alphas[5] = (1 * alphas[0] + 4 * alphas[1] + 2) / 5;	// Bit code 101
				alphas[6] = 0x00;										// Bit code 110
				alphas[7] = 0xFF;										// Bit code 111
			}

			// Note: Have to separate the next two loops,
			//	it operates on a 6-byte system.

			// First three bytes
			bits = *((int*)alphamask);
			for (j = 0; j < 2; j++) {
				for (i = 0; i < 4; i++) {
					// only put pixels out < width or height
					if (((x + i) < width) && ((y + j) < height)) {
						Offset = (y + j) * iBps + (x + i) * nBpp + 3;
						dst[Offset] = alphas[bits & 0x07];
					}
					bits >>= 3;
				}
			}

			// Last three bytes
			bits = *((int*)&alphamask[3]);
			for (j = 2; j < 4; j++) {
				for (i = 0; i < 4; i++) {
					// only put pixels out < width or height
					if (((x + i) < width) && ((y + j) < height)) {
						Offset = (y + j) * iBps + (x + i) * nBpp + 3;
						dst[Offset] = alphas[bits & 0x07];
					}
					bits >>= 3;
				}
			}
		}
	}
}

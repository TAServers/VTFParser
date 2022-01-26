#pragma once

#include <cstdint>

/// <summary>
/// DXT compression format functions and structs taken and modified from VTFLib
/// </summary>
namespace DXTn
{
	// RGBA Pixel type
	struct Colour8888
	{
		uint8_t r;		// change the order of names to change the 
		uint8_t g;		// order of the output ARGB or BGRA, etc...
		uint8_t b;		// Last one is MSB, 1st is LSB.
		uint8_t a;
	};

	// RGB Pixel type
	struct Colour888
	{
		uint8_t r;		// change the order of names to change the 
		uint8_t g;		// order of the output ARGB or BGRA, etc...
		uint8_t b;		// Last one is MSB, 1st is LSB.
	};

	// BGR565 Pixel type
	struct Colour565
	{
		uint32_t nBlue : 5;		// order of names changes
		uint32_t nGreen : 6;		// byte order of output to 32 bit
		uint32_t nRed : 5;
	};

	// DXTn Colour block type
	struct DXTColBlock
	{
		int16_t col0;
		int16_t col1;
		int8_t	row[4];		// no bit fields - use bytes
	};

	// DXTn Alpha block types
	struct DXTAlphaBlockExplicit
	{
		int16_t row[4];
	};

	struct DXTAlphaBlock3BitLinear
	{
		int8_t alpha0;
		int8_t alpha1;
		int8_t stuff[6];
	};

	void DecompressDXT1(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
	void DecompressDXT3(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
	void DecompressDXT5(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
}

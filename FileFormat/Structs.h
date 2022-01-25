#pragma once

#include "Enums.h"
#include <cstdint>

#define VTF_MAX_RESOURCES 32

#pragma pack(1)

/// <summary>
/// Base header struct
/// </summary>
struct VTFHeaderBase
{
	uint8_t      signature[4];       // File signature ("VTF\0"). (or as little-endian integer, 0x00465456)
	uint32_t     version[2];         // version[0].version[1] (currently 7.2).
	uint32_t     headerSize;         // Size of the header struct  (16 byte aligned; currently 80 bytes) + size of the resources dictionary (7.3+).
};

/// <summary>
/// Aligned full header (only for alignment reasons, do not use outside of VTFHeader declaration)
/// </summary>
__declspec(align(16)) struct VTFHeaderFullAligned : public VTFHeaderBase
{
	uint16_t     width;              // Width of the largest mipmap in pixels. Must be a power of 2.
	uint16_t     height;             // Height of the largest mipmap in pixels. Must be a power of 2.
	uint32_t     flags;              // VTF flags.
	uint16_t     frames;             // Number of frames, if animated (1 for no animation).
	uint16_t     firstFrame;         // First frame in animation (0 based). Can be -1 in environment maps older than 7.5, meaning there are 7 faces, not 6.
	uint8_t      padding0[4];        // reflectivity padding (16 byte alignment).
	float        reflectivity[3];    // reflectivity vector.
	uint8_t      padding1[4];        // reflectivity padding (8 byte packing).
	float        bumpmapScale;       // Bumpmap scale.
	IMAGE_FORMAT highResImageFormat; // High resolution image format.
	uint8_t      mipmapCount;        // Number of mipmaps.
	IMAGE_FORMAT lowResImageFormat;  // Low resolution image format (always DXT1).
	uint8_t      lowResImageWidth;   // Low resolution image width.
	uint8_t      lowResImageHeight;  // Low resolution image height.

	uint16_t     depth;              // Depth of the largest mipmap in pixels. Must be a power of 2. Is 1 for a 2D texture.

	uint8_t      padding2[3];        // depth padding (4 byte alignment).
	uint32_t     numResources;       // Number of resources this vtf has. The max appears to be 32.
};

/// <summary>
/// Information regarding a particular resource
/// </summary>
struct ResourceEntryInfo
{
	uint8_t  tag[3]; // A three-byte "tag" that identifies what this resource is.
	uint8_t  flags;  // Resource entry flags. The only known flag is 0x2, which indicates that no data chunk corresponds to this resource.
	uint32_t data;   // The resource's data or the offset to its data in the file
};

/// <summary>
/// Header struct for actual use
/// </summary>
struct VTFHeader : public VTFHeaderFullAligned
{
	uint8_t padding3[8];
	ResourceEntryInfo resourceInfos[VTF_MAX_RESOURCES];
};

#pragma pack()

struct ImageFormatInfo
{
	const char* name;
	uint32_t bitsPerPixel;
	uint32_t bytesPerPixel;
	uint32_t redBitsPerPixel;
	uint32_t greenBitsPerPixel;
	uint32_t blueBitsPerPixel;
	uint32_t alphaBitsPerPixel;
	bool isCompressed;
	bool isSupported;
};

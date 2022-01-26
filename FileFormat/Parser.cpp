#include "Parser.h"

#include <cstdlib>
#include <cstring>

static ImageFormatInfo VTFImageFormatInfo[] = {
	{ "RGBA8888",           32,  4,  8,  8,  8,  8, false,  true }, // IMAGE_FORMAT_RGBA8888,
	{ "ABGR8888",           32,  4,  8,  8,  8,  8, false,  true }, // IMAGE_FORMAT_ABGR8888, 
	{ "RGB888",             24,  3,  8,  8,  8,  0, false,  true }, // IMAGE_FORMAT_RGB888,
	{ "BGR888",             24,  3,  8,  8,  8,  0, false,  true }, // IMAGE_FORMAT_BGR888,
	{ "RGB565",             16,  2,  5,  6,  5,  0, false,  true }, // IMAGE_FORMAT_RGB565, 
	{ "I8",                  8,  1,  0,  0,  0,  0, false,  true }, // IMAGE_FORMAT_I8,
	{ "IA88",               16,  2,  0,  0,  0,  8, false,  true }, // IMAGE_FORMAT_IA88
	{ "P8",                  8,  1,  0,  0,  0,  0, false, false }, // IMAGE_FORMAT_P8
	{ "A8",                  8,  1,  0,  0,  0,  8, false,  true }, // IMAGE_FORMAT_A8
	{ "RGB888 Bluescreen",  24,  3,  8,  8,  8,  0, false,  true }, // IMAGE_FORMAT_RGB888_BLUESCREEN
	{ "BGR888 Bluescreen",  24,  3,  8,  8,  8,  0, false,  true }, // IMAGE_FORMAT_BGR888_BLUESCREEN
	{ "ARGB8888",           32,  4,  8,  8,  8,  8, false,  true }, // IMAGE_FORMAT_ARGB8888
	{ "BGRA8888",           32,  4,  8,  8,  8,  8, false,  true }, // IMAGE_FORMAT_BGRA8888
	{ "DXT1",                4,  0,  0,  0,  0,  0,  true,  true }, // IMAGE_FORMAT_DXT1
	{ "DXT3",                8,  0,  0,  0,  0,  8,  true,  true }, // IMAGE_FORMAT_DXT3
	{ "DXT5",                8,  0,  0,  0,  0,  8,  true,  true }, // IMAGE_FORMAT_DXT5
	{ "BGRX8888",           32,  4,  8,  8,  8,  0, false,  true }, // IMAGE_FORMAT_BGRX8888
	{ "BGR565",             16,  2,  5,  6,  5,  0, false,  true }, // IMAGE_FORMAT_BGR565
	{ "BGRX5551",           16,  2,  5,  5,  5,  0, false,  true }, // IMAGE_FORMAT_BGRX5551
	{ "BGRA4444",           16,  2,  4,  4,  4,  4, false,  true }, // IMAGE_FORMAT_BGRA4444
	{ "DXT1 One Bit Alpha",  4,  0,  0,  0,  0,  1,  true,  true }, // IMAGE_FORMAT_DXT1_ONEBITALPHA
	{ "BGRA5551",           16,  2,  5,  5,  5,  1, false,  true }, // IMAGE_FORMAT_BGRA5551
	{ "UV88",               16,  2,  8,  8,  0,  0, false,  true }, // IMAGE_FORMAT_UV88
	{ "UVWQ8888",           32,  4,  8,  8,  8,  8, false,  true }, // IMAGE_FORMAT_UVWQ8899
	{ "RGBA16161616F",      64,  8, 16, 16, 16, 16, false,  true }, // IMAGE_FORMAT_RGBA16161616F
	{ "RGBA16161616",       64,  8, 16, 16, 16, 16, false,  true }, // IMAGE_FORMAT_RGBA16161616
	{ "UVLX8888",           32,  4,  8,  8,  8,  8, false,  true }  // IMAGE_FORMAT_UVLX8888
};

ImageFormatInfo VTFParser::GetImageFormatInfo(IMAGE_FORMAT format)
{
	if (format <= IMAGE_FORMAT::NONE || format > IMAGE_FORMAT::UVLX8888)
		return ImageFormatInfo{ "Invalid Format", 0, 0, 0, 0, 0, 0, false, false };

	return VTFImageFormatInfo[static_cast<uint32_t>(format)];
}

uint32_t VTFParser::CalcImageSize(uint16_t width, uint16_t height, uint16_t depth, IMAGE_FORMAT format)
{
	switch (format) {
	case IMAGE_FORMAT::DXT1:
	case IMAGE_FORMAT::DXT1_ONEBITALPHA:
		if (width < 4 && width > 0)
			width = 4;

		if (height < 4 && height > 0)
			height = 4;

		return ((width + 3) / 4) * ((height + 3) / 4) * 8 * depth;
	case IMAGE_FORMAT::DXT3:
	case IMAGE_FORMAT::DXT5:
		if (width < 4 && width > 0)
			width = 4;

		if (height < 4 && height > 0)
			height = 4;

		return ((width + 3) / 4) * ((height + 3) / 4) * 16 * depth;
	default:
		return width * height * depth * VTFParser::GetImageFormatInfo(format).bytesPerPixel;
	}
}

uint32_t VTFParser::CalcImageSize(uint16_t width, uint16_t height, uint16_t depth, uint8_t numMips, IMAGE_FORMAT format)
{
	if (width == 0 || height == 0 || depth == 0 || numMips == 0) return 0;

	uint32_t imageSize = 0;
	for (uint8_t i = 0; i < numMips; i++) {
		imageSize += VTFParser::CalcImageSize(width, height, depth, format);

		width >>= 1;
		height >>= 1;
		depth >>= 1;

		if (width < 1)  width = 1;
		if (height < 1) height = 1;
		if (depth < 1)  depth = 1;
	}
	return imageSize;
}

uint8_t VTFParser::GetFaceCount(const VTFHeader* pHeader)
{
	if (pHeader == nullptr) return 0;
	return pHeader->flags & static_cast<uint32_t>(TEXTURE_FLAGS::ENVMAP) ? (pHeader->firstFrame != 0xffff && pHeader->version[1] < 5 ? 7 : 6) : 1;
}

bool VTFParser::ParseHeader(const char* pData, size_t size, VTFHeader* pHeader)
{
	if (pData == nullptr || pHeader == nullptr) return false;
	if (size < sizeof(VTFHeaderBase)) return false;

	VTFHeaderBase fileHeader;

	memcpy(&fileHeader, pData, sizeof(VTFHeaderBase));
	if (memcmp(fileHeader.signature, "VTF\0", 4) != 0) return false;
	if (fileHeader.version[0] != 7 || fileHeader.version[1] < 0 || fileHeader.version[1] > 5) return false;
	if (fileHeader.headerSize > size || fileHeader.headerSize > sizeof(VTFHeader)) return false;

	memset(pHeader, 0, sizeof(VTFHeader));
	memcpy(pHeader, pData, fileHeader.headerSize);
	if (pHeader->highResImageFormat == IMAGE_FORMAT::NONE) return false;

	if (fileHeader.version[1] < 2) pHeader->depth = 1;
	if (fileHeader.version[1] < 3) pHeader->numResources = 0;

	return true;
}

bool VTFParser::ParseImageData(const char* pData, size_t size, const VTFHeader* pHeader, char** ppImageData, uint32_t* pImageDataSize)
{
	// Only need to check for null pointers here (just in case), everything else should be validated by ParseHeader
	if (pData == nullptr || pHeader == nullptr || pImageDataSize == nullptr) return false;

	uint32_t imageDataSize = CalcImageSize(
		pHeader->width, pHeader->height,
		pHeader->depth, pHeader->mipmapCount,
		pHeader->highResImageFormat
	) * pHeader->frames * VTFParser::GetFaceCount(pHeader);

	uint32_t imageDataOffset = 0;
	if (pHeader->numResources > 0) {
		if (pHeader->numResources > VTF_MAX_RESOURCES) return false;

		for (uint32_t i = 0; i < pHeader->numResources; i++) {
			if (memcmp(pHeader->resourceInfos[i].tag, "\x30\0\0", 3) == 0) {
				if (imageDataOffset != 0) return false;
				imageDataOffset = pHeader->resourceInfos[i].data;
			}
		}
	} else {
		uint32_t lowResImageSize = 0;
		if (pHeader->lowResImageFormat != IMAGE_FORMAT::NONE) {
			lowResImageSize = VTFParser::CalcImageSize(pHeader->lowResImageWidth, pHeader->lowResImageHeight, 1, pHeader->lowResImageFormat);
		}

		imageDataOffset = pHeader->headerSize + lowResImageSize;
	}

	if (imageDataOffset + imageDataSize > size) return false;

	*ppImageData = reinterpret_cast<char*>(malloc(imageDataSize));
	memcpy(*ppImageData, pData + imageDataOffset, imageDataSize);
	*pImageDataSize = imageDataSize;

	return true;
}

VTFPixel VTFParser::ParsePixel(const char* pPixelData, IMAGE_FORMAT format)
{
	if (pPixelData == nullptr) return VTFPixel{};

	switch (format) {
	case IMAGE_FORMAT::RGBA8888:
		return VTFPixel{
			static_cast<uint8_t>(pPixelData[0]) / 255.f,
			static_cast<uint8_t>(pPixelData[1]) / 255.f,
			static_cast<uint8_t>(pPixelData[2]) / 255.f,
			static_cast<uint8_t>(pPixelData[3]) / 255.f
		};
	case IMAGE_FORMAT::ABGR8888:
		return VTFPixel{
			static_cast<uint8_t>(pPixelData[3]) / 255.f,
			static_cast<uint8_t>(pPixelData[2]) / 255.f,
			static_cast<uint8_t>(pPixelData[1]) / 255.f,
			static_cast<uint8_t>(pPixelData[0]) / 255.f
		};
	case IMAGE_FORMAT::RGB888_BLUESCREEN: // Can't find any distinction between this and RGB888
	case IMAGE_FORMAT::RGB888:
		return VTFPixel{
			static_cast<uint8_t>(pPixelData[0]) / 255.f,
			static_cast<uint8_t>(pPixelData[1]) / 255.f,
			static_cast<uint8_t>(pPixelData[2]) / 255.f
		};
	case IMAGE_FORMAT::BGR888_BLUESCREEN: // See above
	case IMAGE_FORMAT::BGR888:
		return VTFPixel{
			static_cast<uint8_t>(pPixelData[2]) / 255.f,
			static_cast<uint8_t>(pPixelData[1]) / 255.f,
			static_cast<uint8_t>(pPixelData[0]) / 255.f
		};
	case IMAGE_FORMAT::RGB565:
		return VTFPixel{
			static_cast<uint8_t>(pPixelData[0] & 0b11111000) / 255.f,
			static_cast<uint8_t>(pPixelData[0] << 5 + (pPixelData[1] & 0b11100000) >> 3) / 255.f,
			static_cast<uint8_t>((pPixelData[1] & 0b11111) << 3) / 255.f
		};
	case IMAGE_FORMAT::I8:
	{
		float intensity = static_cast<uint8_t>(pPixelData[0]) / 255.f;
		return VTFPixel{ intensity, intensity, intensity };
	}
	case IMAGE_FORMAT::IA88:
	{
		float intensity = static_cast<uint8_t>(pPixelData[0]) / 255.f;
		return VTFPixel{
			intensity, intensity, intensity,
			static_cast<uint8_t>(pPixelData[1]) / 255.f
		};
	}
	//case IMAGE_FORMAT::P8: Not currently supported
	case IMAGE_FORMAT::A8:
		return VTFPixel{ 0, 0, 0, static_cast<uint8_t>(pPixelData[0]) / 255.f };
	case IMAGE_FORMAT::ARGB8888:
		return VTFPixel{
			static_cast<uint8_t>(pPixelData[1]) / 255.f,
			static_cast<uint8_t>(pPixelData[2]) / 255.f,
			static_cast<uint8_t>(pPixelData[3]) / 255.f,
			static_cast<uint8_t>(pPixelData[0]) / 255.f
		};
	case IMAGE_FORMAT::BGRX8888: // Can't find any difference between this and BGRA8888
	case IMAGE_FORMAT::BGRA8888:
		return VTFPixel{
			static_cast<uint8_t>(pPixelData[2]) / 255.f,
			static_cast<uint8_t>(pPixelData[1]) / 255.f,
			static_cast<uint8_t>(pPixelData[0]) / 255.f,
			static_cast<uint8_t>(pPixelData[3]) / 255.f
		};
	// DXT1 through 5 should be decompressed on read
	case IMAGE_FORMAT::BGR565:
		return VTFPixel{
			static_cast<uint8_t>((pPixelData[1] & 0b11111) << 3) / 255.f,
			static_cast<uint8_t>(pPixelData[0] << 5 + (pPixelData[1] & 0b11100000) >> 3) / 255.f,
			static_cast<uint8_t>(pPixelData[0] & 0b11111000) / 255.f
		};
	case IMAGE_FORMAT::BGRX5551:
	case IMAGE_FORMAT::BGRA5551:
		return VTFPixel{
			static_cast<uint8_t>((pPixelData[1] & 0b00111110) << 2) / 255.f,
			static_cast<uint8_t>(pPixelData[0] << 5 + (pPixelData[1] & 0b11000000) >> 3) / 255.f,
			static_cast<uint8_t>(pPixelData[0] & 0b11111000) / 255.f,
			static_cast<float>(pPixelData[1] & 0b1)
		};
	case IMAGE_FORMAT::BGRA4444:
		return VTFPixel{
			static_cast<uint8_t>(pPixelData[1] & 0b11110000) / 255.f,
			static_cast<uint8_t>(pPixelData[0] << 4) / 255.f,
			static_cast<uint8_t>(pPixelData[0] & 0b11110000) / 255.f,
			static_cast<uint8_t>(pPixelData[1] << 4) / 255.f
		};
	// Not currently supported (not even sure what these formats are used for)
	//case IMAGE_FORMAT::UV88:
	//case IMAGE_FORMAT::UVWQ8888:
	//case IMAGE_FORMAT::UVLX8888:
	case IMAGE_FORMAT::RGBA16161616:
	case IMAGE_FORMAT::RGBA16161616F: // These 2 formats are stored IDENTICALLY in the VTF, even though VTFEdit displays them differently (float version has incorrect colours)
		return VTFPixel{
			static_cast<float>(*reinterpret_cast<const uint16_t*>(pPixelData)) / static_cast<float>(UINT16_MAX),
			static_cast<float>(*reinterpret_cast<const uint16_t*>(pPixelData + 2)) / static_cast<float>(UINT16_MAX),
			static_cast<float>(*reinterpret_cast<const uint16_t*>(pPixelData + 4)) / static_cast<float>(UINT16_MAX),
			static_cast<float>(*reinterpret_cast<const uint16_t*>(pPixelData + 6)) / static_cast<float>(UINT16_MAX)
		};
	default:
		return VTFPixel{};
	}
}

#pragma once

#include "Enums.h"
#include "Structs.h"
#include <cstdint>

namespace VTFParser
{
	/// <summary>
	/// Gets information regarding the specified format
	/// </summary>
	/// <param name="format">Image format to get the info struct of</param>
	/// <returns>ImageFormatInfo struct</returns>
	ImageFormatInfo GetImageFormatInfo(IMAGE_FORMAT format);

	/// <summary>
	/// Calculates the size of an image block assuming no mipmaps
	/// </summary>
	/// <param name="width">Width of the image</param>
	/// <param name="height">Height of the image</param>
	/// <param name="depth">Depth of the image (volumetrics)</param>
	/// <param name="format">Format of the image</param>
	/// <returns>Size of the image block in bytes</returns>
	uint32_t CalcImageSize(uint16_t width, uint16_t height, uint16_t depth, IMAGE_FORMAT format);

	/// <summary>
	/// Calculates the size of an image block
	/// </summary>
	/// <param name="width">Width of the image</param>
	/// <param name="height">Height of the image</param>
	/// <param name="depth">Depth of the image (volumetrics)</param>
	/// <param name="numMips">Number of MIP levels</param>
	/// <param name="format">Format of the image</param>
	/// <returns>Size of the image block in bytes</returns>
	uint32_t CalcImageSize(uint16_t width, uint16_t height, uint16_t depth, uint8_t numMips, IMAGE_FORMAT format);

	/// <summary>
	/// Gets the number of faces in the image (only applicable to envmaps)
	/// </summary>
	/// <param name="pHeader">Readonly pointer to the VTF's header</param>
	/// <returns>Number of faces</returns>
	uint8_t GetFaceCount(const VTFHeader* pHeader);

	/// <summary>
	/// Parses a VTF's header and validates it
	/// </summary>
	/// <param name="pData">Pointer to binary VTF data</param>
	/// <param name="size">Size of the data in bytes</param>
	/// <param name="pHeader">Pointer to header struct to populate</param>
	/// <returns>Whether the parse was successful</returns>
	bool ParseHeader(const char* pData, size_t size, VTFHeader* pHeader);

	/// <summary>
	/// Parses the high resolution image data of a VTF
	/// </summary>
	/// <param name="pData">Pointer to binary VTF data</param>
	/// <param name="size">Size of the data in bytes</param>
	/// <param name="pHeader">Readonly pointer to the VTF's header</param>
	/// <param name="pImageData">Pointer to char* that will be set to the image data</param>
	/// <param name="pImageDataSize">Pointer to size_t that will be set to the size of the image data in bytes</param>
	/// <returns>Whether the parse was successful</returns>
	bool ParseImageData(const char* pData, size_t size, const VTFHeader* pHeader, char** ppImageData, uint32_t* pImageDataSize);
}

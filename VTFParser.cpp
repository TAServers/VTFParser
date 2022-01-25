#include "VTFParser.h"
#include "FileFormat/Parser.h"

#include <iostream>
#include <fstream>
#include <filesystem>

int main()
{
	// Read a VTF
	std::filesystem::path path = "input.vtf";

	if (!std::filesystem::is_regular_file(path)) {
		printf("Couldn't find file");
		return 0;
	}

	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		printf("Failed to open file");
		return 0;
	}

	const size_t size = std::filesystem::file_size(path);
	char* data = reinterpret_cast<char*>(malloc(size));
	file.read(data, size);
	file.close();

	// Test the library
	VTFHeader header;

	if (!VTFParser::ParseHeader(data, size, &header)) {
		printf("Failed to parse the file's header");
		return 0;
	}

	printf(
		"VTF v%u.%u | %ux%u, %u MIP Levels | %s\n",
		header.version[0], header.version[1],
		header.width, header.height,
		header.mipmapCount,
		VTFParser::GetImageFormatInfo(header.highResImageFormat).name
	);

	char* pImageData;
	uint32_t imageDataSize;
	if (!VTFParser::ParseImageData(data, size, &header, &pImageData, &imageDataSize)) {
		printf("Failed to parse high res image data");
		return 0;
	}

	printf("Pixel 0,0 should be %u, %u, %u\n", static_cast<unsigned char>(pImageData[0]), static_cast<unsigned char>(pImageData[1]), static_cast<unsigned char>(pImageData[2]));

	// Cleanup
	free(data);
	free(pImageData);
}

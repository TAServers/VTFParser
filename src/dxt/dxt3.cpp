#include "dxt.hpp"
#include "structs.hpp"
#include "../helpers/offset-data-view.hpp"

/*
	Modified versions of VTFLib's DXTn decompression functions
	All credit goes to https://github.com/NeilJed/VTFLib
*/

namespace VtfParser::Dxt {
  std::vector<Colour<4, uint8_t>> decompressDxt3(
    const std::weak_ptr<std::vector<std::byte>>& src, const size_t width, const size_t height
  ) {
    auto dataView = OffsetDataView(src);
    std::array<Colour8888, 4> colours;
    std::vector<Colour<4, uint8_t>> output(width * height);

    for (uint32_t ySource = 0; ySource < height; ySource += 4) {
      for (uint32_t xSource = 0; xSource < width; xSource += 4) {
        const auto alphaBlock = dataView.parseStruct<DXTAlphaBlockExplicit>(0, "Failed to parse DXT3 alpha block");
        dataView = dataView.withRelativeOffset(8);

        const auto colour0 = dataView.parseStruct<Colour565>(0, "Failed to parse DXT3 colour");
        const auto colour1 = dataView.parseStruct<Colour565>(2, "Failed to parse DXT3 colour");
        const auto bitmask = dataView.parseStruct<uint32_t>(sizeof(uint32_t), "Failed to parse DXT3 bitmask");
        dataView = dataView.withRelativeOffset(8);

        colours[0] = Colour8888(colour0, 255);
        colours[1] = Colour8888(colour1, 255);

        // Four-color block: derive the other two colors.
        // 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
        // These 2-bit codes correspond to the 2-bit fields
        // stored in the 64-bit block.
        colours[2] = Colour8888(
          (2 * colours[0].b + colours[1].b + 1) / 3,
          (2 * colours[0].g + colours[1].g + 1) / 3,
          (2 * colours[0].r + colours[1].r + 1) / 3,
          255
        );

        colours[3] = Colour8888(
          (colours[0].b + 2 * colours[1].b + 1) / 3,
          (colours[0].g + 2 * colours[1].g + 1) / 3,
          (colours[0].r + 2 * colours[1].r + 1) / 3,
          255
        );

        uint32_t pixelIndex = 0;
        for (uint32_t yOffset = 0; yOffset < 4; yOffset++) {
          const uint32_t yDest = ySource + yOffset;
          auto alphaBlockWord = alphaBlock.row.at(yOffset);

          for (uint32_t xOffset = 0; xOffset < 4; xOffset++) {
            const uint32_t xDest = xSource + xOffset;
            const uint32_t colourIndex = (bitmask & (0x03u << pixelIndex * 2u)) >> pixelIndex * 2u;

            if (xDest < width && yDest < height) {
              const auto& colour = colours.at(colourIndex);
              const uint8_t maskedAlphaWord = alphaBlockWord & 0x0fu;

              output[yDest * width + xDest] = Colour<4, uint8_t>{
                colour.r,
                colour.g,
                colour.b,
                maskedAlphaWord | (maskedAlphaWord << 4u),
              };
            }

            alphaBlockWord >>= 4u;
            pixelIndex++;
          }
        }
      }
    }

    return std::move(output);
  }
}

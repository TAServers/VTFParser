#include "dxt.hpp"
#include "structs.hpp"
#include "../helpers/offset-data-view.hpp"

/*
	Modified versions of VTFLib's DXTn decompression functions
	All credit goes to https://github.com/NeilJed/VTFLib
*/

namespace VtfParser::Dxt {
  namespace {
    struct LowerAlphaMask {
      uint32_t mask;
    };

    struct UpperAlphaMask {
      std::array<uint8_t, 3> padding;
      uint32_t mask;
    };
  }

  std::vector<Colour<4, uint8_t>> decompressDxt5(
    const std::weak_ptr<std::vector<std::byte>>& src, const size_t width, const size_t height
  ) {
    auto dataView = OffsetDataView(src);
    std::array<Colour8888, 4> colours;
    std::array<uint8_t, 8> alphas{};
    std::vector<Colour<4, uint8_t>> output(width * height);

    for (uint32_t ySource = 0; ySource < height; ySource += 4) {
      for (uint32_t xSource = 0; xSource < width; xSource += 4) {
        alphas[0] = dataView.parseStruct<uint8_t>(0, "Failed to parse DXT5 alpha");
        alphas[1] = dataView.parseStruct<uint8_t>(1, "Failed to parse DXT5 alpha");
        auto lowerAlphaMask = dataView.parseStruct<LowerAlphaMask>(2, "Failed to parse DXT5 alpha mask");
        auto upperAlphaMask = dataView.parseStruct<UpperAlphaMask>(2, "Failed to parse DXT5 alpha mask");
        dataView = dataView.withRelativeOffset(8);

        const auto colour0 = dataView.parseStruct<Colour565>(0, "Failed to parse DXT5 colour");
        const auto colour1 = dataView.parseStruct<Colour565>(2, "Failed to parse DXT5 colour");
        const auto bitmask = dataView.parseStruct<uint32_t>(sizeof(uint32_t), "Failed to parse DXT5 bitmask");
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

        // 8-alpha or 6-alpha block?
        if (alphas[0] > alphas[1]) {
          // 8-alpha block:  derive the other six alphas.
          // Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
          alphas[2] = (6 * alphas[0] + 1 * alphas[1] + 3) / 7; // bit code 010
          alphas[3] = (5 * alphas[0] + 2 * alphas[1] + 3) / 7; // bit code 011
          alphas[4] = (4 * alphas[0] + 3 * alphas[1] + 3) / 7; // bit code 100
          alphas[5] = (3 * alphas[0] + 4 * alphas[1] + 3) / 7; // bit code 101
          alphas[6] = (2 * alphas[0] + 5 * alphas[1] + 3) / 7; // bit code 110
          alphas[7] = (1 * alphas[0] + 6 * alphas[1] + 3) / 7; // bit code 111
        } else {
          // 6-alpha block.
          // Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
          alphas[2] = (4 * alphas[0] + 1 * alphas[1] + 2) / 5; // Bit code 010
          alphas[3] = (3 * alphas[0] + 2 * alphas[1] + 2) / 5; // Bit code 011
          alphas[4] = (2 * alphas[0] + 3 * alphas[1] + 2) / 5; // Bit code 100
          alphas[5] = (1 * alphas[0] + 4 * alphas[1] + 2) / 5; // Bit code 101
          alphas[6] = 0x00; // Bit code 110
          alphas[7] = 0xFF; // Bit code 111
        }

        uint32_t pixelIndex = 0;
        for (uint32_t yOffset = 0; yOffset < 4; yOffset++) {
          const uint32_t yDest = ySource + yOffset;
          auto& alphaMask = yOffset < 2 ? lowerAlphaMask.mask : upperAlphaMask.mask;

          for (uint32_t xOffset = 0; xOffset < 4; xOffset++, pixelIndex++) {
            const uint32_t xDest = xSource + xOffset;

            if (yDest < width && yDest < height) {
              const uint32_t colourIndex = (bitmask & (0x03u << pixelIndex * 2u)) >> pixelIndex * 2u;
              const auto& colour = colours.at(colourIndex);

              output[yDest * width + xDest] = Colour<4, uint8_t>{
                colour.r,
                colour.g,
                colour.b,
                alphas.at(alphaMask & 0x07u),
              };
            }

            alphaMask >>= 3u;
            pixelIndex++;
          }
        }
      }
    }

    return std::move(output);
  }
}

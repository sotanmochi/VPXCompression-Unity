// Copyright (c) 2020 Soichiro Sugimoto
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include <vector>
#include <vpx/vp8.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_decoder.h>

namespace VPXCompression
{
    class VP8Decoder
    {
    public:
        VP8Decoder();
        ~VP8Decoder();
        std::vector<uint8_t> decode(const uint8_t* frame, int frame_size);

    private:
        std::vector<uint8_t> create_rgb_from_yv12(const vpx_image_t* img);

    private:
        vpx_codec_iface_t* codec_interface;
        vpx_codec_ctx_t codec_context;
    };
}
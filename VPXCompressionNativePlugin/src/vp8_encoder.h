// Copyright (c) 2020 Soichiro Sugimoto
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include <vector>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>

namespace VPXCompression
{
    class VP8Encoder
    {
    public:
        VP8Encoder(int width, int height);
        ~VP8Encoder();
        std::vector<uint8_t> encode_from_rgba(const uint8_t* rgba_frame, bool keyframe, int width, int height, int stride);
        std::vector<uint8_t> encode_from_bgra(const uint8_t* bgra_frame, bool keyframe, int width, int height, int stride);

    private:
        std::vector<uint8_t> encode_frame(bool keyframe);
        bool create_yv12_image_buffer_from_color_frame(const uint8_t* color_frame, int width, int height, int stride, int red_pos, int green_pos, int blue_pos);

    private:
        vpx_codec_iface_t* _codec_interface;
        vpx_codec_ctx_t _codec_context;
        vpx_image_t _vpx_image_buffer;
        int _frame_index;
    };
}
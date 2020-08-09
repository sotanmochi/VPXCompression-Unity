// Copyright (c) 2020 Soichiro Sugimoto
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include <algorithm>
#include <iostream>
#include "vp8_decoder.h"

namespace VPXCompression
{

inline int clamp(int v, int min, int max)
{
	return std::min(std::max(v, min), max);
}

VP8Decoder::VP8Decoder() : codec_interface(vpx_codec_vp8_dx())
{
	vpx_codec_err_t res = vpx_codec_dec_init(&codec_context, codec_interface, NULL, 0);
	if (res != VPX_CODEC_OK)
		throw std::exception("Error from vpx_codec_dec_init.");
}

VP8Decoder::~VP8Decoder()
{
    if (vpx_codec_destroy(&codec_context))
        std::cout << "Error from vpx_codec_destroy." << std::endl;
}

std::vector<uint8_t> VP8Decoder::decode(const uint8_t* frame, int frame_size)
{ 
    vpx_codec_err_t res = vpx_codec_decode(&codec_context, frame, frame_size, 0, 0);

    if (res != VPX_CODEC_OK)
        throw std::exception("Error from vpx_codec_decode in Vp8Encoder::decode()...");

    std::vector<uint8_t> rgb_data;
	vpx_codec_iter_t iter = NULL;
	vpx_image_t* decoded_img;
    while ((decoded_img = vpx_codec_get_frame(&codec_context, &iter)))
    {
        rgb_data = create_rgb_from_yv12(decoded_img);
    }

    return rgb_data;
}

std::vector<uint8_t> VP8Decoder::create_rgb_from_yv12(const vpx_image_t* img)
{
    int width = img->d_w;
    int height = img->d_h;
    std::vector<uint8_t> rgb_data(3 * width * height);

    uint8_t* y_channel = img->planes[VPX_PLANE_Y];
    uint8_t* u_channel = img->planes[VPX_PLANE_U];
    uint8_t* v_channel = img->planes[VPX_PLANE_V];

    int y_stride = img->stride[VPX_PLANE_Y];
    int u_stride = img->stride[VPX_PLANE_U];
    int v_stride = img->stride[VPX_PLANE_V];

    int rgb_index = 0;
    for (int py = 0; py < height; py++)
    {
        for (int px = 0; px < width; px++)
        {
            int y_channel_index = py * y_stride + px;
            int u_channel_index = (py/2) * u_stride + px/2;
            int v_channel_index = (py/2) * v_stride + px/2;

            int y = y_channel[y_channel_index];
            int u = u_channel[u_channel_index];
            int v = v_channel[v_channel_index];

            int c = y - 16;
            int d = u - 128;
            int e = v - 128;

			int r = clamp((298 * c           + 409 * e + 128) >> 8, 0, 255);
			int g = clamp((298 * c - 100 * d - 208 * e + 128) >> 8, 0, 255);
			int b = clamp((298 * c + 516 * d           + 128) >> 8, 0, 255);

			rgb_data[rgb_index + 0] = static_cast<uint8_t>(r);
			rgb_data[rgb_index + 1] = static_cast<uint8_t>(g);
			rgb_data[rgb_index + 2] = static_cast<uint8_t>(b);

            rgb_index += 3;
        }
    }

    return rgb_data;
}

}
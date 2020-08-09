// Copyright (c) 2020 Soichiro Sugimoto
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include <iostream>
#include "vp8_encoder.h"

namespace VPXCompression
{

VP8Encoder::VP8Encoder(int width, int height)
    : _codec_interface(vpx_codec_vp8_cx()), _codec_context(), _vpx_image_buffer(), _frame_index(0)
{
    vpx_codec_enc_cfg_t configuration;

    vpx_codec_err_t res = vpx_codec_enc_config_default(_codec_interface, &configuration, 0);
    if (res != VPX_CODEC_OK)
        throw std::exception("Error from vpx_codec_enc_config_default.");

    // From https://developers.google.com/media/vp9/live-encoding
    // See also https://www.webmproject.org/docs/encoder-parameters/
    configuration.g_w = width;
    configuration.g_h = height;
    configuration.rc_target_bitrate = 4000;

    configuration.g_threads = 4;
    configuration.g_lag_in_frames = 0;
    configuration.rc_min_quantizer = 4;
    configuration.rc_max_quantizer = 48;
    //configuration.rc_max_quantizer = 56;

    configuration.rc_end_usage = VPX_CBR;

    res = vpx_codec_enc_init(&_codec_context, _codec_interface, &configuration, 0);
    if (res != VPX_CODEC_OK)
        throw std::exception("Error from vpx_codec_enc_init.");

    vpx_codec_control(&_codec_context, VP8E_SET_CPUUSED, 6);
    vpx_codec_control(&_codec_context, VP8E_SET_STATIC_THRESHOLD, 0);
    vpx_codec_control(&_codec_context, VP8E_SET_MAX_INTRA_BITRATE_PCT, 300);

    if (!vpx_img_alloc(&_vpx_image_buffer, VPX_IMG_FMT_YV12, configuration.g_w, configuration.g_h, 32))
        throw std::exception("Error from vpx_img_alloc.");
}

VP8Encoder::~VP8Encoder()
{
    vpx_img_free(&_vpx_image_buffer);
    if (vpx_codec_destroy(&_codec_context))
        std::cout << "Error from vpx_codec_destroy." << std::endl;
}

std::vector<uint8_t> VP8Encoder::encode_from_rgba(const uint8_t* rgba_frame, bool keyframe, int width, int height, int stride = 4)
{
    if (!create_yv12_image_buffer_from_color_frame(rgba_frame, width, height, stride, 0, 1, 2))
        std::cout << "Error from create_yuv_from_bgra." << std::endl;

    return encode_frame(keyframe);
}

std::vector<uint8_t> VP8Encoder::encode_from_bgra(const uint8_t* bgra_frame, bool keyframe, int width, int height, int stride = 4)
{
    if (!create_yv12_image_buffer_from_color_frame(bgra_frame, width, height, stride, 2, 1, 0))
        std::cout << "Error from create_yuv_from_bgra." << std::endl;

    return encode_frame(keyframe);
}

std::vector<uint8_t> VP8Encoder::encode_frame(bool keyframe)
{
    int flags = keyframe ? VPX_EFLAG_FORCE_KF : 0;
    vpx_codec_err_t res = vpx_codec_encode(&_codec_context, &_vpx_image_buffer, _frame_index++, 1, flags, VPX_DL_REALTIME);

    if (res != VPX_CODEC_OK)
        throw std::exception("Error from vpx_codec_encode in VP8Encoder::encode_bgra()...");

    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t* pkt = NULL;
    std::vector<uint8_t> encoded_data;
    while (pkt = vpx_codec_get_cx_data(&_codec_context, &iter))
    {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT)
        {
            if (!encoded_data.empty())
                throw std::exception("Multiple frames found from a Vp8 packet...");

            encoded_data.resize(pkt->data.frame.sz);
            memcpy(encoded_data.data(), (uint8_t*)pkt->data.frame.buf, pkt->data.frame.sz);
        }
    }

    return encoded_data;
}

bool VP8Encoder::create_yv12_image_buffer_from_color_frame(const uint8_t* color_frame, int width, int height, int stride, int red_pos, int green_pos, int blue_pos)
{
    red_pos = red_pos % stride;
    green_pos = green_pos % stride;
    blue_pos = blue_pos % stride;

    uint8_t* y_channel = _vpx_image_buffer.planes[VPX_PLANE_Y];
    uint8_t* u_channel = _vpx_image_buffer.planes[VPX_PLANE_U];
    uint8_t* v_channel = _vpx_image_buffer.planes[VPX_PLANE_V];

    int size = width * height;
    for (int index = 0; index < size; index++)
    {
        int buffer_index = index * stride;
        uint8_t r = color_frame[buffer_index + red_pos];
        uint8_t g = color_frame[buffer_index + green_pos];
        uint8_t b = color_frame[buffer_index + blue_pos];
        y_channel[index] = ((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
    }

    int u_stride = _vpx_image_buffer.stride[VPX_PLANE_U];
    int v_stride = _vpx_image_buffer.stride[VPX_PLANE_V];

    for (int y = 0; y < height / 2; y++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            int pixel_pos = 2 * (y * width + x);
            int buffer_index = pixel_pos * stride;

            uint8_t r = color_frame[buffer_index + red_pos];
            uint8_t g = color_frame[buffer_index + green_pos];
            uint8_t b = color_frame[buffer_index + blue_pos];

            u_channel[y * u_stride + x] = ((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
            v_channel[y * v_stride + x] = ((112 * r - 94 * g - 18  * b + 128) >> 8) + 128;
        }
    }

    return true;
}

}
// Copyright (c) 2020 Soichiro Sugimoto
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "IUnityInterface.h"
#include "vp8_encoder.h"
#include "vp8_decoder.h"

extern "C"
{
    UNITY_INTERFACE_EXPORT VPXCompression::VP8Encoder* UNITY_INTERFACE_API create_vp8_encoder(int width, int height)
    {
        return new VPXCompression::VP8Encoder(width, height);
    }

    UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API delete_vp8_encoder(VPXCompression::VP8Encoder* ptr)
    {
        delete ptr;
    }

    UNITY_INTERFACE_EXPORT VPXCompression::VP8Decoder* UNITY_INTERFACE_API create_vp8_decoder()
    {
        return new VPXCompression::VP8Decoder();
    }

    UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API delete_vp8_decoder(VPXCompression::VP8Decoder* ptr)
    {
        delete ptr;
    }

    UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API vp8_encoder_encode_from_rgba
    (
        VPXCompression::VP8Encoder* encoder,
        const uint8_t* rgba_frame,
        int width,
        int height,
        bool keyframe,
        uint8_t* vp8_frame_output
    )
    {
        std::vector<uint8_t> vp8_frame = encoder->encode_from_rgba(rgba_frame, keyframe, width, height, 4);
        for (auto itr = vp8_frame.begin(); itr != vp8_frame.end(); itr++)
        {
            *vp8_frame_output++ = *itr;
        }

        return vp8_frame.size();
    }

    UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API vp8_encoder_encode_from_bgra
    (
        VPXCompression::VP8Encoder* encoder,
        const uint8_t* bgra_frame,
        int width,
        int height,
        bool keyframe,
        uint8_t* vp8_frame_output
    )
    {
        std::vector<uint8_t> vp8_frame = encoder->encode_from_bgra(bgra_frame, keyframe, width, height, 4);
        for (auto itr = vp8_frame.begin(); itr != vp8_frame.end(); itr++)
        {
            *vp8_frame_output++ = *itr;
        }

        return vp8_frame.size();
    }

    UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API vp8_decoder_decode
    (
        VPXCompression::VP8Decoder* decoder,
        const uint8_t* vp8_frame,
        int vp8_frame_size,
        uint8_t* rgb_frame_output
    )
    {
        std::vector<uint8_t> rgb_frame = decoder->decode(vp8_frame, vp8_frame_size);
        for (auto itr = rgb_frame.begin(); itr != rgb_frame.end(); itr++)
        {
            *rgb_frame_output++ = *itr;
        }

        return rgb_frame.size();
    }
}
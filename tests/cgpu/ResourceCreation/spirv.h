#pragma once
#include <stdint.h>

// -fspv-reflect is needed when use DXC
/**
 * Vertex shader SPIR-V.
 * \code
    #pragma shader_stage(vertex)

    struct VertexIn
    {
        float2 inPos : SV_POSITION;
        float4 vertColor : FuckDXC;
    };

    struct VertexOut
    {
        float4 fragColor : COLOR;
    };

    VertexOut main(VertexIn vin)
    {
        VertexOut vout;
        vout.fragColor = vin.vertColor;
        return vout;
    }
 * \endcode
 */
static const uint8_t triangle_vert_spirv[448] = {
    0x03, 0x02, 0x23, 0x07, 0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x0f, 0x00, 0x00, 0x00, // ..#.............
    0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, // ................
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
    0x01, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // ....main........
    0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, // ................
    0x58, 0x02, 0x00, 0x00, 0x05, 0x00, 0x07, 0x00, 0x02, 0x00, 0x00, 0x00, 0x69, 0x6e, 0x2e, 0x76, // X...........in.v
    0x61, 0x72, 0x2e, 0x53, 0x56, 0x5f, 0x50, 0x4f, 0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x00, // ar.SV_POSITION..
    0x05, 0x00, 0x06, 0x00, 0x03, 0x00, 0x00, 0x00, 0x69, 0x6e, 0x2e, 0x76, 0x61, 0x72, 0x2e, 0x46, // ........in.var.F
    0x75, 0x63, 0x6b, 0x44, 0x58, 0x43, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x04, 0x00, 0x00, 0x00, // uckDXC..........
    0x6f, 0x75, 0x74, 0x2e, 0x76, 0x61, 0x72, 0x2e, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x00, 0x00, 0x00, // out.var.COLOR...
    0x05, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, // ........main....
    0x47, 0x00, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // G...............
    0x47, 0x00, 0x04, 0x00, 0x03, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // G...............
    0x47, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // G...............
    0x16, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, // ........ .......
    0x06, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, // ............ ...
    0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, // ................
    0x08, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, // ............ ...
    0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, // ............ ...
    0x0a, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, 0x00, // ................
    0x0b, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, // ....!...........
    0x3b, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ;...............
    0x3b, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ;...............
    0x3b, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ;...............
    0x36, 0x00, 0x05, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 6...............
    0x0c, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, // ............=...
    0x08, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, // ............>...
    0x04, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00  // ............8...
};

/**
 * Fragment shader SPIR-V.
 * \code
    #pragma shader_stage(fragment)

    [[vk::binding(0, 0)]]
    cbuffer perDrawCBuffer : register(b0)
    {
        float4 A;
        float4 B;
    };

    [[vk::binding(0, 1)]]
    cbuffer perFrameCBuffer : register(b1)
    {
        float4 C;
        float4 D;
    };

    float4 main(float4 color : COLOR) : SV_TARGET
    {
        return color * A * C;
    }
 * \endcode
 */
static const uint8_t triangle_frag_spirv[964] = {
    0x03, 0x02, 0x23, 0x07, 0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x00, 0x00, // ..#.............
    0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, // ................
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x07, 0x00, 0x04, 0x00, 0x00, 0x00, // ................
    0x01, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // ....main........
    0x03, 0x00, 0x00, 0x00, 0x10, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, // ................
    0x03, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0x00, 0x05, 0x00, 0x07, 0x00, // ........X.......
    0x04, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x70, 0x65, 0x72, 0x44, 0x72, 0x61, 0x77, // ....type.perDraw
    0x43, 0x42, 0x75, 0x66, 0x66, 0x65, 0x72, 0x00, 0x06, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, // CBuffer.........
    0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x06, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, // ....A...........
    0x01, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x05, 0x00, 0x00, 0x00, // ....B...........
    0x70, 0x65, 0x72, 0x44, 0x72, 0x61, 0x77, 0x43, 0x42, 0x75, 0x66, 0x66, 0x65, 0x72, 0x00, 0x00, // perDrawCBuffer..
    0x05, 0x00, 0x08, 0x00, 0x06, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x70, 0x65, 0x72, // ........type.per
    0x46, 0x72, 0x61, 0x6d, 0x65, 0x43, 0x42, 0x75, 0x66, 0x66, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00, // FrameCBuffer....
    0x06, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x00, 0x00, 0x00, // ............C...
    0x06, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, // ............D...
    0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x00, 0x00, 0x70, 0x65, 0x72, 0x46, 0x72, 0x61, 0x6d, 0x65, // ........perFrame
    0x43, 0x42, 0x75, 0x66, 0x66, 0x65, 0x72, 0x00, 0x05, 0x00, 0x06, 0x00, 0x02, 0x00, 0x00, 0x00, // CBuffer.........
    0x69, 0x6e, 0x2e, 0x76, 0x61, 0x72, 0x2e, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x00, 0x00, 0x00, 0x00, // in.var.COLOR....
    0x05, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x6f, 0x75, 0x74, 0x2e, 0x76, 0x61, 0x72, 0x2e, // ........out.var.
    0x53, 0x56, 0x5f, 0x54, 0x41, 0x52, 0x47, 0x45, 0x54, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, // SV_TARGET.......
    0x01, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, // ....main....G...
    0x02, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, // ............G...
    0x03, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, // ............G...
    0x05, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, // ....".......G...
    0x05, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, // ....!.......G...
    0x07, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, // ....".......G...
    0x07, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, // ....!.......H...
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........#.......
    0x48, 0x00, 0x05, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, // H...........#...
    0x10, 0x00, 0x00, 0x00, 0x47, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // ....G...........
    0x48, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, // H...........#...
    0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ....H...........
    0x23, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x47, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, // #.......G.......
    0x02, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, // ............ ...
    0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, // ....+...........
    0x00, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, // ............ ...
    0x17, 0x00, 0x04, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // ................
    0x1e, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, // ................
    0x20, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, //  ...............
    0x1e, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, // ................
    0x20, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, //  ...............
    0x20, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, //  ...............
    0x20, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, //  ...............
    0x13, 0x00, 0x02, 0x00, 0x10, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00, 0x11, 0x00, 0x00, 0x00, // ........!.......
    0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // .... ...........
    0x0b, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // ....;...........
    0x02, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, // ....;...........
    0x02, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // ....;...........
    0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ....;...........
    0x03, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ....6...........
    0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x13, 0x00, 0x00, 0x00, // ................
    0x3d, 0x00, 0x04, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // =...............
    0x41, 0x00, 0x05, 0x00, 0x12, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // A...............
    0x09, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, // ....=...........
    0x15, 0x00, 0x00, 0x00, 0x85, 0x00, 0x05, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, // ................
    0x14, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00, 0x12, 0x00, 0x00, 0x00, // ........A.......
    0x18, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, // ............=...
    0x0b, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x85, 0x00, 0x05, 0x00, // ................
    0x0b, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, // ................
    0x3e, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, // >...............
    0x38, 0x00, 0x01, 0x00                                                                          // 8...
};

#define MANDELBROT_WIDTH 3200
#define MANDELBROT_HEIGHT 2400
#define WORKGROUP_SIZE 32

/**
 * Compute shader HLSL
 * \code
    #define WIDTH 3200
    #define HEIGHT 2400

    struct Pixel
    {
        float4 value;
    };

    [[vk::binding(0, 0)]]
    RWByteAddressBuffer buf : register(u0);

    [numthreads(32, 32, 1)]
    void main(uint3 ThreadID : SV_DispatchThreadID)
    {
        if(ThreadID.x >= WIDTH || ThreadID.y >= HEIGHT)
            return;
        float x = float(ThreadID.x) / 3200.0f;
        float y = float(ThreadID.y) / 2400.0f;
        float2 uv = float2(x, y);
        float n = 0.0f;
        float2 c = float2(-0.444999992847442626953125f, 0.0f) + ((uv - 0.5f.xx) * 2.3399999141693115234375f);
        float2 z = 0.0f.xx;
        const int M =128;
        for (int i = 0; i < M; i++)
        {
            z = float2((z.x * z.x) - (z.y * z.y), (2.0f * z.x) * z.y) + c;
            if (dot(z, z) > 2.0f)
            {
                break;
            }
            n += 1.0f;
        }

        // we use a simple cosine palette to determine color:
        // http://iquilezles.org/www/articles/palettes/palettes.htm
        float t = float(n) / float(M);
        float3 d = float3(0.3, 0.3 ,0.5);
        float3 e = float3(-0.2, -0.3 ,-0.5);
        float3 f = float3(2.1, 2.0, 3.0);
        float3 g = float3(0.0, 0.1, 0.0);
        float4 color = float4(d + (e * cos(((f * t) + g) * 6.28318023681640625f)), 1.0f);
        buf.Store4(((3200u * ThreadID.y) + ThreadID.x) * 16 + 0, asuint(color));
    }
 * \endcode
 */
static const uint8_t mandelbrot_comp_spirv[2648] = {
    0x03, 0x02, 0x23, 0x07, 0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x7c, 0x00, 0x00, 0x00, // ..#.........|...
    0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x06, 0x00, // ................
    0x01, 0x00, 0x00, 0x00, 0x47, 0x4c, 0x53, 0x4c, 0x2e, 0x73, 0x74, 0x64, 0x2e, 0x34, 0x35, 0x30, // ....GLSL.std.450
    0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ................
    0x0f, 0x00, 0x06, 0x00, 0x05, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, // ............main
    0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x10, 0x00, 0x06, 0x00, 0x02, 0x00, 0x00, 0x00, // ................
    0x11, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // .... ... .......
    0x03, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0x00, 0x05, 0x00, 0x09, 0x00, // ........X.......
    0x04, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x52, 0x57, 0x42, 0x79, 0x74, 0x65, 0x41, // ....type.RWByteA
    0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x42, 0x75, 0x66, 0x66, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00, // ddressBuffer....
    0x05, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x62, 0x75, 0x66, 0x00, 0x05, 0x00, 0x04, 0x00, // ........buf.....
    0x02, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, // ....main....G...
    0x03, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, // ............G...
    0x05, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, // ....".......G...
    0x05, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, // ....!.......G...
    0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, // ............H...
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........#.......
    0x47, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, // G...............
    0x07, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, // .... .......+...
    0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, // ................
    0x09, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, // .... .......+...
    0x09, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x80, 0x0c, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, // ............+...
    0x07, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, // ............+...
    0x09, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x60, 0x09, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, // ........`.......
    0x0d, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, // .... ...+.......
    0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, // ........+.......
    0x0f, 0x00, 0x00, 0x00, 0x0a, 0xd7, 0xe3, 0xbe, 0x17, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00, // ................
    0x0d, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, // ........,.......
    0x11, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, // ............+...
    0x0d, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x2b, 0x00, 0x04, 0x00, // ...........?+...
    0x0d, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x8f, 0xc2, 0x15, 0x40, 0x2c, 0x00, 0x05, 0x00, // ...........@,...
    0x10, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, // ................
    0x2b, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, // +...............
    0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, // +..............@
    0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, // +..............?
    0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x9a, 0x99, 0x99, 0x3e, // +..............>
    0x17, 0x00, 0x04, 0x00, 0x19, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ................
    0x2c, 0x00, 0x06, 0x00, 0x19, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, // ,...............
    0x18, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, // ........+.......
    0x1b, 0x00, 0x00, 0x00, 0xcd, 0xcc, 0x4c, 0xbe, 0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, // ......L.+.......
    0x1c, 0x00, 0x00, 0x00, 0x9a, 0x99, 0x99, 0xbe, 0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, // ........+.......
    0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x2c, 0x00, 0x06, 0x00, 0x19, 0x00, 0x00, 0x00, // ........,.......
    0x1e, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, // ................
    0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, // +...........ff.@
    0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, // +....... .....@@
    0x2c, 0x00, 0x06, 0x00, 0x19, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, // ,.......!.......
    0x16, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, // .... ...+.......
    0x22, 0x00, 0x00, 0x00, 0xcd, 0xcc, 0xcc, 0x3d, 0x2c, 0x00, 0x06, 0x00, 0x19, 0x00, 0x00, 0x00, // "......=,.......
    0x23, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, // #.......".......
    0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0xd0, 0x0f, 0xc9, 0x40, // +.......$......@
    0x2b, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, // +.......%.......
    0x2b, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // +.......&.......
    0x2b, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // +.......'.......
    0x2b, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // +.......(.......
    0x2b, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // +.......).......
    0x1d, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x03, 0x00, // ................
    0x04, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x2a, 0x00, 0x00, 0x00, // ........ ...*...
    0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x2b, 0x00, 0x00, 0x00, // ............+...
    0x09, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x2c, 0x00, 0x00, 0x00, // ........ ...,...
    0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, 0x00, 0x2d, 0x00, 0x00, 0x00, // ....+.......-...
    0x21, 0x00, 0x03, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, // !.......-.......
    0x2f, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x14, 0x00, 0x02, 0x00, // /...............
    0x30, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x31, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, // 0.......1.......
    0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x32, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // .... ...2.......
    0x09, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // ....;...*.......
    0x02, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ....;...,.......
    0x01, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, // ....,.......3...
    0x12, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, // ........+.......
    0x34, 0x00, 0x00, 0x00, 0x0a, 0xd7, 0xa3, 0x39, 0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, // 4......9+.......
    0x35, 0x00, 0x00, 0x00, 0x0e, 0x74, 0xda, 0x39, 0x2b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, // 5....t.9+.......
    0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x36, 0x00, 0x05, 0x00, 0x2d, 0x00, 0x00, 0x00, // 6......<6...-...
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, // ................
    0x37, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, // 7...=...+...8...
    0x03, 0x00, 0x00, 0x00, 0xf7, 0x00, 0x03, 0x00, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........9.......
    0xfb, 0x00, 0x03, 0x00, 0x26, 0x00, 0x00, 0x00, 0x3a, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, // ....&...:.......
    0x3a, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, // :...Q.......;...
    0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xae, 0x00, 0x05, 0x00, 0x30, 0x00, 0x00, 0x00, // 8...........0...
    0x3c, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, // <...;.......Q...
    0x09, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ....=...8.......
    0xae, 0x00, 0x05, 0x00, 0x30, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, // ....0...>...=...
    0x0c, 0x00, 0x00, 0x00, 0xa6, 0x00, 0x05, 0x00, 0x30, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, // ........0...?...
    0x3c, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0xf7, 0x00, 0x03, 0x00, 0x40, 0x00, 0x00, 0x00, // <...>.......@...
    0x00, 0x00, 0x00, 0x00, 0xfa, 0x00, 0x04, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, // ........?...A...
    0x40, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x41, 0x00, 0x00, 0x00, 0xf9, 0x00, 0x02, 0x00, // @.......A.......
    0x39, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x40, 0x00, 0x00, 0x00, 0x70, 0x00, 0x04, 0x00, // 9.......@...p...
    0x0d, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x85, 0x00, 0x05, 0x00, // ....B...;.......
    0x0d, 0x00, 0x00, 0x00, 0x43, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, // ....C...B...4...
    0x70, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, // p.......D...=...
    0x85, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, // ........E...D...
    0x35, 0x00, 0x00, 0x00, 0x50, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, // 5...P.......F...
    0x43, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x00, 0x83, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, // C...E...........
    0x47, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x8e, 0x00, 0x05, 0x00, // G...F...3.......
    0x10, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x47, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, // ....H...G.......
    0x81, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, 0x49, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, // ........I.......
    0x48, 0x00, 0x00, 0x00, 0xf9, 0x00, 0x02, 0x00, 0x4a, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, // H.......J.......
    0x4a, 0x00, 0x00, 0x00, 0xf5, 0x00, 0x07, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x4b, 0x00, 0x00, 0x00, // J...........K...
    0x0e, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00, // ....@...L...M...
    0xf5, 0x00, 0x07, 0x00, 0x10, 0x00, 0x00, 0x00, 0x4e, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, // ........N.......
    0x40, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00, 0xf5, 0x00, 0x07, 0x00, // @...O...M.......
    0x07, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, // ....P.......@...
    0x51, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00, 0xb1, 0x00, 0x05, 0x00, 0x30, 0x00, 0x00, 0x00, // Q...M.......0...
    0x52, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0xf6, 0x00, 0x04, 0x00, // R...P...........
    0x53, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x00, 0x04, 0x00, // S...M...........
    0x52, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x53, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, // R...T...S.......
    0x54, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, // T...Q.......U...
    0x4e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00, // N...............
    0x56, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, // V...U...U...Q...
    0x0d, 0x00, 0x00, 0x00, 0x57, 0x00, 0x00, 0x00, 0x4e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ....W...N.......
    0x85, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x57, 0x00, 0x00, 0x00, // ........X...W...
    0x57, 0x00, 0x00, 0x00, 0x83, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, // W...........Y...
    0x56, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x85, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00, // V...X...........
    0x5a, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x85, 0x00, 0x05, 0x00, // Z.......U.......
    0x0d, 0x00, 0x00, 0x00, 0x5b, 0x00, 0x00, 0x00, 0x5a, 0x00, 0x00, 0x00, 0x57, 0x00, 0x00, 0x00, // ....[...Z...W...
    0x50, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, 0x5c, 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, // P.......\...Y...
    0x5b, 0x00, 0x00, 0x00, 0x81, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, // [...........O...
    0x5c, 0x00, 0x00, 0x00, 0x49, 0x00, 0x00, 0x00, 0x94, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00, // \...I...........
    0x5d, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0xba, 0x00, 0x05, 0x00, // ]...O...O.......
    0x30, 0x00, 0x00, 0x00, 0x5e, 0x00, 0x00, 0x00, 0x5d, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, // 0...^...].......
    0xf7, 0x00, 0x03, 0x00, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x00, 0x04, 0x00, // ...._...........
    0x5e, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, // ^...`..._.......
    0x60, 0x00, 0x00, 0x00, 0xf9, 0x00, 0x02, 0x00, 0x53, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, // `.......S.......
    0x5f, 0x00, 0x00, 0x00, 0x81, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00, // _...........L...
    0x4b, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0xf9, 0x00, 0x02, 0x00, 0x4d, 0x00, 0x00, 0x00, // K...........M...
    0xf8, 0x00, 0x02, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x80, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, // ....M...........
    0x51, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0xf9, 0x00, 0x02, 0x00, // Q...P...........
    0x4a, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x53, 0x00, 0x00, 0x00, 0x85, 0x00, 0x05, 0x00, // J.......S.......
    0x0d, 0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x00, 0x4b, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, // ....a...K...6...
    0x8e, 0x00, 0x05, 0x00, 0x19, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, // ........b...!...
    0x61, 0x00, 0x00, 0x00, 0x81, 0x00, 0x05, 0x00, 0x19, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, // a...........c...
    0x62, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x8e, 0x00, 0x05, 0x00, 0x19, 0x00, 0x00, 0x00, // b...#...........
    0x64, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x06, 0x00, // d...c...$.......
    0x19, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, // ....e...........
    0x64, 0x00, 0x00, 0x00, 0x85, 0x00, 0x05, 0x00, 0x19, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, // d...........f...
    0x1e, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x81, 0x00, 0x05, 0x00, 0x19, 0x00, 0x00, 0x00, // ....e...........
    0x67, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, // g.......f...Q...
    0x0d, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ....h...g.......
    0x51, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x69, 0x00, 0x00, 0x00, 0x67, 0x00, 0x00, 0x00, // Q.......i...g...
    0x01, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x6a, 0x00, 0x00, 0x00, // ....Q.......j...
    0x67, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x50, 0x00, 0x07, 0x00, 0x2f, 0x00, 0x00, 0x00, // g.......P.../...
    0x6b, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x69, 0x00, 0x00, 0x00, 0x6a, 0x00, 0x00, 0x00, // k...h...i...j...
    0x17, 0x00, 0x00, 0x00, 0x84, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, // ............l...
    0x0a, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x80, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, // ....=...........
    0x6d, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x84, 0x00, 0x05, 0x00, // m...l...;.......
    0x09, 0x00, 0x00, 0x00, 0x6e, 0x00, 0x00, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, // ....n...m...%...
    0xc2, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, 0x6f, 0x00, 0x00, 0x00, 0x6e, 0x00, 0x00, 0x00, // ........o...n...
    0x27, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x04, 0x00, 0x31, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, // '...|...1...p...
    0x6b, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, 0x71, 0x00, 0x00, 0x00, // k...Q.......q...
    0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x06, 0x00, 0x32, 0x00, 0x00, 0x00, // p.......A...2...
    0x72, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x6f, 0x00, 0x00, 0x00, // r.......&...o...
    0x3e, 0x00, 0x03, 0x00, 0x72, 0x00, 0x00, 0x00, 0x71, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, // >...r...q...Q...
    0x09, 0x00, 0x00, 0x00, 0x73, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ....s...p.......
    0x80, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 0x6f, 0x00, 0x00, 0x00, // ........t...o...
    0x28, 0x00, 0x00, 0x00, 0x41, 0x00, 0x06, 0x00, 0x32, 0x00, 0x00, 0x00, 0x75, 0x00, 0x00, 0x00, // (...A...2...u...
    0x05, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, // ....&...t...>...
    0x75, 0x00, 0x00, 0x00, 0x73, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, // u...s...Q.......
    0x76, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x80, 0x00, 0x05, 0x00, // v...p...........
    0x09, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00, 0x6f, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, // ....w...o...'...
    0x41, 0x00, 0x06, 0x00, 0x32, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // A...2...x.......
    0x26, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x78, 0x00, 0x00, 0x00, // &...w...>...x...
    0x76, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, 0x79, 0x00, 0x00, 0x00, // v...Q.......y...
    0x70, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x80, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, // p...............
    0x7a, 0x00, 0x00, 0x00, 0x6f, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0x41, 0x00, 0x06, 0x00, // z...o...)...A...
    0x32, 0x00, 0x00, 0x00, 0x7b, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, // 2...{.......&...
    0x7a, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x7b, 0x00, 0x00, 0x00, 0x79, 0x00, 0x00, 0x00, // z...>...{...y...
    0xf9, 0x00, 0x02, 0x00, 0x39, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x39, 0x00, 0x00, 0x00, // ....9.......9...
    0xfd, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00                                                  // ....8...
};

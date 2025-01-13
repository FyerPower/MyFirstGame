#version 430 core

// ###############################################
//                     Structs
// ###############################################

struct Transform {
    vec2 pos;
    vec2 size;
    ivec2 atlasOffset;
    ivec2 spriteSize;
};

/*
 *  Outputs the color of a fragment (pixel) of a quad based on the texture coordinates.
 *
 *  The quad.vert shader is a vertex shader written in GLSL (OpenGL Shading Language) that is used to process
 *  the vertices of textured quads (rectangles) in an OpenGL application. The purpose of this shader is to
 *  transform the vertices of the quads based on their positions, sizes, and other attributes, and to pass the
 *  appropriate texture coordinates to the fragment shader.
 */

// ###########################################################################################################
// Input
// ###########################################################################################################

// Defines a shader storage buffer object (SSBO) named TransformSBO that contains an array of Transform
// structs. This buffer is bound to binding point 0.
layout(std430, binding = 0) buffer TransformSBO
{
    Transform transforms[];
};

// ###########################################################################################################
// Output
// ###########################################################################################################

// Defines the output variable textureCoordsOut, which will hold the texture coordinates for the current
// vertex. This will be passed to the fragment shader.
layout(location = 0) out vec2 textureCoordsOut;
// layout(location = 1) out flat int renderOptions;
// layout(location = 2) out flat int materialIdx;

// ###########################################################################################################
// Uniforms
// ###########################################################################################################

// screenSize: A vec2 representing the size of the screen.
uniform vec2 screenSize;

// orthoProjection: A mat4 representing the orthographic projection matrix.
uniform mat4 orthoProjection;

// ###########################################################################################################
// Main Logic
// ###########################################################################################################

// OpenGL Coordinates System.  How things are drawn on the screen.
// -1, 1                1, 1
//            0, 0
// -1,-1                1,-1

void main()
{
    // Get the transform data for the current instance (quad).
    Transform transform = transforms[gl_InstanceID];

    // Calculate the texture coordinates based on the atlas offset and sprite size (pixel coordinates).
    int left = transform.atlasOffset.x;
    int top = transform.atlasOffset.y;
    int right = transform.atlasOffset.x + transform.spriteSize.x;
    int bottom = transform.atlasOffset.y + transform.spriteSize.y;

    // if (bool(transform.renderOptions & RENDERING_OPTION_FLIP_X)) {
    //     int tmp = left;
    //     left = right;
    //     right = tmp;
    // }

    // if (bool(transform.renderOptions & RENDERING_OPTION_FLIP_Y)) {
    //     int tmp = top;
    //     top = bottom;
    //     bottom = tmp;
    // }

    // Texture coordinates for the six vertices of two triangles forming a quad
    vec2 textureCoords[6] = {
        vec2(left, top),     // Top Left
        vec2(left, bottom),  // Bottom Left
        vec2(right, top),    // Top Right
        vec2(right, top),    // Top Right
        vec2(left, bottom),  // Bottom Left
        vec2(right, bottom), // Bottom Right
    };

    // Set the output texture coordinates for the current vertex.  This will be passed to the fragment shader.
    // textureCoordsOut is a built-in output variable that holds the texture coordinates for the current vertex.
    textureCoordsOut = textureCoords[gl_VertexID];

    // Define the vertices of the quad based on the position and size of the transform.
    vec2 vertices[6] = {
        transform.pos,                                     // Top Left
        vec2(transform.pos + vec2(0.0, transform.size.y)), // Bottom Left
        vec2(transform.pos + vec2(transform.size.x, 0.0)), // Top Right
        vec2(transform.pos + vec2(transform.size.x, 0.0)), // Top Right
        vec2(transform.pos + vec2(0.0, transform.size.y)), // Bottom Left
        transform.pos + transform.size                     // Bottom Right
    };

    // Calculate the final position of the vertex based on the orthographic projection.
    vec2 vertexPos = vertices[gl_VertexID];
    // Convert the 2D vertex position to a 4-component vector (homogeneous coordinates)
    vec4 vertexPosH = vec4(vertexPos, 0.0, 1.0);
    // Transform the vertex position by the orthographic projection matrix
    // gl_Position is a built-in output variable that holds the final position of the vertex.
    gl_Position = orthoProjection * vertexPosH;
}
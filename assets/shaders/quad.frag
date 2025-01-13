#version 430 core

/*
 *  Outputs the color of a fragment (pixel) of a quad based on the texture coordinates.
 * 
 *  The quad.frag shader is a fragment shader written in GLSL (OpenGL Shading Language) that is used to 
 *  render textured quads (rectangles) in an OpenGL application. The purpose of this shader is to fetch 
 *  the color from a texture atlas based on the provided texture coordinates and output the final color 
 *  for each fragment (pixel) of the quad. Additionally, it discards fragments with zero alpha, making 
 *  them fully transparent.
 */

// ###########################################################################################################
// Inputs
//
// Defines the input variable textureCoordsIn, which contains the texture coordinates for the current fragment. 
// This is typically passed from the vertex shader.
// ###########################################################################################################

layout(location = 0) in vec2 textureCoordsIn;
// layout(location = 1) in flat int renderOptions;
// layout(location = 2) in flat int materialIndex;

// ###########################################################################################################
// Output
//
// Defines the output variable fragColor, which will hold the final color of the fragment.
// ###########################################################################################################

layout(location = 0) out vec4 fragColor;

// ###########################################################################################################
// Bindings
//
// Defines a uniform sampler textureAtlas that represents the texture atlas bound to texture unit 0. This is 
// used to fetch the texture color.
// ###########################################################################################################

// binding = 0 binds to GL_TEXTURE0, binding = 1 binds to GL_TEXTURE1, etc.
layout(binding = 0) uniform sampler2D textureAtlas;
// layout(binding = 1) uniform sampler2D fontAtlas;

// ###########################################################################################################
// Buffers
// ###########################################################################################################

// layout(std430, binding = 1) buffer Materials
// {
//   Material materials[];
// };

// ###########################################################################################################
// Main Logic
// ###########################################################################################################

void main()
{
    // Fetches the color from the texture atlas at the specified integer texture coordinates.
    vec4 textureColor = texelFetch(textureAtlas, ivec2(textureCoordsIn), 0);

    // Discards the fragment if its alpha value is zero, making it fully transparent.
    if(textureColor.a == 0.0) {
        discard;
    }

    // Sets the output color of the fragment to the fetched texture color.
    fragColor = textureColor;
}
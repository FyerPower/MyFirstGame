//
//  Outputs the color of a fragment (pixel) of a quad based on the texture coordinates.
// 
//  The quad.frag shader is a fragment shader written in GLSL (OpenGL Shading Language) that is used to 
//  render textured quads (rectangles) in an OpenGL application. The purpose of this shader is to fetch 
//  the color from a texture atlas based on the provided texture coordinates and output the final color 
//  for each fragment (pixel) of the quad. Additionally, it discards fragments with zero alpha, making 
//  them fully transparent.
//

// ###########################################################################################################
// Inputs
//
// Defines the input variable textureCoordsIn, which contains the texture coordinates for the current fragment. 
// This is typically passed from the vertex shader.
// ###########################################################################################################

layout(location = 0) in vec2 textureCoordsIn;
layout(location = 1) in flat int transformTypeIn;
layout(location = 2) in flat vec2 transformSizeIn;
layout(location = 3) in flat int materialIndexIn;

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

layout(std430, binding = 1) buffer Materials
{
  Material materials[];
};

// ###########################################################################################################
// Main Logic
// ###########################################################################################################

// Defines the transform types for sprites
void determine_sprite_color() {
    // Fetches the color from the texture atlas at the specified integer texture coordinates.
    vec4 textureColor = texelFetch(textureAtlas, ivec2(textureCoordsIn), 0);

    // Discards the fragment if its alpha value is zero, making it fully transparent.
    if(textureColor.a == 0.0) {
        discard;
    }

    // Sets the output color of the fragment to the fetched texture color.
    fragColor = textureColor;
}

// Defines the transform types for outlines
void determine_outline_color() {
    float left = textureCoordsIn.x;
    float right = transformSizeIn.x - textureCoordsIn.x;
    float top = textureCoordsIn.y;
    float bottom = transformSizeIn.y - textureCoordsIn.y;

    // // Dashed Outline
    // if(left < 1.0 || right < 1.0) {
    //     if(mod(textureCoordsIn.y, 5.0) < 3.0) {
    //         fragColor = (materialIndexIn >= 0) ? materials[materialIndexIn].color : vec4(1, 1, 1, 0.5);
    //     } else {
    //         discard;
    //     }
    // } else if(top < 1.0 || bottom < 1.0) {
    //     if(mod(textureCoordsIn.x, 4.0) < 2.0) {
    //         fragColor = (materialIndexIn >= 0) ? materials[materialIndexIn].color : vec4(1, 1, 1, 0.5);
    //     } else {
    //         discard;
    //     }
    // } else {
    //     discard;
    // }

    // Determine the minimum distance to the edge
    float minDistance = min(left, min(right, min(top, bottom)));

    // Set the border color if within a certain distance from the edge
    if (minDistance < 3.0) {
        fragColor = vec4(1, 1, 0, 1 - minDistance / 3.0);
    } else {
        discard;
    }
}

// Defines the transform types for outlines
void determine_fill_color() {
    float left = textureCoordsIn.x;
    float right = transformSizeIn.x - textureCoordsIn.x;
    float top = textureCoordsIn.y;
    float bottom = transformSizeIn.y - textureCoordsIn.y;

    vec4 materialColor = (materialIndexIn >= 0) ? materials[materialIndexIn].color : vec4(1, 1, 1, 0.5);

    float minDistance = min(left, min(right, min(top, bottom)));
    if(int(left) == int(floor(top)) || int(floor(left) + 4) == int(floor(top)) || int(floor(left) + 8) == int(floor(top)) || int(floor(left) + 12) == int(floor(top)) || int(floor(left) - 4) == int(floor(top)) || int(floor(left) - 8) == int(floor(top)) || int(floor(left) - 12) == int(floor(top))) {
        materialColor.a = 0.7;
        fragColor = materialColor;
    } else {
        materialColor.a = 0.4;
        fragColor = materialColor;
    }
}

void main()
{
    // If the transform is a sprite...
    if (bool(transformTypeIn & TRANSFORM_TYPE_SPRITE)) {
        determine_sprite_color();
        return;
    } 
    // If the transform is an outline...
    else if (bool(transformTypeIn & TRANSFORM_TYPE_OUTLINE)) {
        determine_outline_color();
        return;
    }
    // If the transform is an fill...
    else if (bool(transformTypeIn & TRANSFORM_TYPE_FILL)) {
        determine_fill_color();
        return;
    }

    discard;
}
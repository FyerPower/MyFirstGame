//
//  Outputs the color of a fragment (pixel) of a quad based on the texture coordinates.
//
//  The quad.vert shader is a vertex shader written in GLSL (OpenGL Shading Language) that is used to process
//  the vertices of textured quads (rectangles) in an OpenGL application. The purpose of this shader is to
//  transform the vertices of the quads based on their positions, sizes, and other attributes, and to pass the
//  appropriate texture coordinates to the fragment shader.
//

// ###########################################################################################################
// Input
// ###########################################################################################################

// Defines a shader storage buffer object (SSBO) named TransformSBO that contains an array of Transform
// structs. This buffer is bound to binding point 0.
layout(binding = 0, std430) readonly buffer TransformSBO
{
    Transform transforms[];
};

// ###########################################################################################################
// Output
// ###########################################################################################################

// Defines the output variable textureCoordsOut, which will hold the texture coordinates for the current
// vertex. This will be passed to the fragment shader.
layout(location = 0) out vec2 textureCoordsOut;
layout(location = 1) out flat int transformTypeOut;
layout(location = 2) out flat vec2 transformSizeOut;
layout(location = 3) out flat int materialIndexOut;

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

void calculate_gl_position(Transform transform)
{
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

void generate_texture_coordinates(Transform transform)
{
    // Calculate the texture coordinates based on the atlas offset and sprite size (pixel coordinates).
    int left = transform.spriteOffset.x;
    int top = transform.spriteOffset.y;
    int right = transform.spriteOffset.x + transform.spriteSize.x;
    int bottom = transform.spriteOffset.y + transform.spriteSize.y;

    // Flip X?
    if (bool(transform.renderOptions & RENDERING_OPTION_FLIP_X)) {
        int tmp = left;
        left = right;
        right = tmp;
    }

    // Flip Y?
    if (bool(transform.renderOptions & RENDERING_OPTION_FLIP_Y)) {
        int tmp = top;
        top = bottom;
        bottom = tmp;
    }

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
}

void generate_outline_coordinates(Transform transform)
{
    // Texture coordinates for the six vertices of two triangles forming a quad
    vec2 textureCoords[6] = {
        vec2(0, 0),                               // Top Left
        vec2(0, transform.size.y),                // Bottom Left
        vec2(transform.size.x, 0),                // Top Right
        vec2(transform.size.x, 0),                // Top Right
        vec2(0, transform.size.y),                // Bottom Left
        vec2(transform.size.x, transform.size.y), // Bottom Right
    };
    textureCoordsOut = textureCoords[gl_VertexID];
}

void main()
{
    // Get the transform data for the current instance (quad).
    Transform transform = transforms[gl_InstanceID];
    transformTypeOut = transform.transformType;
    transformSizeOut = transform.size;
    materialIndexOut = transform.materialIndex;

    // Calculate the final position of the vertex based on the orthographic projection.
    calculate_gl_position(transform);

    // If the transform is a texture... generate the texture coordinates.
    if (bool(transform.transformType & TRANSFORM_TYPE_SPRITE)) {
        generate_texture_coordinates(transform);
    }
    // If the transform is a texture... generate the texture coordinates.
    else if (bool(transform.transformType & TRANSFORM_TYPE_OUTLINE)) {
        generate_outline_coordinates(transform);
    }
    // If the transform is a texture... generate the texture coordinates.
    else if (bool(transform.transformType & TRANSFORM_TYPE_FILL)) {
        generate_outline_coordinates(transform);
    }
}
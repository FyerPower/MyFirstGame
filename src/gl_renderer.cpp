#pragma once

// ###############################################
// #tag Includes
// ###############################################

#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#include "libs/libs.hpp"
#include "input.hpp"
#include "gl_renderer.hpp"
#include "render_interface.hpp"

// To Load PNG Files
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ###############################################
// #tag Constants
// ###############################################

const char* TEXTURE_PATH = "assets/textures/TileSet.png";

// ###############################################
// #tag Structs
// ###############################################

struct GLContext {
    GLuint vaoID;             // Vertex Array Object
    GLuint programID;         // 1
    GLuint textureID;         // 2
    GLuint transformSBOID;    // Transform Storage Buffer Object
    GLuint screenSizeID;      // The location of the uniform variable "screenSize"
    GLuint orthoProjectionID; // The location of the uniform variable "orthoProjection"
    GLuint materialSBOID;     // Material Storage Buffer Object

    long long textureTimestamp;
    long long shaderTimestamp;
};

// ###############################################
// #tag Globals
// ###############################################

static GLContext glContext;

// ###############################################
// #tag Functions
// ###############################################

static void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user)
{
    if (severity == GL_DEBUG_SEVERITY_LOW || severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH) {
        FP_ASSERT(false, "OpenGL Error: %s", message);
    } else {
        FP_LOG((char*)message);
    }
}

GLuint gl_create_shaders(GLenum shaderType, const char* shaderPath, BumpAllocator* transientStorage)
{
    int fileSize = 0;
    GLuint shaderId = glCreateShader(shaderType);
    char* shader = read_file(shaderPath, &fileSize, transientStorage);
    if (!shader) {
        FP_ASSERT(false, "Failed to load shader: %s", shaderPath);
        return 0;
    }
    // char* shaderHeader = read_file("src/shaders/shared_header.hpp", &fileSize, transientStorage);
    // if (!shaderHeader) {
    //     FP_ASSERT(false, "Failed to load shader header");
    //     return 0;
    // }
    const char* shaderSources[] = {shader};
    glShaderSource(shaderId, ArraySize(shaderSources), shaderSources, 0);
    glShaderSource(shaderId, 1, &shader, 0);
    glCompileShader(shaderId);

    // Test if shaders compiled successfully
    {
        int success;
        char shaderLog[2048] = {};
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderId, 2048, 0, shaderLog);
            FP_ASSERT(false, "Failed to compile Shaders %s", shaderLog);
            return 0;
        }
    }

    return shaderId;
};

bool has_updated_shaders()
{
    long long timestampVert = get_timestamp("assets/shaders/quad.vert");
    long long timestampFrag = get_timestamp("assets/shaders/quad.frag");
    return timestampVert > glContext.shaderTimestamp || timestampFrag > glContext.shaderTimestamp;
}

bool load_program_and_shaders(BumpAllocator* transientStorage)
{
    FP_LOG("Loading Shaders");

    // create our shaders
    GLuint vertShaderID = gl_create_shaders(GL_VERTEX_SHADER, "assets/shaders/quad.vert", transientStorage);
    GLuint fragShaderID = gl_create_shaders(GL_FRAGMENT_SHADER, "assets/shaders/quad.frag", transientStorage);

    if (!vertShaderID || !fragShaderID) {
        FP_ASSERT(false, "Failed to create shaders");
        return false;
    }

    long long timestampVert = get_timestamp("assets/shaders/quad.vert");
    long long timestampFrag = get_timestamp("assets/shaders/quad.frag");
    glContext.shaderTimestamp = (int)fmax(timestampVert, timestampFrag);

    // Create the OpenGL Program, attach shaders, and link to the context
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertShaderID);
    glAttachShader(programID, fragShaderID);
    glLinkProgram(programID);

    // Because the shaders are now attached to the program, we can clean them up and detach / delete them.
    glDetachShader(programID, vertShaderID);
    glDetachShader(programID, fragShaderID);
    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);

    // Validate if program works
    int programSuccess;
    char programInfoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &programSuccess);
    if (!programSuccess) {
        glGetProgramInfoLog(programID, 512, 0, programInfoLog);
        FP_ASSERT(false, "Failed to link program: %s", programInfoLog);
        return false;
    }

    // Delete the old program and use the new one
    glDeleteProgram(glContext.programID);
    glContext.programID = programID;

    glUseProgram(programID);

    return true;
}

bool has_updated_textures()
{
    long long currentTimestamp = get_timestamp(TEXTURE_PATH);
    return currentTimestamp > glContext.textureTimestamp;
}

bool load_textures()
{
    FP_LOG("Loading Textures");

    int width, height, channels;
    unsigned char* data = stbi_load(TEXTURE_PATH, &width, &height, &channels, STBI_rgb_alpha);

    if (!data) {
        FP_ASSERT(false, "Failed to Load Image");
        return false;
    }

    // Load Texture into GPU
    glActiveTexture(GL_TEXTURE0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Free the Image from memory
    stbi_image_free(data);

    // Update our timestamp
    glContext.textureTimestamp = get_timestamp(TEXTURE_PATH);

    FP_LOG("Successfully Loaded Textures(%s) Width(%d) Height(%d) Channels(%d)", TEXTURE_PATH, width, height, channels);
    return true;
}

// ###############################################
// #tag Init_and_Render
// ###############################################

bool gl_init(BumpAllocator* transientStorage)
{
    // Load all OpenGL Functions
    load_gl_functions();

    // Register callback method that will handle debug messaging within OpenGL
    glDebugMessageCallback(&gl_debug_callback, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);

    if (!load_program_and_shaders(transientStorage)) {
        return false;
    }

    // Create Vertex Array Object - This is a container for our Vertex Buffer State Objects that supply data to the shaders.
    glGenVertexArrays(1, &glContext.vaoID);
    glBindVertexArray(glContext.vaoID);

    // Texture Loading with STBImage
    {
        // Generate the texture and store the result into our glContext.textureID
        glGenTextures(1, &glContext.textureID);
        // Set our texture as Active (Slot 0 - refers to the shaders "location=0")
        glActiveTexture(GL_TEXTURE0);
        // Bind the Texture
        glBindTexture(GL_TEXTURE_2D, glContext.textureID);

        // Set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the Textures
        if (!load_textures()) {
            return false;
        }
    }

    // Create Transform Storage Buffer - Generate 1 buffer, configure it, and store it in glContext.transformSBOID, then unbind it
    glGenBuffers(1, &glContext.transformSBOID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, glContext.transformSBOID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Transform) * renderData->transforms.maxElements, renderData->transforms.elements, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // // Create Materials Storage Buffer - Generate 1 buffer, configure it, and store it in glContext.materialSBOID, then unbind it
    // glGenBuffers(1, &glContext.materialSBOID);
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, glContext.materialSBOID);
    // glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Material) * renderData->materials.maxElements, renderData->materials.elements, GL_DYNAMIC_DRAW);
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Create the Uniforms - Screen Size and Orthographic Projection
    glContext.screenSizeID = glGetUniformLocation(glContext.programID, "screenSize");
    glContext.orthoProjectionID = glGetUniformLocation(glContext.programID, "orthoProjection");

    // Disable MultiSampling (aka Anti-Aliasing)
    glDisable(GL_MULTISAMPLE);

    // Allows for transparent textures to be rendered as transparent instead of solid.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Use Program
    glUseProgram(glContext.programID);

    // Success
    return true;
};

/**
 *
 */
void gl_render(BumpAllocator* transientStorage)
{
    // Hot Reloading - Textures
    if (has_updated_textures() && !load_textures()) {
        FP_ERROR("Textures have been updated and have failed to load");
        return;
    }
    if (has_updated_shaders() && !load_program_and_shaders(transientStorage)) {
        FP_ERROR("Shaders have been updated and have failed to load");
        return;
    }

    // Clear the Screen by setting the color to black
    glClearColor(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f);
    // Clear the Depth Buffer
    glClearDepth(0.0f);
    // Clear the Color and Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set the Viewport to the size of the screen
    glViewport(0, 0, input->screenSize.x, input->screenSize.y);

    // Copy screen size to the GPU
    Vec2 screenSize = {(float)input->screenSize.x, (float)input->screenSize.y};
    glUniform2fv(glContext.screenSizeID, 1, &screenSize.x);

    // // Copy Materials to the GPU
    // {
    //     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, glContext.materialSBOID);
    //     glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Material) * renderData->materials.count, renderData->materials.elements);
    //     renderData->materials.clear();
    // }

    // Game Tiles / Objects
    {
        // Game Camera - Orthographic Projection
        OrthographicCamera2D camera = renderData->gameCamera;
        Vec2 zoomedDimensions = camera.getZoomedDimensions();
        Mat4 orthoProjection = Geometry::orthographic_projection(camera.position.x - zoomedDimensions.x / 2.0f,  // left
                                                                 camera.position.x + zoomedDimensions.x / 2.0f,  // right
                                                                 camera.position.y - zoomedDimensions.y / 2.0f,  // top
                                                                 camera.position.y + zoomedDimensions.y / 2.0f); // bottom
        glUniformMatrix4fv(glContext.orthoProjectionID, 1, GL_FALSE, &orthoProjection.ax);

        // Copy transforms to the GPU
        GLsizeiptr buffer_size = sizeof(Transform) * renderData->transforms.count;
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, glContext.transformSBOID);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, buffer_size, renderData->transforms.elements);

        // Draw the quads (2x triangles) totally 6 indices per quad, render all transforms (instances)
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderData->transforms.count);

        // Reset for next Frame
        renderData->transforms.clear();
    }
}
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

const char* TEXTURE_PATH = "assets/textures/atlas.png";

// ###############################################
// #tag Structs
// ###############################################

struct GLContext {
    GLuint programID;         // 1
    GLuint textureID;         // 2
    GLuint transformSBOID;    // Transform Storage Buffer Object
    GLuint screenSizeID;      // The location of the uniform variable "screenSize"
    GLuint orthoProjectionID; // The location of the uniform variable "orthoProjection"

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

GLuint gl_create_shaders(GLenum shaderType, char* shaderPath, BumpAllocator* transientStorage)
{
    int fileSize = 0;
    GLuint shaderId = glCreateShader(shaderType);
    char* shader = read_file(shaderPath, &fileSize, transientStorage);
    if (!shader) {
        FP_ASSERT(false, "Failed to load shader: %s", shaderPath);
        return 0;
    }
    glShaderSource(shaderId, 1, &shader, 0);
    glCompileShader(shaderId);
    // Test if Vertext Shader compiled successfully
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

bool load_shaders(BumpAllocator* transientStorage)
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
    glContext.programID = glCreateProgram();
    glAttachShader(glContext.programID, vertShaderID);
    glAttachShader(glContext.programID, fragShaderID);
    glLinkProgram(glContext.programID);

    // Because the shaders are now attached to the program, we can clean them up and detach / delete them.
    glDetachShader(glContext.programID, vertShaderID);
    glDetachShader(glContext.programID, fragShaderID);
    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);

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
    unsigned char* data = stbi_load(TEXTURE_PATH,  // Filename,
                                    &width,        // X Position
                                    &height,       // Y Position
                                    &channels,     // Comp (Number of channels)
                                    STBI_rgb_alpha // Req Comp (Number of channels required)
    );

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

    if (!load_shaders(transientStorage)) {
        return false;
    }

    // This has to be done, otherwise OpenGL will not draw anything
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

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

        if (!load_textures()) {
            return false;
        }
    }

    // Transform Storage Buffer
    {
        // Generate (1) buffer and store it within (glContext.transformSBOID)
        glGenBuffers(1, &glContext.transformSBOID);
        // Bind (Shader Storage Buffer) at binding (0) to (glContext.transformSBOID)
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, glContext.transformSBOID);
        // Calculate the max size of our buffer based on its contents
        GLsizeiptr max_buffer_size = sizeof(Transform) * renderData->transforms.maxElements;
        // Create and Initialize our (Shader Storage Buffer) with size (max_buffer_size) using data
        // (renderData.transforms) for use with (drawing)
        glBufferData(GL_SHADER_STORAGE_BUFFER, max_buffer_size, renderData->transforms.elements, GL_DYNAMIC_DRAW);
    }

    // Uniforms
    {
        glContext.screenSizeID = glGetUniformLocation(glContext.programID, "screenSize");
        glContext.orthoProjectionID = glGetUniformLocation(glContext.programID, "orthoProjection");
    }

    // Disable MultiSampling (aka Anti-Aliasing)
    glDisable(GL_MULTISAMPLE);

    // Depth Tesing
    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_GREATER);

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
    if (has_updated_textures()) {
        Sleep(100);
        if (!load_textures()) {
            return;
        }
    }
    if (has_updated_shaders() && !load_shaders(transientStorage)) {
        return;
    }

    glClearColor(119.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 1.0f);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, input->screenSize.x, input->screenSize.y);

    // Copy screen size to the GPU
    Vec2 screenSize = {(float)input->screenSize.x, (float)input->screenSize.y};
    glUniform2fv(glContext.screenSizeID, 1, &screenSize.x);

    // Orthographic Projection
    OrthographicCamera2D camera = renderData->gameCamera;
    Vec2 zoomedDimensions = camera.getZoomedDimensions();
    Mat4 orthoProjection = Geometry::orthographic_projection(camera.position.x - zoomedDimensions.x / 2.0f,  // left
                                                             camera.position.x + zoomedDimensions.x / 2.0f,  // right
                                                             camera.position.y - zoomedDimensions.y / 2.0f,  // top
                                                             camera.position.y + zoomedDimensions.y / 2.0f); // bottom

    glUniformMatrix4fv(glContext.orthoProjectionID, 1, GL_FALSE, &orthoProjection.ax);

    // Opaque Objects
    {
        // Copy transforms to the GPU
        GLsizeiptr buffer_size = sizeof(Transform) * renderData->transforms.count;
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, buffer_size, renderData->transforms.elements);

        //
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderData->transforms.count);

        // FP_LOG("Rendering: %i items", renderData->transforms.count);

        // Reset for next Frame
        renderData->transforms.clear();
    }
}
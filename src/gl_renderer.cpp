#pragma once

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
//                     Constants
// ###############################################

const char* TEXTURE_PATH = "assets/textures/player/player.png";

// ###############################################
//                     Structs
// ###############################################

struct GLContext {
    GLuint programID;         //
    GLuint textureID;         //
    GLuint transformSBOID;    // Transform Storage Buffer Object
    GLuint screenSizeID;      // The location of the uniform variable "screenSize"
    GLuint orthoProjectionID; // The location of the uniform variable "orthoProjection"
};

// ###############################################
//                     Globals
// ###############################################

static GLContext glContext;

// ###############################################
//                     Functions
// ###############################################

static void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                       const GLchar* message, const void* user)
{
    if (severity == GL_DEBUG_SEVERITY_LOW || severity == GL_DEBUG_SEVERITY_MEDIUM ||
        severity == GL_DEBUG_SEVERITY_HIGH) {
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
        FP_ASSERT(false, "Failed to load shaders.");
        return false;
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
        }
    }

    return shaderId;
};

bool gl_init(BumpAllocator* transientStorage)
{
    // Load all OpenGL Functions
    load_gl_functions();

    // Register callback method that will handle debug messaging within OpenGL
    glDebugMessageCallback(&gl_debug_callback, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);

    // create our shaders
    GLuint vertShaderID = gl_create_shaders(GL_VERTEX_SHADER, "assets/shaders/quad.vert", transientStorage);
    GLuint fragShaderID = gl_create_shaders(GL_FRAGMENT_SHADER, "assets/shaders/quad.frag", transientStorage);

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

    // This has to be done, otherwise OpenGL will not draw anything
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Texture Loading with STBImage
    {
        int width, height, channels;
        char* data = (char*)stbi_load(TEXTURE_PATH, // Filename,
                                      &width,       // X Position
                                      &height,      // Y Position
                                      &channels,    // Comp (Number of channels)
                                      4             // Req Comp (Number of channels required)
        );

        if (!data) {
            FP_ASSERT(false, "Failed to Load Image")
            return false;
        }

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

        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    // Transform Storage Buffer
    {
        // Generate (1) buffer and store it within (glContext.transformSBOID)
        glGenBuffers(1, &glContext.transformSBOID);
        // Bind (Shader Storage Buffer) at binding (0) to (glContext.transformSBOID)
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, glContext.transformSBOID);
        // Calculate the max size of our buffer based on its contents
        GLsizeiptr max_buffer_size = sizeof(Transform) * MAX_TRANSFORMS;
        // Create and Initialize our (Shader Storage Buffer) with size (max_buffer_size) using data
        // (renderData.transforms) for use with (drawing)
        glBufferData(GL_SHADER_STORAGE_BUFFER, max_buffer_size, renderData->transforms, GL_DYNAMIC_DRAW);
    }

    // Uniforms
    {
        glContext.screenSizeID = glGetUniformLocation(glContext.programID, "screenSize");
        glContext.orthoProjectionID = glGetUniformLocation(glContext.programID, "orthoProjection");
    }

    // sRGB output (even if input texture is non-sRGB -> don't rely on texture used)
    // Your font is not using sRGB, for example (not that it matters there, because no actual color is sampled from it)
    // But this could prevent some future bug when you start mixing different types of textures
    // Of course, you still need to correctly set the image file source format when using glTexImage2D()
    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(GL_MULTISAMPLE);

    // Depth Tesing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);

    // Use Program
    glUseProgram(glContext.programID);

    // Success
    return true;
};

/**
 *
 */
void gl_render()
{
    glClearColor(119.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 1.0f);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, input->screenSizeX, input->screenSizeY);

    // Copy screen size to the GPU
    Vec2 screenSize = {(float)input->screenSizeX, (float)input->screenSizeY};
    glUniform2fv(glContext.screenSizeID, 1, &screenSize.x);

    // Orthographic Projection
    OrthographicCamera2D camera = renderData->gameCamera;
    Mat4 orthoProjection = orthographic_projection(camera.position.x - camera.dimensions.x / 2.0f,  // left
                                                   camera.position.x + camera.dimensions.x / 2.0f,  // right
                                                   camera.position.y - camera.dimensions.y / 2.0f,  // top
                                                   camera.position.y + camera.dimensions.y / 2.0f); // bottom
    glUniformMatrix4fv(glContext.orthoProjectionID, 1, GL_FALSE, &orthoProjection.ax);

    // Opaque Objects
    {
        // Copy transforms to the GPU
        GLsizeiptr buffer_size = sizeof(Transform) * renderData->transformCount;
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, buffer_size, renderData->transforms);

        //
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderData->transformCount);

        FP_LOG("Rendering: %i items", renderData->transformCount);

        // Reset for next Frame
        renderData->transformCount = 0;
    }
}
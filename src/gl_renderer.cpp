#pragma once

#include "gl_renderer.hpp"
#include "libs/bump_allocator.hpp"
#include "libs/file_io.hpp"

// ###############################################
//                     Structs
// ###############################################
struct GLContext {
    GLuint programID;
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
            FP_ASSERT(false, "Failed to compile Vertex Shaders %s", shaderLog);
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

    // Depth Tesing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);

    // Use Program
    glUseProgram(glContext.programID);

    // Success
    return true;
};

/**
 * R
 */
void gl_render()
{
    glClearColor(119.0f / 255.0f, 33.0f / 255.0f, 111.0f / 255.0f, 1.0f);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, input.screenSizeX, input.screenSizeY);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
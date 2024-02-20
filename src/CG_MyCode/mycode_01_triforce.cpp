#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "..\common\debugging.h"
 

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 1000, "indexed", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewInit();

    printout_opengl_glsl_info();
    GLuint va;
    glGenVertexArrays(1, &va);
    glBindVertexArray(va);

    /* create render data in RAM */
    GLuint positionAttribIndex = 0;
    float positions[] = {   -0.5, -0.45,    // 1st vertex
                            0.0,  -0.45,    // 2nd vertex
                            0.5,  -0.45,    // 3nd vertex
                            -0.25, 0.0,     // 4th vertex
                            0.25,  0.0,     // 5th vertex
                            0.0,   0.45     // 6th vertex 
    };
    /* create a buffer for the render data in video RAM */
    GLuint positionsBuffer;
    glCreateBuffers(1, &positionsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionsBuffer);

    /* declare what data in RAM are filling the bufferin video RAM */
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(positionAttribIndex);

    /* specify the data format */
    glVertexAttribPointer(positionAttribIndex, 2, GL_FLOAT, false, 0, 0);

    GLuint colorAttribIndex = 1;
    float colors[] = {      0.5, 0.1, 0.4,  // 1st vertex
                            0.5, 0.1, 0.4,  // 2nd vertex
                            0.5, 0.1, 0.4,  // 3rd vertex
                            1.0, 0.8, 0.0,  // 4th vertex
                            1.0, 0.8, 0.0,  // 5th vertex
                            1.0, 1.0, 0.2   // 6th vertex
    };
    /* create a buffer for the render data in video RAM */
    GLuint colorsBuffer;
    glCreateBuffers(1, &colorsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorsBuffer);

    /* declare what data in RAM are filling the bufferin video RAM */
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorAttribIndex);

    /* specify the data format */
    glVertexAttribPointer(colorAttribIndex, 3, GL_FLOAT, false, 0, 0);

    GLuint indices[] = { 0,1,3,1,2,4,3,4,5 };

    GLuint indexBuffer;
    glCreateBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 9, indices, GL_STATIC_DRAW);

    /* create a vertex shader */
    std::string  vertex_shader_src = "#version 330\n \
        in vec2 aPosition;\
        in vec3 aColor;\
        out vec3 vColor;\
        void main(void)\
        {\
            gl_Position = vec4(aPosition, 0.0, 1.0);\
            vColor = aColor;\
        }";
    const GLchar* vs_source = (const GLchar*)vertex_shader_src.c_str();
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_source, NULL);
    glCompileShader(vertex_shader);
    check_shader(vertex_shader);

    /* create a fragment shader */
    std::string   fragment_shader_src = "#version 330 \n \
        out vec4 color;\
        in vec3 vColor;\
        void main(void)\
        {\
            color = vec4(vColor, 1.0);\
        }";
    const GLchar* fs_source = (const GLchar*)fragment_shader_src.c_str();
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fs_source, NULL);
    glCompileShader(fragment_shader);
    check_shader(fragment_shader);

    GLuint program_shader = glCreateProgram();
    glAttachShader(program_shader, vertex_shader);
    glAttachShader(program_shader, fragment_shader);
    glBindAttribLocation(program_shader, positionAttribIndex, "aPosition");
    glBindAttribLocation(program_shader, colorAttribIndex, "aColor");
    glLinkProgram(program_shader);
    
    validate_shader_program(program_shader);

	/* use the program shader "program_shader" */
	glUseProgram(program_shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, NULL);
        check_gl_errors(__LINE__,__FILE__);
 
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
	glUseProgram(0);

    glfwTerminate();
    return 0;
}

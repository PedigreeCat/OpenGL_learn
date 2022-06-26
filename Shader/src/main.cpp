#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>

#define LOG_SIZE (512)

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Viewport
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    // GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 子版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac OS X系统需要添加这行代码
#endif
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 窗口大小调整时回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Shader
    Shader ourShader("res/Shader.vs", "res/Shader.fs");

    float vertices[] = {
        // 位置                 // 颜色
         0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f, // 右下
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, // 左下
         0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f  // 顶部
    };

    // VAO
    /* glBindVertexArray(VAO)到glBindVertexArray(0)之间的代码
    *  为配置VBO和EBO并绑定到VAO的流程 */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // VertexAttrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // glVertexAttribPointer已经将VBO注册为了顶点属性的绑定VBO，在那之后我们可以安全的进行unbind
    // 也就是说这里解绑也没关系，VBO已经绑定到VAO了VAO只会记录VBO的绑定动作，不记录解绑动作
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 但是VAO会记录EBO的绑定和解绑操作，所以这里不能解绑EBO
    // 你可以在以后将VAO解绑，以防止其他VAO意外的修改了当前的VAO，不过这种情况很少发生
    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // deal with input
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render instruct
        ourShader.use();

        // update uniform color
        //float timeValue = glfwGetTime();
        //float greenValue = sin(timeValue) / 2.0f + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Final
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // ���汾��
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // �Ӱ汾��
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ʹ�ú���ģʽ
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac OS Xϵͳ��Ҫ������д���
#endif
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // ���ڴ�С����ʱ�ص�����
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
        // λ��                 // ��ɫ
         0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f, // ����
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, // ����
         0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f  // ����
    };

    // VAO
    /* glBindVertexArray(VAO)��glBindVertexArray(0)֮��Ĵ���
    *  Ϊ����VBO��EBO���󶨵�VAO������ */
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

    // glVertexAttribPointer�Ѿ���VBOע��Ϊ�˶������Եİ�VBO������֮�����ǿ��԰�ȫ�Ľ���unbind
    // Ҳ����˵������Ҳû��ϵ��VBO�Ѿ��󶨵�VAO��VAOֻ���¼VBO�İ󶨶���������¼�����
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // ����VAO���¼EBO�İ󶨺ͽ��������������ﲻ�ܽ��EBO
    // ��������Ժ�VAO����Է�ֹ����VAO������޸��˵�ǰ��VAO����������������ٷ���
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
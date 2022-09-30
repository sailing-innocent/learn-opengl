#include "common.h"
#include "renderapp.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TB_GL_NAMESPACE_BEGIN

RenderApp::RenderApp() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

RenderApp::~RenderApp() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

bool RenderApp::ShouldClose() {
    return glfwWindowShouldClose(window);
}

bool RenderApp::CompileVertexShader(const char* path) {
    char _vertexShaderSource[SHADER_SOURCE_BUFSZ * SHADER_SOURCE_ROWS] = {0};
    readShaderSource(path, _vertexShaderSource);
    const char* vertexShaderSource = _vertexShaderSource;
    // unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAIDED\n" << infoLog << std::endl;
    }
    return true;
}

bool RenderApp::CompileFragmentShader(const char* path) {
    char _fragmentShaderSource[SHADER_SOURCE_BUFSZ * SHADER_SOURCE_ROWS] = {0};
    readShaderSource(path, _fragmentShaderSource);
    const char* fragmentShaderSource = _fragmentShaderSource;
    // unsigned int vertexShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAIDED\n" << infoLog << std::endl;
    }
    return true;
}

bool RenderApp::LinkProgram() {
    // unsigned int shaderProgram;

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

bool RenderApp::Run() {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wareframe mode
    // set uniforms
    // SetUniforms();
    glUseProgram(shaderProgram); 
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glfwSwapBuffers(window);
    glfwPollEvents();

    return true;
}

void RenderApp::LoadTexture() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set texture wrap parameter
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("F:/images/wall_512x512.jpg", &width, &height, &nrChannels, 0);
    std::cout << "Generate Image " << width << "," << height << std::endl; 
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }


    stbi_image_free(data);
}

bool RenderApp::CreateGeometry() {
    /*
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };
    */
    /*
    float vertices[] = {
            0.5f, 0.5f, 0.0f, // top right
            -0.5f, 0.5f, 0.0f, // top left
            -0.5f, -0.5f, 0.0f, // bottom left
            0.5f, -0.5f, 0.0f, // bottom right
    };
    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
    };
    */
    /*
    float vertices[] = {
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    */
    float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // set the vertex attribute poniters
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    return true;
}

void RenderApp::SetUniforms() {
    float timeValue = (float)glfwGetTime();
    float greenValue = (sin(timeValue)/2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
}

bool RenderApp::Init() {

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Window Caption", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
        return false;
    }
    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // compile vertex shader

    // vertex shader
    const char* vertexShaderSourcePath = "G:/learnopengl/shader/v.glsl";
    CompileVertexShader(vertexShaderSourcePath);

    // fragment shader
    const char* fragmentShaderSourcePath = "G:/learnopengl/shader/f.glsl";
    CompileFragmentShader(fragmentShaderSourcePath);
    // compile program

    LinkProgram();

    // geometry
    CreateGeometry();
    
    return true;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
};

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void readShaderSource(const char* path, char* pdest)
{
    char** ppSource = (char**)malloc(SHADER_SOURCE_ROWS * sizeof(char*));
    std::ifstream fin(path, std::ifstream::in);
    if (!fin.is_open())
    {
        std::cerr << "File " << path << " Not OPEN" << std::endl;
    }
    
    int i = 0;
    char* pSource = (char* )malloc(SHADER_SOURCE_BUFSZ * sizeof(char));
    while (fin.getline(pSource, SHADER_SOURCE_BUFSZ) && i < SHADER_SOURCE_ROWS) {
        // std::cout << "fetching source: " << pSource << std::endl; 
        ppSource[i] = (char*)malloc(SHADER_SOURCE_BUFSZ * sizeof(char)); 
        strcpy_s(ppSource[i], SHADER_SOURCE_BUFSZ, pSource);
        i += 1;
    }
    free(pSource);
    int p = 0;
    for (int j = 0; j < i; j++) {
        // std::cout << j << ": " << ppSource[j] << std::endl;
        int m = 0;
        while (ppSource[j][m] != '\0' && m < SHADER_SOURCE_BUFSZ) {
            if (p < SHADER_SOURCE_ROWS * SHADER_SOURCE_BUFSZ) {
                pdest[p] = ppSource[j][m];
                p++;
            }
            m++;
        }
        pdest[p] = '\n';
        p++;
        free(ppSource[j]);
    }
    std::cout << "CURRENT FETCH SOURCE: " << pdest << std::endl; 
    free(ppSource);
}

TB_GL_NAMESPACE_END

#include "config.h"

using namespace std;

static unsigned int compileShader(unsigned int type,const string &source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id,1,&src,nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id,GL_COMPILE_STATUS, &result);
    if(!result){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id,length,&length,message);
        cout << "Failed to compile shader" << endl;
        cout << message<< endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createSahder(const string &vertexSahder,const string &fragmentSahder){
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexSahder);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentSahder);

    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
unsigned int make_shader(const string &vertex_filepath, const string &fragment_filepath);
unsigned int make_module(const string &filepath, unsigned int module_type);

int main(void)
{
    GLFWwindow* window;

    if(!glfwInit()){
        cout << "mal" << endl;
        return -1;
    }

    window = glfwCreateWindow(640,480, "Ventana",NULL,NULL);
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        return -1;
    }

    glClearColor(0.25f,0.5f,0.25f,1.0f);

    //se crea el shader
    //unsigned int shader = make_shader(
        //"../src/shaders/vertex.txt",
        //"../src/shaders/fragment.txt"
    //);
    float positions[6] = {
        -0.5f,-0.5f,
        0.0f,0.5f,
        0.5f,-0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main(){\n"
        "   gl_Position = position;\n"
        "}\n";

    string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main(){\n"
        "   color = vec4(1.0,0.0,0.0,1.0);\n"
        "}\n";
    
    unsigned int shader = createSahder(vertexShader,fragmentShader);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)){
        

        glClear(GL_COLOR_BUFFER_BIT);
        //glUseProgram(shader);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        //swap front and back buffers
        glfwSwapBuffers(window);

        //Poll for and process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}

unsigned int make_shader(const string &vertex_filepath, const string &fragment_filepath){
    vector <unsigned int> modules;
    modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

    unsigned int shader = glCreateProgram();
    for(unsigned int shaderModule : modules){
        glAttachShader(shader, shaderModule);
    }
    glLinkProgram(shader);

    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success){
        char errorLog [1024];
        glGetProgramInfoLog(shader,1024,NULL,errorLog);
        cout<< "shader linking error:\n" <<errorLog<< endl;
    }

    for(unsigned int shaderModule : modules){
        glDeleteShader(shaderModule);
    }

    return shader;
}

unsigned int make_module(const string &filepath, unsigned int module_type){
    ifstream file;
    stringstream bufferedLines;
    string line;

    file.open(filepath);
    while(getline(file,line)){
        bufferedLines << line << "\n";
    }
    string shaderSource = bufferedLines.str();
    const char* shaderSrc = shaderSource.c_str();
    bufferedLines.str("");
    file.close();

    unsigned int shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule,1,&shaderSrc,NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if(!success){
        char errorLog [1024];
        glGetShaderInfoLog(shaderModule,1024,NULL,errorLog);
        cout<< "Error en el modulo del shader:\n" <<errorLog<< endl;
    }
    return shaderModule;
}
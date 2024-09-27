#include "Shader.h"
#include "ErrorHandler.h"

Shader::Shader(const std::string& filePath)
	: m_FilePath(filePath), m_RendererID(0)
{
    //gets the shaders codes from another file
    ShaderProgramSource shaderSource = GetShader(filePath);

    //pass the source codes created to the method made
    m_RendererID = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    //uniforms are varibales that can be setted on the CPU side (C++)
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
    //uniforms are varibales that can be setted on the CPU side (C++)
    GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    //uniforms are varibales that can be setted on the CPU side (C++)
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    //uniforms are varibales that can be setted on the CPU side (C++)
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

int Shader::GetUniformLocation(const std::string& name)
{
    //fist check if the uniform location already existis in the cache
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    //gets the id of the uniform variable declared on the fragment Shader
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) 
        printf("Warning: uniform '%s doesn't exist!\n", name.c_str());
    else
        //add the new location to the cache
        m_UniformLocationCache[name] = location;
    
    return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    //create the shader on the GPU and returns its id
    unsigned int id = glCreateShader(type);

    //get the raw string to use in the next method
    const char* src = source.c_str();

    //attach the source code of the shader to the created id 
    GLCall(glShaderSource(id, 1, &src, nullptr));

    //compile the source code of the shader
    GLCall(glCompileShader(id));

    //error handling

    //get the compile status of the shader into the result variable
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    //if the result is false (0), lets see what happened
    if (result == GL_FALSE)
    {
        //get the log into a variable based on the length of the shader  
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        //some c magic to create a raw string dinamically into the stack
        char* message = (char*)alloca(length * sizeof(char));

        //get the info log based on the shader and its length to store it into the message raw string
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        //print the error log into the screen
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:" << std::endl;
        std::cout << message << std::endl;

        //deletes the shader that didn't worked out and return 0
        GLCall(glDeleteShader(id));
        return 0;
    }

    //returns the shader id on the GPU
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //a program is an object to wich shader objects can be attatched
    unsigned int program = glCreateProgram();

    //creationg and compiling of the shaders objects
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //attach the shaders to the program
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    //do program magic
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    //clear intermediate data without deleting the shaders source codes
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

//reads the shader source code file and divide the file into vertex and fragment to return into the structure
ShaderProgramSource Shader::GetShader(const std::string& filePath)
{
    //read the file from the path
    std::ifstream shaderFileStream(filePath);

    //a simplee enum to control where the code will be written
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    //variables necessary for the code
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(shaderFileStream, line))
    {
        //if found a #shader on the line, check if is vertex or fragment to store in type
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            if (type != ShaderType::NONE)
            {
                //transforms the type into int to write in the array
                ss[(int)type] << line << "\n";
            }
        }
    }

    //returns the structure as strings
    return { ss[0].str(), ss[1].str() };
}
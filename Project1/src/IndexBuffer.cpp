#include "IndexBuffer.h"
#include "ErrorHandler.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    :m_Count(count)
{
    //make sure the GLuint size is the same as used on glBufferData, it may vary from platform
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    //generate 1 buffer stored at the address of the id
    GLCall(glGenBuffers(1, &m_RendererID));

    //each buffer needs to be "selected" (bind) to be used, like a state machine
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

    //actually creates the buffer in the mode of the enum, with the size of the positions and for whats usage
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    //delete the current buffer binded to m_RendererID
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    //Bind (select) the buffer to be used
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    //Unbind (deselect) the buffer to not be used
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

#include "VertexBuffer.h"
#include "Renderer.h"
#include "ErrorHandler.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    //generate 1 buffer stored at the address of the id
    GLCall(glGenBuffers(1, &m_RendererID));

    //each buffer needs to be "selected" (bind) to be used, like a state machine
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

    //actually creates the buffer in the mode of the enum, with the size of the positions and for whats usage
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() 
{
    //delete the current buffer binded to m_RendererID
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    //Bind (select) the buffer to be used
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    //Unbind (deselect) the buffer to not be used
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

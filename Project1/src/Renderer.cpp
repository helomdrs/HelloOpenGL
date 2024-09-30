#include "Renderer.h"

#include "ErrorHandler.h"


void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
    //bind (select) the vertex array, index buffer and shader
    shader.Bind();
    vertexArray.Bind();
    indexBuffer.Bind();
    
    //The method that actually draw the buffers into the screen (nested into Error Handling)
    GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    //clear the screen each frame
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

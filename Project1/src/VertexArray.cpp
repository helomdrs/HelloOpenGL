#include "VertexArray.h"
#include "VertexBufferLayout.h"

//a Vertex Array Object (VAO) is an OpenGL Object that stores all of the satate needed to supply vertex data. 
//it stores the format of the vertex data as well as the Buffer Objects providing the vertex data arrays

VertexArray::VertexArray()
{
	//generate a (1) quantity of VAO and store its id on the address of m_RendererID
	GLCall(glGenVertexArrays(1, &m_RenderID));
}

VertexArray::~VertexArray()
{
	//delete the VAO stored on the address passed
	GLCall(glDeleteVertexArrays(1, &m_RenderID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, VertexBufferLayout layout)
{
	Bind();
	vb.Bind();

	//go through the elements of the layout passed to add it to the array of the buffer
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}	
}

void VertexArray::Bind() const
{
	//bind (select) the buffer to be used
	GLCall(glBindVertexArray(m_RenderID));
}

void VertexArray::Unbind() const
{
	//unbind (deselect) the buffer to be used
	GLCall(glBindVertexArray(0));
}

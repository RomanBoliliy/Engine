#pragma once
#include "VertexBufferLayout.h"

namespace RenderEngine {
	VertexBufferLayout::VertexBufferLayout() 
	: m_stride(0)
	{
	
	}

void VertexBufferLayout::reserveElements(const size_t count)
{
	m_layouElements.reserve(count);
}

void VertexBufferLayout::addElementLayoutFloat(const unsigned int count, const const bool normalized)
{
	m_layouElements.push_back({ static_cast<GLint>(count), GL_FLOAT, normalized, count * sizeof(GLfloat) });
	m_stride += m_layouElements.back().size;
}

}
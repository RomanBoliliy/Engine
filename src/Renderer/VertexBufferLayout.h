#pragma once
#include <vector>
#include "glad/glad.h"

namespace Renderer {

	struct VertexBufferLayoutElement {
		GLint count;
		GLenum type;
		GLboolean normalized;
		unsigned int size;
	};

	class VertexBufferLayout {
	public:
		VertexBufferLayout();

		void reserveElements(const size_t count);
		unsigned int getStride() const { return m_stride; }
		void addElementLayoutFloat(const unsigned int count, const const bool normalized);
		const std::vector<VertexBufferLayoutElement>& getlayoutElements() const { return m_layouElements; }

	private:

		std::vector<VertexBufferLayoutElement> m_layouElements;
		unsigned int m_stride;
	};
}
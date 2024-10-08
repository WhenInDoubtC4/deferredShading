/*
*	Extension of ew::Mesh to add TBN attributes
*/

#include "mesh.h"
#include "../ew/external/glad.h"

#define VERTEX_ATTRIBUTE(index, size, type, normalized, stride, pointer) \
	glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<const void*>(pointer)); \
	glEnableVertexAttribArray(index);

//Rearranged to be compatible with ew::mesh
constexpr GLuint POSITION_ATTRIBUTE_INDEX = 0;
constexpr GLuint NORMAL_ATTRIBUTE_INDEX = 1;
constexpr GLuint UV_ATTRIBUTE_INDEX = 2;
constexpr GLuint TANGENT_ATTRIBUTE_INDEX = 3;
constexpr GLuint BITANGENT_ATTRIBUTE_INDEX = 4;

namespace Util
{
	Mesh::Mesh(const MeshData& meshData)
	{
		load(meshData);
	}

	void Mesh::load(const MeshData& meshData)
	{
		if (!_isInitialized)
		{
			glGenVertexArrays(1, &_vao);
			glBindVertexArray(_vao);

			glGenBuffers(1, &_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);

			glGenBuffers(1, &_ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

			VERTEX_ATTRIBUTE(POSITION_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
			VERTEX_ATTRIBUTE(TANGENT_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, tangent));
			VERTEX_ATTRIBUTE(BITANGENT_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, bitangent));
			VERTEX_ATTRIBUTE(NORMAL_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
			VERTEX_ATTRIBUTE(UV_ATTRIBUTE_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, uv));

			_isInitialized = true;
		}

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

		_numVertices = meshData.vertices.size();
		_numIndicies = meshData.indicies.size();
		if (!meshData.vertices.empty())
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _numVertices, meshData.vertices.data(), GL_STATIC_DRAW);
		}
		if (!meshData.indicies.empty())
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _numIndicies, meshData.indicies.data(), GL_STATIC_DRAW);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Mesh::draw(ew::DrawMode drawMode) const
	{
		glBindVertexArray(_vao);
		if (drawMode == ew::DrawMode::TRIANGLES)
		{
			glDrawElements(GL_TRIANGLES, _numIndicies, GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawArrays(GL_POINTS, 0, _numVertices);
		}
	}
}
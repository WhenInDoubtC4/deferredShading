/*
*	Extension of ew::Mesh to add TBN attributes
*/

#include "../ew/mesh.h"
#include <glm/glm.hpp>
#include <vector>

namespace Util
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	struct MeshData
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indicies;
	};

	class Mesh
	{
	public:
		Mesh() {};
		Mesh(const MeshData& meshData);
		void load(const MeshData& meshData);
		void draw(ew::DrawMode drawMode = ew::DrawMode::TRIANGLES) const;
	private:
		bool _isInitialized = false;
		unsigned int _vao = 0;
		unsigned int _vbo = 0;
		unsigned int _ebo = 0;
		unsigned int _numIndicies = 0;
		unsigned int _numVertices = 0;
	};
}
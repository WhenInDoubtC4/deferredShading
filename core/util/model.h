/*
*	Extension of ew::Model to calculate tangent space
*/

#include "mesh.h"
#include "../ew/shader.h"

namespace Util
{
	class Model
	{
	public:
		Model(const std::string& filepath);
		void draw();
	private:
		std::vector<Util::Mesh> _meshes;
	};
}
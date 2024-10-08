/*
*	Extension of ew::Model to calculate tangent space
*/

#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

namespace Util
{
	Util::Mesh processAiMesh(aiMesh* aiMesh);

	Model::Model(const std::string& filepath)
	{
		Assimp::Importer importer;
		const aiScene* aiScene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
		for (size_t i = 0; i < aiScene->mNumMeshes; i++)
		{
			aiMesh* aiMesh = aiScene->mMeshes[i];
			_meshes.push_back(processAiMesh(aiMesh));
		}
	}

	void Model::draw()
	{
		for (Util::Mesh& mesh : _meshes) mesh.draw();
	}

	glm::vec3 convertAIVec3(const aiVector3D& v) 
	{
		return glm::vec3(v.x, v.y, v.z);
	}

	Util::Mesh processAiMesh(aiMesh* aiMesh)
	{
		Util::MeshData meshData;

		for (size_t i = 0; i < aiMesh->mNumVertices; i++)
		{
			Util::Vertex vertex;
			vertex.position = convertAIVec3(aiMesh->mVertices[i]);
			if (aiMesh->HasTangentsAndBitangents())
			{
				vertex.tangent = convertAIVec3(aiMesh->mTangents[i]);
				vertex.bitangent = convertAIVec3(aiMesh->mBitangents[i]);
			}
			if (aiMesh->HasNormals())
			{
				vertex.normal = convertAIVec3(aiMesh->mNormals[i]);
			}
			if (aiMesh->HasTextureCoords(0))
			{
				vertex.uv = glm::vec2(convertAIVec3(aiMesh->mTextureCoords[0][i]));
			}

			meshData.vertices.push_back(vertex);
		}

		for (size_t i = 0; i < aiMesh->mNumFaces; i++)
		{
			for (size_t j = 0; j < aiMesh->mFaces[i].mNumIndices; j++)
			{
				meshData.indicies.push_back(aiMesh->mFaces[i].mIndices[j]);
			}
		}

		return Util::Mesh(meshData);
	}
}
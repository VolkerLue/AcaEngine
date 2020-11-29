#pragma once

#include "../core/shader.hpp"
#include "../camera.hpp"
#include "glm/glm.hpp"
#include "mesh.hpp"
#include <vector>

namespace graphics {

	class Mesh;
	class Texture2D;

	class MeshRenderer
	{
	public:
		MeshRenderer();

		void draw(const Mesh& _mesh, const Texture2D& _texture, const glm::mat4& _transform);

		void present(const Camera& _camera);
		void clear();

	private:

		struct MeshInstance {
			std::vector<Mesh::Vertex> meshVertices;
			const Texture2D& texture;
			const glm::mat4& transform;
			unsigned vao;
			unsigned vbo;
			mutable bool dirty;
		};

		std::vector<MeshInstance> instances;

		Program program;

	};
}
#pragma once

#include "../core/shader.hpp"
#include "../camera.hpp"
#include "glm/glm.hpp"
#include "mesh.hpp"
#include <vector>
#include <engine/graphics/core/geometrybuffer.hpp>

namespace graphics {
	class MeshRenderer
	{
	public:
		MeshRenderer();
		~MeshRenderer();
		void draw(const Mesh& _mesh, const Texture2D& _texture, const glm::mat4& _transform);
		void present(const Camera& _camera);
		void clear();
		Program program;
	private:
		struct MeshInstance {
			const std::vector<Mesh::Vertex>& meshVertices;
			const Texture2D& texture;
			glm::mat4 transform;
		};
		graphics::GeometryBuffer* geometryBuffer;
		std::vector<MeshInstance> instances;
	};
}

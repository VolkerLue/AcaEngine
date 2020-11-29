#include "meshrenderer.hpp"
#include "../core/opengl.hpp"
#include "../core/texture.hpp"
#include "mesh.hpp"


namespace graphics {
	//TODO: Textur richtig abbilden

	MeshRenderer::MeshRenderer() {
		program.attach(ShaderManager::get("shader/mesh.vert", ShaderType::VERTEX));
		program.attach(ShaderManager::get("shader/mesh.frag", ShaderType::FRAGMENT));
		program.link();

	}

	void MeshRenderer::draw(const Mesh& _mesh, const Texture2D& _texture, const glm::mat4& _transform) {
		unsigned vao;
		unsigned vbo;

		glCall(glGenVertexArrays, 1, &vao);
		glCall(glBindVertexArray, vao);

		glCall(glGenBuffers, 1, &vbo);
		glCall(glBindBuffer, GL_ARRAY_BUFFER, vbo);

		glCall(glEnableVertexAttribArray, 0);
		glCall(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);

		glCall(glEnableVertexAttribArray, 1);
		glCall(glVertexAttribPointer, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Normal));

		glCall(glEnableVertexAttribArray, 2);
		glCall(glVertexAttribPointer, 2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, TexCoords));

		bool dirty = true;
		struct MeshInstance mi = {_mesh.vertices, _texture, _transform, vao, vbo, dirty};
		instances.push_back(mi);
	}

	void MeshRenderer::present(const Camera& _camera) {
		if (instances.empty()) return;

		program.use();
		unsigned transformID = glCall(glGetUniformLocation, program.getID(), "Transform");
		unsigned cameraID = glCall(glGetUniformLocation, program.getID(), "Camera");

		for (auto it = instances.begin(); it != instances.end(); it++) {
			it->texture.bind(0);
			glCall(glUniformMatrix4fv, transformID, 1, GL_FALSE, &it->transform[0][0]);
			glCall(glUniformMatrix4fv, cameraID, 1, GL_FALSE, &_camera.getViewProjection()[0][0]);
			if (it->dirty) {
				glCall(glBindBuffer, GL_ARRAY_BUFFER, it->vbo);
				glCall(glBufferData, GL_ARRAY_BUFFER, it->meshVertices.size() * sizeof(Mesh::Vertex), &(it->meshVertices[0]), GL_STATIC_DRAW);
				it->dirty = false;
			}
			glCall(glBindVertexArray, it->vao);
			glCall(glDrawArrays, GL_TRIANGLES, 0, it->meshVertices.size());
			
		}
		
	}

	void MeshRenderer::clear() {
		instances.clear();
	}

}

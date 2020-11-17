#include "meshrenderer.hpp"
#include "../core/opengl.hpp"
#include "../core/texture.hpp"
#include "mesh.hpp"


namespace graphics {
	//TODO: Textur richtig abbilden, mehrere textures, mehrere objekte zeichnen, kamera richtig nutzen

	MeshRenderer::MeshRenderer() : dirty(true) {
		program.attach(ShaderManager::get("shader\\mesh.vert", ShaderType::VERTEX));
		program.attach(ShaderManager::get("shader\\mesh.frag", ShaderType::FRAGMENT));
		program.link();

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

	}

	void MeshRenderer::draw(const Mesh& _mesh, const Texture2D& _texture, const glm::mat4& _transform) {
		meshes.push_back(_mesh);
		textures = &_texture;
		transforms.push_back(_transform);
		dirty = true;
	}

	void MeshRenderer::present(const Camera& _camera) {
		if (meshes.size() == 0) return;
		program.use();
		program.setUniform(0, _camera.getViewProjection());

		(*textures).bind(0);
		unsigned transformID = glCall(glGetUniformLocation, program.getID(), "Transform");
		glCall(glUniformMatrix4fv, transformID, 1, GL_FALSE, &transforms[0][0][0]);


		if (dirty) {
			glCall(glBindBuffer, GL_ARRAY_BUFFER, vbo);
			glCall(glBufferData, GL_ARRAY_BUFFER, meshes[0].vertices.size() * sizeof(Mesh::Vertex), &(meshes[0].vertices[0]), GL_STATIC_DRAW);
			dirty = false;
		}
		glCall(glBindVertexArray, vao);
		glCall(glDrawArrays, GL_TRIANGLES, 0, meshes[0].vertices.size() * 3);
	}

	void MeshRenderer::clear() {
		meshes.clear();
		textures = (graphics::Texture2D*) 0;
		transforms.clear();
		dirty = true;
	}

}

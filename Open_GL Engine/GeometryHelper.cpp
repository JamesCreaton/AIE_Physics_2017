#include "GeometryHelper.h"
#include "gl_core_4_4.h"
#include "RenderData.h"

#define screen_width 1920
#define screen_height 1080

#include "tiny_obj_loader.h"

namespace GeometryHelper
{
	RenderData * CreateGrid(unsigned int rows, unsigned int cols, float width, float height, glm::vec4 a_colour)
	{
		SimpleVertex* verticies = new SimpleVertex[rows * cols];
		float rowSpaceing = width / (cols - 1);
		float colSpacing = height / (rows - 1);

		for (unsigned int r = 0; r < rows; ++r)
		{
			for (unsigned int c = 0; c < cols; ++c)
			{
				SimpleVertex& vert = verticies[r*cols + c];
				vert.position = glm::vec4(
					-(width / 2) + (colSpacing* c)
					, 0,
					-(height / 2) + (rowSpaceing * r), 1);

				vert.colour = a_colour;
			}
		}

		unsigned int* indices = new unsigned[(rows - 1) * (cols - 1) * 6];
		unsigned int index = 0;

		for (unsigned int r = 0; r < (rows - 1); ++r)
		{
			for (unsigned int c = 0; c < (cols - 1); ++c)
			{
				indices[index++] = r * cols + c;
				indices[index++] = (r + 1) * cols + c;
				indices[index++] = (r + 1) * cols + (c + 1);

				indices[index++] = r * cols + c;
				indices[index++] = (r + 1) * cols + (c + 1);
				indices[index++] = r * cols + (c + 1);
			}
		}

		unsigned int numberOfIndices = (rows - 1) * (cols - 1) * 6;
		RenderData* renderData = new RenderData();
		renderData->GenerateBuffers();
		// bind the vao(also binds vbo and ibo)
		renderData->Bind();
		// sends vertex data to the vbo
		glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(SimpleVertex), verticies,
			GL_STATIC_DRAW);

		// send index data to the ibo
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			numberOfIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		// tell opengl where to find the data in the vertex
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(SimpleVertex),
			(char*)offsetof(SimpleVertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(SimpleVertex),
			(char*)offsetof(SimpleVertex, colour));


		renderData->SetNumberOfIndicies(numberOfIndices);
		renderData->Unbind();

		std::vector<SimpleVertex> verts(verticies, verticies + (rows * cols));
		renderData->GetBounds().fit(verts);


		delete[] verticies;
		delete[] indices;

		return renderData;
	}
	RenderData * CreateQuad(float width, float height, glm::vec3 facingDir)
	{
		return (nullptr);
	}

	std::vector<RenderData*> LoadOBJFromDisk(std::string path)
	{
		tinyobj::attrib_t attribs;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool loadedSuccessfully = tinyobj::LoadObj(&attribs, &shapes, &materials, &err, path.c_str());
		assert(loadedSuccessfully && "ERROR::OBJFILE::INCORRECTPATH?");

		std::vector<RenderData*> modelRenderData;
		for (auto& shape : shapes) {
			std::vector<ObjVertex> vertices;
			vertices.reserve(shape.mesh.num_face_vertices.size() * 3);

			int index = 0;


			for (auto face : shape.mesh.num_face_vertices) {

				for (int i = 0; i < 3; ++i) {

					tinyobj::index_t idx = shape.mesh.indices[index + i];
					ObjVertex v;
					// positions
					v.position.x = attribs.vertices[3 * idx.vertex_index + 0];
					v.position.y = attribs.vertices[3 * idx.vertex_index + 1];
					v.position.z = attribs.vertices[3 * idx.vertex_index + 2];
					v.position.w = 1.0f;

					// normals
					if (!attribs.normals.empty()) {
						v.normal.x = attribs.normals[3 * idx.normal_index + 0];
						v.normal.y = attribs.normals[3 * idx.normal_index + 1];
						v.normal.z = attribs.normals[3 * idx.normal_index + 2];
					}
					// texture coordinates
					if (!attribs.texcoords.empty()) {
						v.uv.s = attribs.texcoords[2 * idx.texcoord_index + 0];
						v.uv.t = attribs.texcoords[2 * idx.texcoord_index + 1];
					}
					vertices.push_back(v);
				}
				index += face;
			}

			// collect triangle vertices
			std::vector<unsigned int> indices;
			indices.resize(vertices.size());
			for (int i = 0; i < vertices.size(); i++){
				indices.push_back(i);
			}

			calculateTangents(vertices, indices);

			RenderData* renderData = new RenderData();
			renderData->GenerateBuffers(false);
			renderData->Bind();
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ObjVertex), vertices.data(), GL_STATIC_DRAW);


			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);


			glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(ObjVertex), (char*)offsetof(ObjVertex, position));
			glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(ObjVertex), (char*)offsetof(ObjVertex, normal));
			glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(ObjVertex), (char*)offsetof(ObjVertex, uv));
			glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(ObjVertex), (char*)offsetof(ObjVertex, tangent));
			glVertexAttribPointer(4, 5, GL_FLOAT, false, sizeof(ObjVertex), (char*)offsetof(ObjVertex, biTangent));



			renderData->SetNumberOfIndicies(shape.mesh.num_face_vertices.size() * 3);

			renderData->Unbind();
			renderData->GetBounds().fit(vertices);
			
			modelRenderData.push_back(renderData);
		}

		return modelRenderData;
	}

	void calculateTangents(std::vector<ObjVertex>& vertices, const std::vector<unsigned int>& indices) {
		unsigned int vertexCount = (unsigned int)vertices.size();
		glm::vec4* tan1 = new glm::vec4[vertexCount * 2];
		glm::vec4* tan2 = tan1 + vertexCount;
		memset(tan1, 0, vertexCount * sizeof(glm::vec4) * 2);

		unsigned int indexCount = (unsigned int)indices.size();
		for (unsigned int a = 0; a < indexCount; a += 3) {
			long i1 = indices[a];
			long i2 = indices[a + 1];
			long i3 = indices[a + 2];

			const glm::vec4& v1 = vertices[i1].position;
			const glm::vec4& v2 = vertices[i2].position;
			const glm::vec4& v3 = vertices[i3].position;

			const glm::vec2& w1 = vertices[i1].uv;
			const glm::vec2& w2 = vertices[i2].uv;
			const glm::vec2& w3 = vertices[i3].uv;

			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			float r = 1.0F / (s1 * t2 - s2 * t1);
			glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
				(t2 * z1 - t1 * z2) * r, 0);
			glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
				(s1 * z2 - s2 * z1) * r, 0);

			tan1[i1] += sdir;
			tan1[i2] += sdir;
			tan1[i3] += sdir;

			tan2[i1] += tdir;
			tan2[i2] += tdir;
			tan2[i3] += tdir;
		}

		for (unsigned int a = 0; a < vertexCount; a++) {
			const glm::vec3& n = glm::vec3(vertices[a].normal);
			const glm::vec3& t = glm::vec3(tan1[a]);

			// Gram-Schmidt orthogonalize
			vertices[a].tangent = glm::vec4(glm::normalize(t - n * glm::dot(n, t)), 0);

			// Calculate handedness (direction of bitangent)
			vertices[a].tangent.w = (glm::dot(glm::cross(glm::vec3(n), glm::vec3(t)), glm::vec3(tan2[a])) < 0.0F) ? 1.0F : -1.0F;

			// calculate bitangent (ignoring for Vertex)
			vertices[a].biTangent = glm::vec4(glm::cross(glm::vec3(vertices[a].normal), glm::vec3(vertices[a].tangent)) * vertices[a].tangent.w, 0);
			vertices[a].tangent.w = 0;
		}

		delete[] tan1;
	}


	//Need to remove and implement properly now that it's working
	unsigned int createVertexBuffer(const tinyobj::attrib_t& attribs, const tinyobj::shape_t& shape)
	{
		//collect triangle vertices
		std::vector<ObjVertex> vertices;

		int index = 0;
		for (auto face : shape.mesh.num_face_vertices) {
			for (int i = 0; i < 3; ++i) {
				tinyobj::index_t idx = shape.mesh.indices[index + i];

				ObjVertex v;

				// positions
				v.position.x = attribs.vertices[3 * idx.vertex_index + 0];
				v.position.y = attribs.vertices[3 * idx.vertex_index + 1];
				v.position.z = attribs.vertices[3 * idx.vertex_index + 2];
				v.position.w = 1.0f;

				// normals
				if (!attribs.normals.empty()) {
					v.normal.x = attribs.normals[3 * idx.normal_index + 0];
					v.normal.y = attribs.normals[3 * idx.normal_index + 1];
					v.normal.z = attribs.normals[3 * idx.normal_index + 2];
				}
				// uvs
				if (!attribs.texcoords.empty()) {
					v.uv.x = attribs.texcoords[2 * idx.texcoord_index + 0];
					v.uv.y = attribs.texcoords[2 * idx.texcoord_index + 1];
				}
				vertices.push_back(v);
			}
			index += face;
		}

		//bind vertex data
		unsigned int buffer = 0;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ObjVertex), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return buffer;
	}
	void LoadHandTest(std::vector<GLInfo>& a_meshes)
	{
		std::string err; 
		tinyobj::attrib_t attribs[2]; 
		std::vector<tinyobj::shape_t> shapes[2]; 
		std::vector<tinyobj::material_t> materials[2]; 
		tinyobj::LoadObj(&attribs[0], &shapes[0], &materials[0], &err, "./models/hand/hand_00.obj", "./models/hand/"); 
		tinyobj::LoadObj(&attribs[1], &shapes[1], &materials[1], &err, "./models/hand/hand_37.obj", "./models/hand/");

		// loaded models have matching shape counts 
		for (int i = 0; i < shapes[0].size(); ++i) { 
			GLInfo gl;
			gl.vbo1 = createVertexBuffer(attribs[0], shapes[0][i]);
			gl.vbo2 = createVertexBuffer(attribs[1], shapes[1][i]);
			
			// shapes have matching face counts 
			gl.faceCount = shapes[0][i].mesh.num_face_vertices.size();
			glGenVertexArrays(1, &gl.vao);
			glBindVertexArray(gl.vao);
			
			// bind first VBO 
			glBindBuffer(GL_ARRAY_BUFFER, gl.vbo1);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(ObjVertex), (char*)offsetof(ObjVertex, position));
			glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(ObjVertex), (char*)offsetof(ObjVertex, normal));
			glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(ObjVertex), (char*)offsetof(ObjVertex, uv));

			
			// bind second VBO 
			glBindBuffer(GL_ARRAY_BUFFER, gl.vbo2);

			// position 2 
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ObjVertex), (char*)offsetof(ObjVertex, position));

			// normal 2 
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_TRUE, sizeof(ObjVertex), (char*)offsetof(ObjVertex, normal));
			glBindVertexArray(0); 
			
			a_meshes.push_back(gl);
		}
	}
	void DrawHand(std::vector<GLInfo>& a_meshes)
	{
		for (auto& gl : a_meshes) {
			glBindVertexArray(gl.vao);
			glDrawArrays(GL_TRIANGLES, 0, gl.faceCount * 3);
		}
	}

}



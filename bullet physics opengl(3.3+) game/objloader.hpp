#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <GL/glew.h>

#define GLFW_DLL

// Include GLFW
#include <glfw3.h>

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include "glm/glm.hpp"

#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure
#include "assimp/postprocess.h"     // Post processing flags

class ObjLoader
{
	private:
		GLuint vao; //vertex array object

		//vectors to store data of buffers
		std::vector<unsigned short> indices;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		//buffers
		GLuint vertexbuffer;
		GLuint normalbuffer;
		GLuint uvbuffer;
		GLuint elementbuffer;

		bool hasUVS;

	public:
		void ObjLoader::initObj(char *path, bool uvvalue);
		void render();
		void ObjLoader::killObject();
};




#endif
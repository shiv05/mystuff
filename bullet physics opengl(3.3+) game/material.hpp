#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <glm\glm.hpp>

class Material
{
	private:
		float diffuse[3];
		float ambient[3];
		float specular[3];
		float shininess;
	
	public:
		void Material::setMaterialValues(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shin)
		{
			diffuse[0] = diff.x;
			diffuse[1] = diff.y;
			diffuse[2] = diff.z;

			ambient[0] = amb.x;
			ambient[1] = amb.y;
			ambient[2] = amb.z;

			specular[0] = spec.x;
			specular[1] = spec.y;
			specular[2] = spec.z;

			shininess = shin;
		}

		glm::vec3 getMaterialAmbient(){
			return glm::vec3(ambient[0],ambient[1],ambient[2]);
		}
		glm::vec3 getMaterialDiffuse(){
			return glm::vec3(diffuse[0], diffuse[1], diffuse[2]);
		}
		glm::vec3 getMaterialSpecular(){
			return glm::vec3(specular[0], specular[1], specular[2]);
		}
		float getMaterialShininess(){
			return shininess;
		}

		~Material(){

		}

};

#endif
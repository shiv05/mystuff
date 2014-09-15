#ifndef LIGHTMANAGER_HPP
#define LIGHTMANAGER_HPP

#include <list>
#include "light.hpp"

class LightManager{
	
private:
	std::list<Light*> lights;

public:
	std::list<Light*>& getLights(){
		return lights;
	}
	
	void addLight(Light* light){
		Light::nextlightid++;
		light->setid(Light::nextlightid);
		lights.push_back(light);
	}
};

#endif
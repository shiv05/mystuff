#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "material.hpp"
#include "objloader.hpp"'
#include "bulletworld.hpp"


class GameObject
{
	private:
		Material go_material;
		ObjLoader go_obj;
		int go_rigidbodyindex;

	public:
		
		GameObject(Material mat, ObjLoader obj, int rigidbodyindex)
		{
			go_material = mat;
			go_obj = obj;
			go_rigidbodyindex = rigidbodyindex;
		}

		int getRigidBodyIndex(){
			return go_rigidbodyindex;
		}

		Material getMaterial(){
			return go_material;
		}

		ObjLoader getObj(){
			return go_obj;
		}
};

#endif
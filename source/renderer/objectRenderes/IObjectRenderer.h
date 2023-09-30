#pragma once

#include "../../entity/Object.h"

class IObjectRenderer {

	public:

		virtual void draw(Object* gameObject) = 0;

};
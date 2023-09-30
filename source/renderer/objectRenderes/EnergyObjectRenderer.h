#pragma once

#include "IObjectRenderer.h"

class EnergyObjectRenderer : public virtual IObjectRenderer {

	public:

		virtual void draw(Object* gameObject) override;

};
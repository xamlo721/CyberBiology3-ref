#pragma once

#include "IObjectRenderer.h"

class NaturalObjectRenderer : public virtual IObjectRenderer {

public:

	virtual void draw(Object* gameObject) override;

};
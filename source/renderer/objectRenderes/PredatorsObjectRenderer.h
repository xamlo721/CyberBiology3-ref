#pragma once

#include "IObjectRenderer.h"

class PredatorsObjectRenderer : public virtual IObjectRenderer {

public:

	virtual void draw(Object* gameObject) override;

};
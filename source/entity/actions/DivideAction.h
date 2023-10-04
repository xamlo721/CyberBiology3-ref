#pragma once

#include "IBotAction.h"

class DivideAction : public virtual IBotAction {

	public:

		virtual void onActivate(Bot* object, CellCluster* cluster) override;


};

#pragma once

#include "IBotAction.h"

class MoveAction : public virtual IBotAction {

	public:

		virtual void onActivate(Bot* object, CellCluster* cluster) override;


};

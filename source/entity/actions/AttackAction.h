#pragma once

#include "IBotAction.h"

class AttackAction : public virtual IBotAction {

	public:

		virtual void onActivate(Bot* object, CellCluster* cluster) override;

};

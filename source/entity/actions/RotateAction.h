#pragma once

#include "IBotAction.h"

class RotateAction : public virtual IBotAction {

	public :

		virtual void onActivate(Bot* object, CellCluster* cluster) override;


};


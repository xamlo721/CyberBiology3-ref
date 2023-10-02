#pragma once

#include "IBotAction.h"

class DivideAction : public virtual IBotAction {

		bool ArtificialSelectionWatcher_OnDivide(Bot* object);

	public:

		virtual void onActivate(Bot* object, CellCluster* cluster) override;


};

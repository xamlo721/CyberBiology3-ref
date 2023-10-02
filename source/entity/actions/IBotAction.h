#pragma once

#include "../Bot.h"
#include "../../world/CellCluster.h"

class IBotAction {

	public:

		virtual void onActivate(Bot * object, CellCluster * cluster) = 0;



};


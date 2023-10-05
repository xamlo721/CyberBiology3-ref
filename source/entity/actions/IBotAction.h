#pragma once

#include "../Bot.h"
#include "../../world/CellCluster.h"



class IBotAction {

	public:

		virtual void onActivate(Bot * object, CellCluster * cluster) = 0;



		Point validateBotLookAtCoordinates(Point botLookAtCoords) {

			return Point(botLookAtCoords.x + visibleDistance, botLookAtCoords.y + visibleDistance);
		}

};


#pragma once

#include "../MyTypes.h"


class SimulationTimer {

	private:

		static Clock clock;


	public:

		static TimePoint getTime() {
			return clock.now();
		}



};


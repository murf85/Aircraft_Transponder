#include <limits>
#include "include/aircraft_transponder_top.hpp"
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/stdout.hpp"
#include "cadmium/simulation/logger/csv.hpp"

using namespace cadmium;

int main() {

	auto model = std::make_shared<Transponder_top>("Aircraft Transponder Model");
	auto rootCoordinator = RootCoordinator(model);

	// rootCoordinator.setLogger<STDOUTLogger>(";");
	rootCoordinator.setLogger<CSVLogger>("aircraft_transponder_output.csv", ";");

	rootCoordinator.start();
	rootCoordinator.simulate(150.1);
	rootCoordinator.stop();	

	return 0;
}
#pragma once
#include "aircraft_types.hpp"

#include <array>
#include <memory>
#include <string>
#include <vector>

class Tower;
class Aircraft;

class AircraftFactory
{
private:
    const std::string airlines[8]                = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
    std::vector<std::string> used_flight_numbers = {};
    std::array<AircraftType, 3> aircraft_types;
    std::unique_ptr<Aircraft> create_aircraft(Tower& tower);
    const int min_fuel = 150;
    const int max_fuel = 3000;

public:
    AircraftFactory();
    friend class TowerSimulation;
};
#include "aircraft_factory.hpp"

#include "aircraft.hpp"

AircraftFactory::AircraftFactory() :
    aircraft_types { AircraftType(.02f, .05f, .02f, MediaPath { "l1011_48px.png" }),
                     AircraftType(.02f, .05f, .02f, MediaPath { "b707_jat.png" }),
                     AircraftType(.05f, .08f, .02f, MediaPath { "concorde_af.png" }) }
{}

std::unique_ptr<Aircraft> AircraftFactory::create_aircraft(Tower& tower)
{
    std::string flight_number;

    do
    {
        flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
    }
    while (std::find(used_flight_numbers.begin(), used_flight_numbers.end(), flight_number) !=
           used_flight_numbers.end());

    used_flight_numbers.emplace_back(flight_number);

    const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
    const Point3D direction = (-start).normalize();
    int init_fuel           = min_fuel + std::rand() % (max_fuel - min_fuel + 1);

    return std::make_unique<Aircraft>(aircraft_types.at(rand() % 3), flight_number, start, direction, tower,
                                      init_fuel);
}
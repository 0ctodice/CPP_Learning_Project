#pragma once

class Airport;
class AircraftManager;
struct AircraftType;

class TowerSimulation
{
private:
    bool help                         = false;
    Airport* airport                  = nullptr;
    AircraftManager* aircraft_manager = nullptr;

    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void create_aircraft(const AircraftType& type) const;
    void create_random_aircraft() const;

    void create_keystrokes() const;
    void display_help() const;

    void init_airport();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};

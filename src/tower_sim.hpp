#pragma once

#include "GL/opengl_interface.hpp"
#include "aircraft_factory.hpp"
#include "aircraft_manager.hpp"
#include "airport.hpp"
#include "img/media_path.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <string>

struct AircraftType;

struct ContextInitializer //===================> SEGFAUUUUUUUUUULT
{
    ContextInitializer(int argc, char** argv)
    {
        MediaPath::initialize(argv[0]);
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        GL::init_gl(argc, argv, "Airport Tower Simulation");
    }
};

class TowerSimulation
{
private:
    bool help = false;
    std::unique_ptr<Airport> airport;
    std::unique_ptr<ContextInitializer> context;
    std::unique_ptr<AircraftManager> aircraft_manager;
    std::unique_ptr<AircraftFactory> aircraft_factory;

    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void create_random_aircraft() const;

    void create_keystrokes();
    void display_help() const;
    void print_existing_aircraft(int i);

    void init_airport();

public:
    TowerSimulation(int argc, char** argv);

    void launch();
};

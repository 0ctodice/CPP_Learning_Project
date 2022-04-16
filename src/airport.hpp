#pragma once

#include "GL/displayable.hpp"
#include "GL/dynamic_object.hpp"
#include "GL/texture.hpp"
#include "aircraft_manager.hpp"
#include "airport_type.hpp"
#include "geometry.hpp"
#include "img/image.hpp"
#include "runway.hpp"
#include "terminal.hpp"
#include "tower.hpp"

#include <vector>

class Airport : public GL::Displayable, public GL::DynamicObject
{
private:
    const AirportType& type;
    const Point3D pos;
    const GL::Texture2D texture;
    std::vector<Terminal> terminals;
    AircraftManager* aircraft_manager;
    Tower tower;
    int fuel_stock       = 0;
    int ordered_fuel     = 0;
    int next_refill_time = 0;

    // reserve a terminal
    // if a terminal is free, return
    // 1. a sequence of waypoints reaching the terminal from the runway-end and
    // 2. the number of the terminal (used for liberating the terminal later)
    // otherwise, return an empty waypoint-vector and any number
    std::pair<WaypointQueue, size_t> reserve_terminal(Aircraft& aircraft)
    {
        const auto it =
            std::find_if(terminals.begin(), terminals.end(), [](const Terminal& t) { return !t.in_use(); });

        if (it != terminals.end())
        {
            it->assign_craft(aircraft);
            const auto term_idx = std::distance(terminals.begin(), it);
            return { type.air_to_terminal(pos, 0, term_idx), term_idx };
        }
        else
        {
            return { {}, 0u };
        }
    }

    void free_terminal(const size_t terminal_number) { terminals.at(terminal_number).free_me(); }

    WaypointQueue start_path(const size_t terminal_number)
    {
        return type.terminal_to_air(pos, 0, terminal_number);
    }

    Terminal& get_terminal(const size_t terminal_num) { return terminals.at(terminal_num); }

public:
    Airport(const AirportType& type_, const Point3D& pos_, const img::Image* image,
            AircraftManager* aircrafts, const float z_ = 1.0f) :
        GL::Displayable { z_ },
        type { type_ },
        pos { pos_ },
        texture { image },
        terminals { type.create_terminals() },
        tower { *this }
    {
        aircraft_manager = aircrafts;
    }

    ~Airport() { free(aircraft_manager); }

    Tower& get_tower() { return tower; }

    void display() const override { texture.draw(project_2D(pos), { 2.0f, 2.0f }); }

    bool move() override
    {
        assert(next_refill_time >= 0);
        if (next_refill_time == 0)
        {
            int needed_fuel = aircraft_manager->get_required_fuel();
            ordered_fuel    = std::min(5000, needed_fuel);
            std::cout << "fuel stock : " << fuel_stock << std::endl;
            fuel_stock += ordered_fuel;
            std::cout << "needed fuel : " << needed_fuel << std::endl;
            std::cout << "received fuel : " << ordered_fuel << std::endl;
            next_refill_time = 101;
        }
        next_refill_time--;

        std::for_each(terminals.begin(), terminals.end(),
                      [this](Terminal& t)
                      {
                          t.refill_aircraft_if_needed(fuel_stock);
                          t.move();
                      });

        return true;
    }

    friend class Tower;
};

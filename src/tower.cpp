#include "tower.hpp"

#include "airport.hpp"
#include "terminal.hpp"

#include <cassert>

WaypointQueue Tower::get_circle() const
{
    return { { Point3D { -1.5f, -1.5f, .5f }, wp_air },
             { Point3D { 1.5f, -1.5f, .5f }, wp_air },
             { Point3D { 1.5f, 1.5f, .5f }, wp_air },
             { Point3D { -1.5f, 1.5f, .5f }, wp_air } };
}

WaypointQueue Tower::get_instructions(Aircraft& aircraft)
{

    if (!aircraft.is_at_terminal)
    {
        // if the aircraft is far, then just guide it to the airport vicinity
        if (aircraft.distance_to(airport.pos) < 5)
        {
            // try and reserve a terminal for the craft to land
            const auto vp = airport.reserve_terminal(aircraft);
            if (!vp.first.empty())
            {
                reserved_terminals.emplace(&aircraft, vp.second);
                return vp.first;
            }
            else
            {
                return get_circle();
            }
        }
        else
        {
            return get_circle();
        }
    }
    else
    {
        // get a path for the craft to start
        const auto it = reserved_terminals.find(&aircraft);
        assert(it != reserved_terminals.end());
        const auto terminal_num = it->second;
        Terminal& terminal      = airport.get_terminal(terminal_num);
        if (!terminal.is_servicing())
        {
            aircraft.has_left = true;
            terminal.finish_service();
            reserved_terminals.erase(it);
            aircraft.is_at_terminal = false;
            return airport.start_path(terminal_num);
        }
        else
        {
            return {};
        }
    }
}

WaypointQueue Tower::reserve_terminal(Aircraft& aircraft)
{
    if (aircraft.is_circling())
    {
        const auto vp = airport.reserve_terminal(aircraft);
        if (!vp.first.empty())
        {
            reserved_terminals.emplace(&aircraft, vp.second);
            return vp.first;
        }
    }
    return {};
}

void Tower::unregister_on_crash(const Aircraft& aircraft)
{
    const auto it = reserved_terminals.find(&aircraft);
    if (it != reserved_terminals.end())
    {
        airport.free_terminal(it->second);
        reserved_terminals.erase(it);
    }
}

void Tower::arrived_at_terminal(const Aircraft& aircraft)
{
    assert(aircraft.is_on_ground());
    const auto it = reserved_terminals.find(&aircraft);
    assert(it != reserved_terminals.end());
    airport.get_terminal(it->second).start_service(aircraft);
}

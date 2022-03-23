#pragma once

#include "aircraft.hpp"

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts = {};
    int landing_crash_counter                        = 0;

public:
    void add_aircraft(std::unique_ptr<Aircraft> aircraft)
    {
        assert(aircraft);
        aircrafts.emplace_back(std::move(aircraft));
    }

    bool move() override
    {
        std::sort(aircrafts.begin(), aircrafts.end(),
                  [](const std::unique_ptr<Aircraft>& a, const std::unique_ptr<Aircraft>& b)
                  { return a->has_terminal() > b->has_terminal() && a->fuel < b->fuel; });

        aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                       [this](const std::unique_ptr<Aircraft>& aircraft)
                                       {
                                           bool state = false;
                                           try
                                           {
                                               state = !aircraft->move();
                                           } catch (const AircraftCrash& err)
                                           {
                                               std::cerr << err.what() << std::endl;
                                               landing_crash_counter++;
                                               return true;
                                           }
                                           return state;
                                       }),
                        aircrafts.end());
        return true;
    }

    int get_required_fuel()
    {
        int value = 0;
        std::for_each(aircrafts.begin(), aircrafts.end(),
                      [&value](const std::unique_ptr<Aircraft>& a)
                      {
                          assert(a);
                          if (a->is_at_terminal && !(a->has_left) && a->is_low_on_fuel())
                          {
                              value += (3000 - a->fuel);
                          }
                      });
        return value;
    }

    friend class TowerSimulation;
};
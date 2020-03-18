#ifndef __GOAL_AGENT__H
#define __GOAL_AGENT__H 

#include "enviro.h"

using namespace enviro;

class goalController : public Process, public AgentInterface {

    public:
    goalController() : Process(), AgentInterface() {}

    void init() {label("goal",-13,2);}
    void start() {}
    void update() {}
    void stop() {}

};

class goal : public Agent {
    public:
    goal(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    goalController c;
};

DECLARE_INTERFACE(goal)

#endif
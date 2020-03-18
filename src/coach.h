#ifndef __COACH_AGENT__H
#define __COACH_AGENT__H 

#include "enviro.h"

using namespace enviro;

class coachController : public Process, public AgentInterface {

    public:
    coachController() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {label("press space bar to stop/start; \n press r to restart; \n click on screen to teleport robot",15,15);}
    void stop() {}

};

class coach : public Agent {
    public:
    coach(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    coachController c;
};

DECLARE_INTERFACE(coach)

#endif
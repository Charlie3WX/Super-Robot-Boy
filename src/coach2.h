#ifndef __COACH2_AGENT__H
#define __COACH2_AGENT__H 

#include "enviro.h"

using namespace enviro;

// A class invisible on screen serve as textbox to show instructions

class coach2Controller : public Process, public AgentInterface {

    public:
    coach2Controller() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {label("press w/s to accelerate/decelerate; \n press a/d to bare left/right",15,15) ;}
    void stop() {}

};

class coach2 : public Agent {
    public:
    coach2(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    coach2Controller c;
};

DECLARE_INTERFACE(coach2)

#endif
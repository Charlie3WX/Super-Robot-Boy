#ifndef __PIT_AGENT__H
#define __PIT_AGENT__H 

#include "enviro.h"

using namespace enviro;

// A static object on screen serve as pit

class pitController : public Process, public AgentInterface {

    public:
    pitController() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {label("pit",-7,2);}
    void stop() {}

};

class pit : public Agent {
    public:
    pit(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    pitController c;
};

DECLARE_INTERFACE(pit)

#endif
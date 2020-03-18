#ifndef __MY_ROBOT_AGENT__H
#define __MY_ROBOT_AGENT__H

#include "enviro.h"

using namespace enviro;

class MyRobotController : public Process, public AgentInterface
{

public:
    MyRobotController() : Process(), AgentInterface() {}
    void init()
    {
        v.clear();
        b = false;
        v = {0, 0, 0};
        iter = 0;
        state = "stop";
        dead_end = false;
        dead = false;
        watch("keydown", [&](Event &e) {
            auto k = e.value()["key"].get<std::string>();
            if (k == " " && state == "stop")
            {
                state = "forward";
            }
            else if (k == " " && state == "forward")
            {
                state = "stop";
            }
            else if (k == "r")
            {
                dead_end = false;
                v.clear();
                v = {0, 0, 0};
                iter = 0;
                teleport(-310, -160, 0);
            }
            else if (k == "a")
            {
                track_velocity (5, -2);
            }
            else if (k == "d")
            {
                track_velocity (5, 2);
            }
        });

        watch("keyup", [&](Event &e) {
            auto k = e.value()["key"].get<std::string>();
            if (k == "a" || k == "d")
            {
                state = "forward";
            }
        });

        watch("screen_click", [this](Event e) {
            teleport(e.value()["x"], e.value()["y"], 0);
        });

        notice_collisions_with("goal", [&](Event &e) {
            //label("ROBOT WIN!",15,-5);
            state = "ROBOT WIN!";
        });
    }
    void start() {}
    void update()
    {

        if (a == true)
        {
            d = "dead";
        }
        else
        {
            d = "live";
        }
        label(d, 15, -5);

        if (state == "ROBOT WIN!")
        {
            track_velocity(0, 0);
        }
        else
        {
            if (sensor_reflection_type(0) == "goal")
            {
                state = "forward";
            }

            if (sensor_reflection_type(0) == "goal")
            {
                state = "win_prim";
                track_velocity(5, 0);
            }

            if (state == "forward")
            {
                track_velocity(5, 0);
                //if (dead_end == false)
                //{
                if (sensor_value(0) < 20)
                {

                    if (sensor_value(1) < 30 && sensor_value(2) < 30)
                    {

                        v1 = sensor_value(0);

                        dead_end = true;
                        //state = "dead_end";
                    }

                    if (sensor_value(1) > sensor_value(2))
                    {

                        track_velocity(0, 0);
                        apply_force(0, 617);
                    }
                    if (sensor_value(1) < sensor_value(2))
                    {
                        track_velocity(0, 0);
                        apply_force(0, -617);
                    }
                }

                if (a == true)
                {
                    if (sensor_value(0) < v2 - 10)
                    {
                        if (dir == "l")
                        {
                            apply_force(0, -617);

                            a = false;
                        }
                        else
                        {
                            apply_force(0, 617);

                            a = false;
                        }
                    }
                }

                if (dead_end == true)
                {
                    if (sensor_value(1) < 30 && sensor_value(2) < 30 && sensor_value(0) > 150)
                    {
                        b = true;
                        dead_end = false;
                    }
                }
                if (b == true)
                {
                    if (sensor_value(1) > 100)
                    {

                        v2 = sensor_value(0);
                        dir = "r";
                        a = true;
                        b = false;
                    }
                    else if (sensor_value(2) > 100)
                    { // left, so turn left

                        v2 = sensor_value(0);

                        dir = "l";
                        a = true;
                        b = false;
                    }
                }
            }

            if (state == "dead_end")
            {
                track_velocity(5, 0);
                if (sensor_value(1) > 3 * v1)
                {
                    a = true;
                    v2 = sensor_value(0);
                    dir = "r";
                }
                else if (sensor_value(2) > 3 * v1)
                { // left, so turn left
                    a = true;
                    v2 = sensor_value(0);
                    dir = "l";
                }
                if (a == true)
                {
                    if (sensor_value(0) < v2)
                    {
                        if (dir == "l")
                        {
                            apply_force(0, -617);
                            dead_end = false;
                        }
                        else
                        {
                            apply_force(0, 617);
                            dead_end = false;
                        }
                    }
                }
            }
        }

        //if (sensor_value(1) < 30 && sensor_value(2) < 30)
        //{
        //  dead_end = true;
        //}

        // if (sensor_value(1) > sensor_value(2))
        //{ // right > left
        //turn right
        //emit(Event("right"));
        //     apply_force(0,618);
        //state = "right";
        //  }
        //  else if (sensor_value(2) > sensor_value(1))
        // { // right > left
        //turn right
        //emit(Event("left"));
        //     state = "left";
        // }
        //   }

        //if (sensor_value(1) < 30 && sensor_value(2) < 30)
        //{
        //  dead_end = true;
        //}

        // if (sensor_value(1) > sensor_value(2))
        //{ // right > left
        //turn right
        //emit(Event("right"));
        //     apply_force(0,618);
        //state = "right";
        //  }
        //  else if (sensor_value(2) > sensor_value(1))
        // { // right > left
        //turn right
        //emit(Event("left"));
        //     state = "left";
        // }

        /*
                    else if( (sensor_value(2) < sensor_value(1) ) && (sensor_value(2) <50) && (sensor_value(1) <50))
                    {
                        //emit(Event("micro_right"));
                        state = "micro_right";
                    }
                    else if ((sensor_value(1) < sensor_value(2) ) && (sensor_value(2) <50) && (sensor_value(1) <50))
                    {
                        //emit(Event("micro_left"));
                        state = "micro_left";
                    }*/

        /*
                    if (sensor_value(1) > 120 && sensor_value(1) > sensor_value(2))
                    {
                        iter++;
                        state = "right90";
                        v1 = sensor_value(1);
                    }
                    else if (sensor_value(2) > 120 && sensor_value(2) > sensor_value(1))
                    {
                        iter++;
                        state = "left90";
                        v1 = sensor_value(2);
                    }
                    else if (sensor_value(2) < 10)
                    {
                        //emit(Event("micro_right"));
                        state = "micro_right";
                    }
                    else if (sensor_value(1) < 10)
                    {
                        //emit(Event("micro_left"));
                        state = "micro_left";
                    }
                }*/

        if (state == "right")
        {
            track_velocity(0, 5);

            if (sensor_value(0) > 70)
            {
                state = "forward";
            }
        }

        else if (state == "left")
        {
            track_velocity(0, -5);
            if (sensor_value(0) > 70)
            {
                state = "forward";
            }
        }

        else if (state == "right90")
        {

            track_velocity(0, 1);
            if (sensor_value(0) > 70 && sensor_value(1) > 30 && sensor_value(2) > 30)
            {
                dead_end = false;

                state = "forward";
            }
        }

        else if (state == "left90")
        {

            track_velocity(0, -1);
            if (sensor_value(0) > 70 && sensor_value(1) > 30 && sensor_value(2) > 30) //
            {
                dead_end = false;
                state = "forward";
            }
        }

        else if (state == "micro_left")
        {
            track_velocity(0.5, -0.1);
            if (sensor_value(1) > 0.1)
            {
                state = "forward";
            }
        }

        else if (state == "micro_right")
        {
            track_velocity(0.5, 0.1);
            if (sensor_value(2) > 0.1)
            {
                state = "forward";
            }
        }

        else if (state == "stop")
        {
            track_velocity(0, 0);
        }
    }

    void
    stop()
    {
    }

    std::string state;
    std::string d;
    std::string dir;
    std::vector<int> v;
    bool a;
    double v1;
    double v2;
    int iter;
    bool dead_end;
    bool dead;
    bool b;
};

/*
class Rotating_right : public State, public AgentInterface
{
public:
    void entry(const Event &e) { rate = 2; }
    void during()
    {
        track_velocity(0, rate);
        if (sensor_value(0) > 110)
        {
            emit(Event("R_forward"));
        }
    }
    void exit(const Event &e) {}
    double rate;
    void set_tick_name(std::string s) { tick_name = s; }
    std::string tick_name;
};

class Rotating_left : public State, public AgentInterface
{
public:
    void entry(const Event &e) { rate = -2; }
    void during()
    {
        track_velocity(0, rate);
        if (sensor_value(0) > 110)
        {
            emit(Event("L_forward"));
        }
    }
    void exit(const Event &e) {}
    double rate;
    void set_tick_name(std::string s) { tick_name = s; }
    std::string tick_name;
};

class Micro_left : public State, public AgentInterface
{
public:
    void entry(const Event &e) { rate = -1; }
    void during()
    {
        track_velocity(0, rate);
        if (sensor_value(1) > 1)
        {
            emit(Event("Ml_out"));
        }
    }
    void exit(const Event &e) {}
    double rate;
    void set_tick_name(std::string s) { tick_name = s; }
    std::string tick_name;
};

class Micro_right : public State, public AgentInterface
{
public:
    void entry(const Event &e) { rate = 1; }
    void during()
    {
        track_velocity(0, rate);
        if (sensor_value(2) > 1)
        {
            emit(Event("Mr_out"));
        }
    }
    void exit(const Event &e) {}
    double rate;
    void set_tick_name(std::string s) { tick_name = s; }
    std::string tick_name;
};

class Stop : public State, public AgentInterface
{
public:
    void entry(const Event &e) {}
    void during()
    {
        track_velocity(0, 0);
    }
    void exit(const Event &e) {}
    double rate;
    void set_tick_name(std::string s) { tick_name = s; }
    std::string tick_name;
};

class MyRobotController : public StateMachine, public AgentInterface, public Process
{

public:
    MyRobotController() : StateMachine(), Process()
    {
        set_initial(moving_forward);
        tick_name = "tick_" + std::to_string(rand() % 1000); // use an agent specific generated
                                                             // event name in case there are
                                                             // multiple instances of this class
        add_transition("right", moving_forward, rotating_right);
        add_transition("left", moving_forward, rotating_left);
        add_transition("R_forward", rotating_right, moving_forward);
        add_transition("L_forward", rotating_left, moving_forward);
        add_transition("micro_right", moving_forward, micro_right);
        add_transition("micro_left", moving_forward, micro_left);
        add_transition("Ml_out", micro_left, moving_forward);
        add_transition("Mr_out", micro_right, moving_forward);
        add_transition("space_bar", moving_forward, stops);
        //moving_forward.set_tick_name(tick_name);
        //rotating_left.set_tick_name(tick_name);
        //rotating_right.set_tick_name(tick_name);
    }

    MovingForward moving_forward;
    Rotating_right rotating_right;
    Rotating_left rotating_left;
    Micro_left micro_left;
    Micro_right micro_right;
    Stop stops;
    std::string tick_name;

    void init()
    {
        /* watch("keydown", [&](Event &e) {
            auto k = e.value()["key"].get<std::string>();
            if ( k == " " && !firing ) {
                  Agent& bullet = add_agent("Bullet", 
                    x() + 17*cos(angle()), 
                    y() + 17*sin(angle()), 
                    angle(), 
                    BULLET_STYLE);    
                    bullet.apply_force(50,0);
                  firing = true;
            } else if ( k == "w" ) {
                  f = magnitude;              
            } else if ( k == "s" ) {
                  f = -magnitude;  
            } else if ( k == "a" ) {
                  tau = -magnitude;
            } else if ( k == "d" ) {
                  tau = magnitude;
            } 
        });   
    }

    void start() {}
    void update() {}
    void stop() {}
};*/

class MyRobot : public Agent
{
public:
    MyRobot(json spec, World &world) : Agent(spec, world)
    {
        add_process(c);
    }

private:
    MyRobotController c;
};

DECLARE_INTERFACE(MyRobot)

#endif
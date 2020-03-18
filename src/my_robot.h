#ifndef __MY_ROBOT_AGENT__H
#define __MY_ROBOT_AGENT__H

#include "enviro.h"

using namespace enviro;

// Our main character, the robot hero
// Robot moves on his own, turns 90 degrees when meet a corner,
// if encountered a dead end, go back and go optional route

class MyRobotController : public Process, public AgentInterface
{

public:
    MyRobotController() : Process(), AgentInterface() {}
    void init() // initialize parameters
    {
        rate = 5.0;     // rate of moving forward
        state = "stop"; // initial state set to stop

        // all conditional variable set to false
        turned_180 = false;
        dead_end = false;
        ready_to_turn = false;

        // watch for various key press events
        watch("keydown", [&](Event &e) {
            auto k = e.value()["key"].get<std::string>();
            if (k == " " && state == "stop") // space bar to start robot
            {
                state = "forward";
            }
            else if (k == " " && state == "forward") // space bar to stop robot
            {
                state = "stop";
            }
            else if (k == "r") // r to reset and teleport to starting point
            {
                dead_end = false;
                rate = 5.0;
                state = "stop";
                teleport(-310, -160, 0);
            }
            else if (k == "a") // a to bare left
            {
                track_velocity(rate, -2);
                label("left", 15, -5);
            }
            else if (k == "d") // d to bare right
            {
                track_velocity(rate, 2);
                label("right", 15, -5);
            }

            else if (k == "w") // w to increase speed
            {
                rate += 0.5;
            }

            else if (k == "s") // s to decrease speed
            {
                rate -= 0.5;
            }
        });

        // watch for key down envent
        watch("keyup", [&](Event &e) {
            auto k = e.value()["key"].get<std::string>();
            if (k == "a" || k == "d")
            {
                state = "forward";
            }
        });

        // watch for click to teleport robot
        watch("screen_click", [this](Event e) {
            teleport(e.value()["x"], e.value()["y"], 0);
        });

        // watch for the robot collide with goal object
        notice_collisions_with("goal", [&](Event &e) {
            state = "ROBOT WIN!"; // if so then win
        });

        // watch for the robot collide with pit
        notice_collisions_with("pit", [&](Event &e) {
            label("ROBOT DIES!", 50, -5);
            state = "ROBOT DIES!"; // if so then die
        });
    }
    void start() {}
    void update() // core update loop
    {

        if (state == "stop") // stop state
        {
            label("stop", 15, -5);
            track_velocity(0, 0);
        }

        if (state == "ROBOT WIN!") // win state
        {
            label("ROBOT WIN!", -100, -5);
            track_velocity(0, 0); // if won, stop moving
        }

        else
        {

            if (state == "forward") // forward state
            {
                label("forward", 15, -5); // label state
                track_velocity(rate, 0);  // go staright forward

                if (sensor_reflection_type(0) == "goal") // if our robot sees the goal, go staright to it
                {

                    track_velocity(rate, 0);
                }
                else
                {

                    if (sensor_value(0) < 20) // front sensor indicates wall imminent
                    {

                        if (sensor_value(1) < 30 && sensor_value(2) < 30) // check left and right sensor to see if reaches a dead end
                        {
                            label("dead end", 15, -5);
                            v1 = sensor_value(0);

                            dead_end = true;
                        }

                        if (sensor_value(1) > sensor_value(2)) // if right sensor value > left
                        {
                            label("turn right", 15, -5);
                            track_velocity(0, 0);
                            apply_force(0, 617); // turn right
                        }
                        if (sensor_value(1) < sensor_value(2)) // if right sensor value < left
                        {
                            label("turn left", 15, -5);
                            track_velocity(0, 0);
                            apply_force(0, -617); // turn left
                        }
                    }

                    if (ready_to_turn == true)
                    {
                        // check for front sensor so that robot doesn't turn immidieately to hit wall
                        if (sensor_value(0) < v2 - 10)
                        {
                            if (dir == "l")
                            {
                                apply_force(0, -617);

                                ready_to_turn = false;
                            }
                            else
                            {
                                apply_force(0, 617);

                                ready_to_turn = false;
                            }
                        }
                    }

                    if (dead_end == true) // once dead_end is true, check if already turned 180 degree
                    {
                        if (sensor_value(1) < 30 && sensor_value(2) < 30 && sensor_value(0) > 150) // by checking front sensor value
                        {
                            turned_180 = true;
                            dead_end = false;
                        }
                    }
                    if (turned_180 == true)
                    { // now check whether left or right has a way
                        if (sensor_value(1) > 100)
                        {
                            label("ready to turn", 15, -5);
                            v2 = sensor_value(0); // save front sensor value at this moment
                            dir = "r";            // direction to turn next
                            ready_to_turn = true;
                            turned_180 = false;
                        }
                        else if (sensor_value(2) > 100)
                        {
                            label("ready to turn", 15, -5);
                            v2 = sensor_value(0); // save front sensor value at this momen
                            dir = "l";            // direction to turn next
                            ready_to_turn = true;
                            turned_180 = false;
                        }
                    }
                }
            }
        }
    }
    void
    stop()
    {
    }

    std::string state; // string to indicate which state
    std::string dir;   // direction to turn after a dead end

    //boolean values for turning after dead end:
    bool ready_to_turn;
    bool dead_end;
    bool turned_180;

    // temperary variable to store sensor value
    double v1;
    double v2;

    // rate at which robot moves
    double rate;
};

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
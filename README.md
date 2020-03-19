# Super-Robot-Boy
A robot maze mini game using ENVIRO and Elma

By Wenbo Xu 
for EEP P 520 Wi 20 - final project

Goal of the Project
---

The goal of this project is to develop a autonomous robot to navigate through a pre-defined maze, the robot will have ability to move in a straight line, turn right angle when approches corners, and to go back and go optional route if encounters a deadend. The robot will win if reaches the goal object, or die if collide with the pit object. Whole project is coded in C++ based on [enviro](https://github.com/klavinslab/enviro) and [elma](https://github.com/klavinslab/elma) developed by Professor Eric Klavins.

The robot perceive and make decisions about how to navigate through the maze based on the sensor values it gets through the 4 sensors attached on it. In addition to the autonomous part, the user can intervene the robot by keyboard and mouse commands including starting and stoping the robot, accelerate and decelerate, teleport the robot, reset the robot or change the direction where the robot is facing slightly.

Our robot hero is not a omin-directional agent, therefore cannot move sideways, this design decision is made aiming to simulate how real life robot might work. 

Key Challenges
---

- The project initialily used `track_velocity` only, not only to move forward but also to turn corners. This makes turning a consistent 90 degrees very difficult (we need to turn 90 degrees to best keep robot center on the track), since only sensor values are avaliable to me, so I can only use sensor value to determine when to start turning and end turning. Then for each different corner, although the threshold sensor value can be the same for checking when to start turning, but since the value for front sensor threshold for when to stop turning will be very different depending on what the situation is after turning this corner (e.g. if it is another corner then sensor value will be small, if it is staright up ahead then front sensor could be very large), setting a threshold will be diffficult. 

   Initially I try to mitigate this problem by adding `micro_left` and `micro_right` function to the robot so when it senses the left or right side is too close to the wall then it will start moving slightly to the left and right. But this will just result in robot constantly bumping into the walls and mess with overall moving scheme. Also the sensor update issue doesn't help, which I will disscuss in the following challenges.
   
   This challenge is finally addressed by major change of code, now it uses `apply_force` to turn corners, so a relatively consistent 90 degrees can be turned. All these work is aimed to keep our robot hero properly centered on the track at all times. so it doesn't bump into walls.
   
- The program uses a lot of if statements on sensor values to make various decisions, if statement is evaluated in the `update()` part of the process.

   It takes me rather a small amount of time to find out that sensor value updates a lot faster than the if conditions are checked. The issue with this is starightforward, for example, it is almost impossible to check for the condition `sensor_value(a) == sensor_value(b)`, and many times, checking for `sensor_value(a) > sensor_value(b)` is inconsistent at best. 
   
   Because of the update inconsistency, checking for comparisions never gaurantee the same result, so the best solution for this is to compare sensor value with constant values. Like, if `sensor_value(0) <20` then start turning, and utilize comparisions that is very clear, like whether to turn left or right depends on if `sensor_value(1) < sensor_value(2)`, so our margin of error can be forgiving.
   
   As I mentioned above, since the update speed for check for conditions is much slower than the update speed for the sensor value, we are essntially just sampling a sensor value at a giving speed, this results in randomness in the automonous robot moving scheme. 
   
   Sometimes the robot will move just fine and able to reach the goal even only uses the `track_velocity` method, other times it will often fails. Then as I mentioned above, `apply_force` is used to avoid checking exit condition for turning corners. Even so, since the robot is moving when applying force, and enter condition for turning is still checked by making a comparision of front sensor value with the threshold value, there are still insconsistencies regarding whether the robot could turn 90 degrees or not and therefore can reach the goal or not.
   
   To solve this, I gave the user some control over the direction of the robot, so the user could help the robot just a little bit when the robot is too close to the side walls.
   
   But this is a compromise nontheless, in an ideal situation the robot can be fully autonomous and can reach the goal every time, if I had more time I could make the robot to keep itself at the center at all time. Anyone is welcomed to modify the code to implement a better behavior.
   
- Initially, the program is based on statemachine, exiting and entering different states. But in order to use various of keyboard and mouse commands, the program is overhauled to inherent from process instead. After the change, the essence of the state machine logic is till there, but implemented using many if conditions.

- A major challenge comes when how the robot is able to deal with dead ends. 

   Our robot hero notice the dead end situation by checking if front, left and right sensor are all smaller than a constant value, then turns 180 degree to go back.
   
   Since the robot is set to ignore any optional route by default, after it encounters a dead end and goes back, it will take any optional route that's sensed.
   
   Once it noticed for example, there is a optional route to its left on the way back, if we turn immediately then there is a high chance that the robot will hit the wall. To solve this, various of conditions are implemented so that when it notice a optional route, first take note of to turn next or right, then the robot agent will take note of the front sensor value, and after front sensor value has decreased by a constant (set to be 10) then turn. 
   
   The aim of this is to let the robot wait a little bit before turning immedieately when it senses a optional route to avoid hitting walls, I used the technique of comparing old front sensor value and real time front sensor value to achieve this.

Install and run the code
---

- Get [Docker](https://www.docker.com/) installed

- In your terminal, Pull enviro image and mount your folder by doing

   `docker run -p80:80 -p8765:8765 -v {$Your path}:/source -it klavins/enviro:v1.4 bash`
   swap `{$Your path}` with your desired mounting folder
   
- clone this repo and copy all the files to your mounted folder

- In your terminal, make sure you are in the right path with all the files from this github repo and use the following command:

   `esm start`
   
   `make`

- Make sure you don't use command `esm init`, which will clear all the files.

- Go to http://localhost in your browser, and you will see the enviro client

- Type in command `enviro` in your terminal to start the program!

- Press Ctrl+C in terminal to break out the running program, then you can `make` again if any change is made in the code, or do `enviro` again to restart the client. 

Run, use and play with it!
---

After successfully compling and running the code, now you can play with it!

Remeber, use command `enviro` in terminal to start the program

- Controls:


   Press `space bar` to start/stop the robot

   Press `r` to reset the robot

   Press `a` or `d` to slightly turn the direction of the robot

   Press `w` to accelerate, `s` to decelerate

   Click on screen to teleport the robot

- Lable to the top right of the robot indicates its state

- Goal:

   Start by just pressing `space bar` to let the robot move on its own, see if it can reaches the goal by itself
   
   If the robot freaks out, press `r` to reset it to starting point
   
   Help our robot hero out! Press `a` and `d` to slightly change its direction if it is too closed to one side of the wall
   
- Rules:

   If our robot hero touches pit, it dies!
   
   If it touches the goal, it wins!
   
There are some randomness to how the robot turn, because of the difference in sampling rate of the `update` function and sensor values, try running the program more than 1 time (using `r` to restart and run again), if you are lucky, the robot will get to the goal on its own, if he couldn't, be kind and help him a little bit on its journey by pressing `a` and `d` to adjust its facing direction.

Be aware that changing the speed of the robot may be super cool, it could also mess up the angle it turns.

Contact Me!
---
If you have any issue regarding running the program or anything, feel free to email me at wenboxu@uw.edu

Acknowedgments
---

All code written on my own or uses parts or get inspiration from [enviro](https://github.com/klavinslab/enviro) and [elma](https://github.com/klavinslab/elma).

All code based on [enviro](https://github.com/klavinslab/enviro) and [elma](https://github.com/klavinslab/elma) by professor Eric Klavins.

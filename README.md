# Super-Robot-Boy
A robot maze mini game using ENVIRO and Elma

By Wenbo Xu 
for EEP P 520 Wi 20 - final project

Goal of the Project
---

The goal of this project is to develop a autonomous robot to navigate through a pre-defined maze, the robot will have ability to move in a straight line, turn right corners when approches corners, and to go back and go optional route if encounters a deadend. The robot will win if reaches the goal object, or die if collide with the pit object. Whole project is coded in C++ based on [enviro](https://github.com/klavinslab/enviro) and [elma](https://github.com/klavinslab/elma) developed by professor Eric Klavins.

The robot perceive and make decisions about how to navigate through the maze based on the sensor values it gets through the 4 sensors attached on it. In addition to the autonomous part, the user can intervene the robot by keyboard and mouse commands including starting and stoping the robot, accelerate and decelerate, teleport the robot, reset the robot or change the direction where the robot is facing slightly.

Our robot hero is not a omin-directional agent, therefore cannot move sideways, this design decision is made aiming to simulate how real life robot might work. 

Key Challenges:
---

- The project initialily used track_velocity only, not only to move forward but also to turn corners. This makes turning a consistent 90 degrees very difficult (we need to turn 90 degrees to best keep robot center on the track), since only sensor values are avaliable to us, so I can only use sensor value to determine when to start turning and end turning. Then for each different corner, the though the threshold sensor value can be the same for checking when to start turning, but since the value for front sensor threshold for when to stop turning will be very different depending on what the situation is after turning this corner (e.g. if it is another corner then sensor value will be small, if it is staright up ahead then front sensor could be very large), setting a threshold will be diffficult. 

   Initially I try to mitigate this problem by adding micro_left and micro_right command to the robot so when it senses the left or right side is too close to the wall then it will start moving slightly to the left and right



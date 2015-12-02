Final Project repository for Fall 2015 CS3281

Authors:
Zachary Barton
Eric Rubio

Project 4: Distributed Resource and Node Monitor

Computing clusters are often used in high performance computing.
In this project you will create a distributed resource monitor that will monitor the health of the cluster.
Each node in the cluster will run a set of monitors that will periodically output the available resources.
You will specifically consider the following resources:
•	CPU
•	Memory
•	Storage
•	Network


All nodes will disseminate their resource value using the LCM package (http://lcm-proj.github.io/).
Take a look at the LCM tutorial: http://lcm-proj.github.io/tut_lcmgen.html.
You will also develop a central monitoring utility that will output the currently available resources across cluster.
It will also mark a node from where no information is available as offline.

Finally, you will implement simple threshold based rules to execute pre-specified functions when the monitored value
either goes below a threshold of goes above a threshold.

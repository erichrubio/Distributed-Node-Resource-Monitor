Authors:
Eric Rubio
Zachary Barton

Distributed Resource and Node Monitor

Computing clusters are often used in high performance computing.
We have created a distributed resource monitor that will monitor the health of the cluster.
Each node in the cluster runs a set of monitors that periodically output the available resources.
The program monitors following resources:
•	CPU
•	Memory
•	Storage
•	Network


The nodes disseminate their resource value using the LCM package (http://lcm-proj.github.io/).
A central monitoring utility that outputs the currently available resources across cluster and also marks a node from where no information is available as offline.

A set of threshold based rules execute pre-specified functions when the monitored value either goes below a threshold of goes above a threshold.

# QtWFM

QtWFM is a C++ program designed to perform basic work force management operations such as resource analyzing, shift and break planning.

## Resource Analysis

Resource Analysis is one of the main tasks of the planning department of the work force management teams. It is related to calculate the number of the required employees using the `AHT`, `Volume`, `Work Hour`, `Off Day`, `Utilization Rate` parameters of a project.

`Volume` is the number of tasks that the customer promises to send for a specific time interval. Usually this time interval corresponds a day. So `Volume` is the number of tasks that are supposed to be accomplished by the employees in a day.

`AHT` is the average handling/processing time each worker has to spend to perform each task.

`Utilization Rate` refers to the ratio of working time spent without offline activities such as breaks, meetings, training, to total working hours.

`Coverage` represents the ratio of full-time equivalent employees (FTE) to employees required.

`Occupancy` represents the ratio of the coverage according to 100 % coverage.

`Waiting Time` is the average waiting time between two consequent tasks and can be found multplicating the `trend` of a specific hour with the ratio of the idle rate (1 - occupancy rate) to occupancy rate of that specific hour.

## Shift Planning

Shift Planning is one of the main important tasks for which the planning department of workforce management are responsible. At this step, the possible shift systems that provides at least 100 % coverage in every hour are searched.

For now the program generates only template shifts which do not include employee off day or vacation day requests.

## Break Planning

Break Planning is another important task which is related with planning the offlice activities of employees. QtWFM searches the best times and plan the activities in such a way that the coverage rate of every minute is aimed to be over 100%. 

## Screenshots

![img_1](https://github.com/user-attachments/assets/8a66158e-6199-40c9-b05d-b02d0881287e)
![img_2](https://github.com/user-attachments/assets/b08e805e-326f-44df-89f4-30d184316680)
![img_3](https://github.com/user-attachments/assets/342757ff-550f-4b22-bb94-e6e1039a4adf)
![img_4](https://github.com/user-attachments/assets/6649d543-6032-4416-9199-075df02052ea)


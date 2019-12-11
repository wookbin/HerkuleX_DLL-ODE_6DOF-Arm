### README.md

- This is an example of 6DOF Arm using HerkuleX DLL and ODE Library.
- The sample code is Created in version 'Visual Studio 2019(MFC)'
- The sample code is based on the assumption that the user connected 6 HerkuleX (ID 1 ~ 6).
* Serial device connected to PC is set to COM3 & 115,200bps Boardrate

[Sample Code UI Description]

1) After executing the program, press the 'Connect' button as shown in the picture below, and the communication connection will be established. If you press the 'Get Data' button, the initialize operation will proceed.
![1](https://user-images.githubusercontent.com/58063370/70582401-c8565b80-1bfd-11ea-8e0b-2f342600274e.png)
![2](https://user-images.githubusercontent.com/58063370/70582476-fe93db00-1bfd-11ea-8d86-b7f755165430.PNG)

2) The three 'POSE Set' buttons located at the bottom center can be used to store three locations, and the 'Pose Go' button located to the right of the 'Pose Set' buttons can be used to move to the saved location.

- 'Pose Set' command to save the position is to hold the 6DOF Arm by hand and then press the 'Pose Set' button at the desired position to save the current 6 axis angles.
- In the pose setup process, you need to turn off the motor's torque by pressing the 'Servo Off' button at the bottom right of the UI. To perform the pose move process after saving the position, turn on the torque of the motor by pressing the 'Servo on' button.

3) Since there is no addition to the current kinematics, it does not move to the user's desired three-dimensional coordinates. This part will be updated later.
- Please let me know if you have a useful kinematics library (open source).


- contact point(email): zzang0736@naver.com

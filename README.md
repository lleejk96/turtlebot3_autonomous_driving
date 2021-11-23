# turtlebot3_autonomous_driving
### 터틀봇3을 사용한 실내 자율 주행 프로젝트
##### 
해당 프로젝트는 터틀봇이라는 로봇키트를 사용하여 진행하였다. 터블봇에는 모터랑 바퀴가 달려 주행이 가능하다. 특히 lidar 센서와 라즈베리파이 카메라등 센서 등을 통해서 자율주행이 가능하도록 진행하였다.
ROS를 통해서 구현하였고, CPP를 사용하였다.
터틀봇에는 라즈베리파이를 탑재하여 SSH 프로토콜을 통해서 노트북으로 원격 제어를 하였다.

라즈베리파이는 Ubuntu 16.04에서 진행하였고 ROS version은 kinetic이다.   
Remote PC는 Ubuntu 18.04에서 진행하였고 ROS version은 melodic이다.   

***
### 1. 하드웨어
  * 터틀봇3
    <img width="400" alt="car" src="https://user-images.githubusercontent.com/66461571/142986953-cfc70b4b-f6e4-4064-a7bc-2a066824f0b8.PNG">   
    
  * 라즈베리파이
    <img width="200" alt="car" src="https://user-images.githubusercontent.com/66461571/142987144-39c7321a-4d50-462f-950c-89a7f1a2d1e8.jpg">
    
  * Lidar Sensor
    <img width="200" alt="car" src="https://user-images.githubusercontent.com/66461571/142987284-0fec72c2-d2f4-4898-9997-94dc60782004.PNG">
    
   * 라즈베리파이 카메라
    <img width="200" alt="car" src="https://user-images.githubusercontent.com/66461571/142987439-933c34df-a0cc-434b-934c-2167be320d6c.PNG">
    
    
 ### 2. 프로젝트 진행
 #### 1. remote pc에 ros 설치 

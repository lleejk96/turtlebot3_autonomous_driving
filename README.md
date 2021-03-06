# turtlebot3_autonomous_driving
### 터틀봇3을 사용한 실내 자율 주행 프로젝트
##### 
해당 프로젝트는 터틀봇이라는 로봇키트를 사용하여 진행하였다. 터블봇에는 모터랑 바퀴가 달려 주행이 가능하다. 특히 lidar 센서와 라즈베리파이 카메라등 센서 등을 통해서 자율주행이 가능하도록 진행하였다.
ROS를 통해서 구현하였고, CPP를 사용하였다.
터틀봇에는 라즈베리파이를 탑재하여 SSH 프로토콜을 통해서 노트북으로 원격 제어를 하였다.

라즈베리파이는 Ubuntu 16.04에서 진행하였고 ROS version은 kinetic이다.   
Remote PC는 Ubuntu 18.04에서 진행하였고 ROS version은 melodic이다.   

***
## 1. 하드웨어
  * 터틀봇3
    <img width="400" alt="car" src="https://user-images.githubusercontent.com/66461571/142986953-cfc70b4b-f6e4-4064-a7bc-2a066824f0b8.PNG">   
    
  * 라즈베리파이
    <img width="200" alt="car" src="https://user-images.githubusercontent.com/66461571/142987144-39c7321a-4d50-462f-950c-89a7f1a2d1e8.jpg">
    
  * Lidar Sensor
    <img width="200" alt="car" src="https://user-images.githubusercontent.com/66461571/142987284-0fec72c2-d2f4-4898-9997-94dc60782004.PNG">
    
  * 라즈베리파이 카메라
    <img width="200" alt="car" src="https://user-images.githubusercontent.com/66461571/142987439-933c34df-a0cc-434b-934c-2167be320d6c.PNG">
    
    
 ## 2. 프로젝트 진행
 #### 1. remote pc에 ros 설치
   ros Document를 통해서 ros melodic을 설치 <http://wiki.ros.org/melodic/Installation/Ubuntu>
   
 #### 2. ros workspace 생성
   ros 패키지를 담을 workspace가 필요하다. workspace를 생성하여 ros를 통해 개발한 패키지를 저장하고 빌드할 수 있게 해준다.
    
    $ mkdir -p ~/catkin_ws/src
    $ cd ~/catkin_ws/src
    $ catkin_init_workspace
    
 #### 3. ros package 생성
   ros 패키지는 ros 프로그램을 빌드하고 실행하기 위한 최속 구성단위이다. catkin 빌드 시스템을 이용해 ros 패키지를 빌드한다.
   해당 ros 패키지를 통해서 자율 주행 코드를 작성하고, 터틀봇과 Topic 통신을 한다. 
   
    $ cd ~/catkin_ws/src
    $ catkin_create_pkg turtlebot std_msgs rospy roscpp
    $ cd ~/catkin_ws
    $ catkin_make
    $ source ~/catkin_ws/devel/setup.bash
    
 #### 4. followbot node 생성
1. 센서들에 Topic 통신할 publisher와 subscriber 선언   
   센서들과 topic 통신을 하기 위해서 터틀봇에 topic을 전송할 publisher와 센서값을 받아올 subscriber를 선언하였다.   
   
<pre><code>
ros::Publisher cmd_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);
image_transport::Subscriber img_sub = it.subscribe("camera/rgb/image_raw",1,img_cb);
ros::Subscriber scan_sub = n.subscribe<sensor_msgs::LaserScan>("scan",1,scan_cb);
</code></pre>

2. callback 함수 img_cb로 카메라 데이터 비전 처리
   - 카메라 영상이 RGB로 가져오기 때문에 차선을 인식하기 위해서는 색상이 필요하다. 트랙에는 white 라인과 yellow 라인으로 구성되어 있다.
   - 카메라의 모든 영상들을 사용할 필요는 없다. 그래서 비전 처리할 구역을 차선이 보이는 구역에 지정하였다.
   - 해당 라인이 인식되었다고 보여줄 필요가 있다. 
   
3. callback 함수 scan_cb를 통해 라이더 값을 이용하여 장애물 회피
   - 주행 도중에 장애물이 나타날 수 있다. 이때 라이더 센서를 통해 장애물을 감지할 수 있다. 라이더 센서 데이터 바탕으로 회피 각도를 지정해주었다.

4. while문을 통해 터틀봇에 Twist 값들을 전송하여 주행시작
   - 반복을 통해서 터틀봇에게 계속 publish를 해주어 주행을 하게 해주어야 한다. 물체가 있을때랑 없을때랑 움직을 다르게 하였다.

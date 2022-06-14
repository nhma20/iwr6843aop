# iwr6843aop
IWR6843AOP C++ ROS2 pointcloud reader

Modified from: https://github.com/ACChallenge2021/iwr6843aop

Clone in source directory of ROS2 workspace. To build only this package:
```
colcon build --packages-select iwr6843aop
```

Run:
```
ros2 run iwr6843aop iwr6843aop_test --ros-args -p cfg_path:=<path/to/config.cfg>
```

TODO:
- Point to .cfg file in folder instead of fixed path

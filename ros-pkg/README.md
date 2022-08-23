# ROS package to connect rover with the car

Usage:

1. Link/Copy this directory to your ROS workspace.
2. Compile it with `catkin_make`.
3. Test it: In different terminals run:
   - `roscore`
   - `devel/lib/ipa2x_car_connect/ipa2x_car_connect` optionally with
     `--server`
   - `devel/lib/ipa2x_car_connect/rover-sim`
4. Run the Android application and see how the warning appears
   periodically.

# invisible-ai-takehome

To run:

```
mkdir build
cd build
cmake ..
make
./takehome ../data
```

All the code is in `main.cpp`. There is a `CameraBase` class that takes in a path to a camera data file and defines two functions: `readData()` and `updateCounts()`. `updateCounts()` is defined but `readData()` must be defined when instantiating an object of type `CameraDerived` (the inherited class). There is also a function called `processCamera()` which reads the camera data and updates the `counts` map accordingly. The `counts` map is defined in the `main` function and is a mapping from frame index to number of true values that frame had across all the cameras. Each camera spawns its own thread that calls the `processCamera()` function. When all threads are done executing, `main` aggregates and prints out the appropriate data.

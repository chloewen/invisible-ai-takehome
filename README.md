# invisible-ai-takehome

To run:

```
mkdir build
cd build
cmake ..
make
./takehome ../data
```

All the code is in `main.cpp`. Within `main.cpp`, there are 2 functions: `main()` and `updateCounts()`. `main()` initializes a map `counts` of frame indexes to the number of true counts for that frame. `updateCounts()` takes the path of a camera output file and pointer to `counts`, reads that camera output, and accordingly updates the map. Within `main()`, each camera spawns a new thread that calls `updateCounts()`. The values of `counts` are atomic, so that if two threads are updating the same value, the value is guaranteed to be incremented twice. Finally, when all threads are done executing, `main()` will go through `counts()` and print out the desired data.

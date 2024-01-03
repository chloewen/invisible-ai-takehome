#include <iostream>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <filesystem>
#include <sys/stat.h>
#include <thread>
#include <atomic>
using namespace std;
namespace fs = std::filesystem;

// dataFileName: path to a camera output file
// counts: pointer to counts map created in main()
// updateCounts() reads the output in dataFileName and updates counts accordingly
int updateCounts(string dataFileName, map<int,atomic<int>> *counts) {
  // open and read camera file
  ifstream camFile;
  camFile.open(dataFileName);
  string buf;
  if (camFile.is_open()) {
    while (getline(camFile, buf)) {
      int frameIdx;
      char frameVal[5];
      const char *bufArr = buf.c_str();
      sscanf(bufArr, "%d, %s", &frameIdx, frameVal);
      // update map with counts
      if (strncmp(frameVal, "true", 4) == 0) {
        (*counts)[frameIdx]++;
      }  
    }
    camFile.close();
  } else {
    cout << "Couldn't open file\n";
    return 1;
  }

  return 0;
}

int main(int argc, char **argv) {
  // create data structure mapping frame indexes to count
  map<int,atomic<int>> counts; 
  int numCameras = 0; // for overall counts
  // spawn new thread per camera
  for (const auto & entry : fs::directory_iterator(argv[1])) {
    string dataFileName = entry.path();
    thread t(&updateCounts, dataFileName, &counts);
    numCameras++;
    t.join(); // thread needs to finish executing before main starts aggregating data
  }

  // aggregate results
  int majTrueCount = 0; 
  int anyTrueCount = 0; 
  int allTrueCount = 0; 
  for (const auto & pair: counts) {
    if (pair.second > numCameras / 2) {
      majTrueCount++;
    }
    if (pair.second > 0) {
      anyTrueCount++;
    }
    if (pair.second == numCameras) {
      allTrueCount++;
    }
  }

  // print output
  cout << "Frames with majority true: " << majTrueCount << endl;
  cout << "Frames with any true: " << anyTrueCount << endl;
  cout << "Frames with all true: " << allTrueCount << endl;
  
  return 0;
}
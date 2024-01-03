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
using namespace std;
namespace fs = std::filesystem;


int getCounts(string dataFileName, map<int,int> *counts) {
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
  map<int,int> counts; 
  int numCameras = 0;
  for (const auto & entry : fs::directory_iterator(argv[1])) {
    string dataFileName = entry.path();
    thread t(&getCounts, dataFileName, &counts);
    // if (getCounts(dataFileName, &counts)) return 1;
    numCameras++;
    t.join();
  }
  

  // print counts (TODO: delete later)
  for (const auto & pair: counts) {
    cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
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

  cout << "Frames with majority true: " << majTrueCount << endl;
  cout << "Frames with any true: " << anyTrueCount << endl;
  cout << "Frames with all true: " << allTrueCount << endl;
  
  return 0;
}
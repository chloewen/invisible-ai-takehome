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
#include <mutex>
#include <functional>
#include <vector>
using namespace std;
namespace fs = std::filesystem;

mutex counts_map_mutex; 

class CameraBase {
  public: 
    string dataFileName;
    virtual vector<int> readData(string dataFileName) {throw logic_error("Not implemented");};
    virtual void updateCounts(vector<int> trueFrameIdxs, map<int,int>* counts) {throw logic_error("Not implemented");};
};

class CameraDerived : public CameraBase {
  public: 
    vector<int> readData(string dataFileName) {
      ifstream camFile;
      camFile.open(dataFileName);
      string buf;
      vector<int> trueFrameIdxs; 
      if (camFile.is_open()) {
        while (getline(camFile, buf)) {
          int frameIdx;
          char frameVal[5];
          const char *bufArr = buf.c_str();
          sscanf(bufArr, "%d, %s", &frameIdx, frameVal);
          if (strncmp(frameVal, "true", 4) == 0) {
            trueFrameIdxs.push_back(frameIdx);
          }  
        }
        camFile.close();
      } else {
        cout << "Couldn't open file\n";
      }
      return trueFrameIdxs;
    }

    void updateCounts(vector<int> trueFrameIdxs, map<int,int> *counts) {
      for (auto i = trueFrameIdxs.begin(); i != trueFrameIdxs.end(); i++) {
        std::lock_guard<mutex> guard(counts_map_mutex); // lock counts with every insert
        (*counts)[*i]++;
      }
    }
};

void processCamera(CameraDerived camera, map<int,int> *counts) {
  vector<int> trueFrameIdxs = camera.readData(camera.dataFileName);
  camera.updateCounts(trueFrameIdxs, counts);
}

int main(int argc, char **argv) {
  // create data structure mapping frame indexes to count
  map<int,int> counts; 
  int numCameras = 0; // for overall counts
  // spawn new thread per camera
  for (const auto & entry : fs::directory_iterator(argv[1])) {
    string dataFileName = entry.path();
    CameraDerived camera;
    camera.dataFileName = dataFileName;
    thread t(&processCamera, camera, &counts);
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
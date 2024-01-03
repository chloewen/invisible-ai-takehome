#include <iostream>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
using namespace std;

int getCounts(string dataFileName, map<int,int> *counts) {
  // open and read camera file
  ifstream camFile;
  camFile.open(dataFileName);
  string buf;
  if (camFile.is_open()) {
    while (getline(camFile, buf)) {
      int frameIdx;
      char *frameVal;
      char * bufArr = new char[buf.length() + 1];
      strcpy(bufArr, buf.c_str());
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
  cout << "test" << argv[1] << endl;
  // create data structure mapping frame indexes to count
  map<int,int> counts; 
  int numCameras = 6; // TODO: fix
  for (int i = 0; i < numCameras; i++) {
    string dataFileName = string(argv[1]) + "/cam" + to_string(i) + ".txt";
    if (getCounts(dataFileName, &counts)) {
      return 1;
    }
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
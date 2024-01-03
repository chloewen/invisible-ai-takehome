#include <iostream>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
using namespace std;

int getCounts(string dataFileName, unordered_map<int,int> *counts) {
  // open and read camera file
  ifstream camFile;
  camFile.open(dataFileName);
  string buf;
  if ( camFile.is_open() ) {
    while (getline(camFile, buf)) {
      
      int frameIdx;
      char *frameVal;
      char * bufArr = new char[buf.length() + 1];
      strcpy(bufArr, buf.c_str());
      sscanf(bufArr, "%d, %s", &frameIdx, frameVal);
      cout << buf << frameIdx << frameVal << '\n';
    }
    camFile.close();
  } else {
    cout << "Couldn't open file\n";
    return 1;
  }

  // // print counts (TODO: delete later)
  // for (const auto & pair: *counts) {
  //   cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
  // }
  return 0;
}

int main(int argc, char **argv) {
  cout << "test" << argv[1] << endl;
  // create data structure mapping frame indexes to count
  unordered_map<int,int> counts; 
  string dataFileName = string(argv[1]) + "/cam0.txt";
  if (getCounts(dataFileName, &counts)) {
    return 1;
  }
  
  return 0;
}
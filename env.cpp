#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <cerrno>
#include <limits.h>

using namespace std;

// PROTOTYPES

/**
 * Prints the string error corresponding to the set errno value to stdout,
 * then exits with failure code.
 */
void failure();

// MAIN

int main(int argc, const char * argv[], const char * envp[]) {

  cout.setf(std::ios::unitbuf);
  cin.setf(std::ios::unitbuf);

  if(argc != 1) { cout << "Usage: ./env" << endl; exit(EXIT_FAILURE); } 

  for(const char ** env = envp; *env != nullptr; env++) {
    const char* var = *env;
    cout << var << endl;
  } // for

  return EXIT_SUCCESS;
} // main

// DEFINITIONS

void failure() {
  int err = errno;
  cout << strerror(err) << endl;
  exit(EXIT_FAILURE);
} // failure


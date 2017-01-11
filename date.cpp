#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <cerrno>
#include <ctime>
#include <time.h>

using namespace std;

// PROTOTYPES

/**
 * Prints the string error corresponding to the set errno value to stdout,
 * then exits with failure code.
 */
void failure();

// MAIN

int main(int argc, char * argv[]) {

  cout.setf(std::ios::unitbuf);
  cin.setf(std::ios::unitbuf);

  char output[200];
  time_t rawt;
  struct tm * t = nullptr;

  if(time(&rawt) == ((time_t) -1)) { // time sys call fails
    failure();
  } // if
  
  if((t = localtime(&rawt)) == nullptr) {
    failure();
  } // if

  if(argc == 1) { // default date/time representation
    const char dformat[] = "%a %b %d %X %Z %Y";
    if(strftime(output,sizeof(output),dformat,t) == 0) {
      failure();
    } // if
  } else { // chosen date/time representation
    string format = argv[1];
    if(format[0] != '+' || format.length() == 1) {
      cout << "Usage: ./date [+format]" << endl;
      exit(EXIT_FAILURE);
    } // if
    format = format.substr(1); // cuts off the '+'
    if(strftime(output,sizeof(output),format.c_str(),t) == 0) {
      failure();
    } // if    
  } // if/else
  
  cout << string(output) << endl;

  return EXIT_SUCCESS;
} // main

// DEFINITIONS

void failure() {
  int err = errno;
  cout << strerror(err) << endl;
  exit(EXIT_FAILURE);
} // failure


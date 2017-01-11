#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cerrno>

using namespace std;

void nope_out(const string & prefix);

/**
 * the main function of ln.cpp checks arguments in order to create either a hard link between files, or a symbolic link
 * if the -s flag is included. If the file does not exist, then nope_out is called and the appropriate error is printed.
 */

int main(const int argc, const char * argv[]){

  cout.setf(std::ios::unitbuf);
  cin.setf(std::ios::unitbuf);

  if(argc == 3 && string(argv[1]) != "-s") if(link(argv[1], argv[2]) == -1) nope_out("link");
  if(argc == 4 && string(argv[1]) == "-s") if(symlink(argv[2], argv[3]) == -1) nope_out("symlink");
  return EXIT_SUCCESS;
}//main

void nope_out(const string & prefix) {
  perror(prefix.c_str());
  exit(EXIT_FAILURE);
} // nope_out

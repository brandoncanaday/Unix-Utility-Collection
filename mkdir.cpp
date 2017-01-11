#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <cerrno>
#include <limits.h>
#include <sys/stat.h>
#include <vector>

using namespace std;

// PROTOTYPES

/**
 * Prints the string error corresponding to the set errno value to stdout,
 * then exits with failure code.
 */
void failure();

/**
 * Sets the appropriate flag toggle based on the given command line arg.
 * Will fail if the usage requirements are not met.
 *
 * @param const char* the given command line arg from argv[]
 */
void parse_arg(const char*);

/*
 * Checks if given command line arg is a number or not.
 *
 * @param const char* the given command line arg from argv[]
 * @return true if is number, false if not
 */
bool isNumber(const char*);

// GLOBALS

bool p_toggle = false, m_toggle = false;

// MAIN

int main(int argc, char * argv[]) {

  cout.setf(std::ios::unitbuf);
  cin.setf(std::ios::unitbuf);
  
  //  mode_t old_umask = umask(022); // makes all dir's created have default permissions of 755 (rwxr-xr-x)
  mode_t default_mode = 0777;
  mode_t user_input_mode = default_mode;
  int index_of_perm = 0;

  // parses args
  for(int i = 1; i < argc; i++) {
    parse_arg(argv[i]); // sets the global flag toggles
  } // for

  // loops through args to look for octal permission. first num is taken as octal, all after are treated as pathnames
  int numCount = 0;
  for(int i = 1; i < argc; i++) {
    if(isNumber(argv[i])) {
      if(m_toggle) {
	string s_num = string(argv[i]);
	index_of_perm = i;
	numCount++;
	if(numCount > 1) {
	  cout << "Usage: ./mkdir [-p] [-m mode] dir..." << endl;
	  exit(EXIT_FAILURE);	  
	} // 
	try {
	  user_input_mode = stoi(s_num,0,8);
	  break;
	} catch(invalid_argument invalidargument) {
	  cout << "./mkdir: Invalid octal value" << endl;
	  exit(EXIT_FAILURE);	  	  
	} catch(out_of_range outofrange) {
	  cout << "./mkdir: Invalid octal value" << endl;
	  exit(EXIT_FAILURE);	  	  
	} // try/catch
      } // if
    } // if
  } // for
  
  // final check for octal permissions
  if(m_toggle && numCount == 0) {
    cout << "Usage: ./mkdir [-p] [-m mode] dir..." << endl;
    exit(EXIT_FAILURE);	     
  } // if
  
  // loops through args to look for dir
  int dir_total = 0;
  for(int i = 1; i < argc; i++) { // starts at arg following './wc'
    //skips arg if a flag OR i == index of first octal permission
    if(string(argv[i]) == "-p" || string(argv[i]) == "-m" ||  string(argv[i]) == "-pm" || 
       string(argv[i]) == "-mp" || i == index_of_perm) {
      continue;
    } // if
    
    // found pathname
    if(p_toggle) { // create intermediate dir if doesn't exist
      dir_total++;
      umask(022);
      string path = argv[i];
      char * p = nullptr;

      // makes all intermediate dirs before final dir
      for(p=strchr(argv[i]+1, '/'); p; p=strchr(p+1, '/')) { 
	*p='\0';
	if (mkdir(argv[i], default_mode) == -1) { // makes dir w/ permissions 755
	  if (errno!=EEXIST) { *p='/'; break; } // if
	} // if
	*p='/';
      } // for

      // makes final dir in path
      if(m_toggle) {
	umask(000);
	if(mkdir(path.c_str(),user_input_mode) == -1) { // makes final dir w/ chosen permissions
	  int err = errno;
	  cout << "./mkdir: " << strerror(err) << endl;
	  if(i == argc-1) { exit(EXIT_FAILURE); } // if last arg
	} // if
      } else {
	if(mkdir(path.c_str(),default_mode) == -1) { // makes final dir w/ permissions 755
	  int err = errno;
	  cout << "./mkdir: " << strerror(err) << endl;
	  if(i == argc-1) { exit(EXIT_FAILURE); } // if last arg
	} // if
      } // if/else
      umask(022); // just in case

    } else { // throw error if intermediate dir doesn't exist
      dir_total++;
      if(access(argv[i],F_OK) == -1) { // target dir doesn't exist
	mode_t mode = (m_toggle) ? user_input_mode : default_mode;
	if(m_toggle) { umask(000); } // if
	if(mkdir(argv[i],mode) == -1) {
	  int err = errno;
	  cout << "./mkdir: cannot create directory `" << argv[i] << "': " << strerror(err) << endl;
	  if(i == argc-1) { exit(EXIT_FAILURE); } // if last arg
	} // if
	umask(022);
      } // if
    } // if/else
  } // for
  
  // dir_total == 0: prints usage statement
  if(dir_total == 0) {
    cout << "Usage: ./mkdir [-p] [-m mode] dir..." << endl;
    exit(EXIT_FAILURE);	  
  } // if

  return EXIT_SUCCESS;
} // main

// DEFINITIONS

void failure() {
  int err = errno;
  cout << strerror(err) << endl;
  exit(EXIT_FAILURE);
} // failure

void parse_arg(const char * arg) {
  if(string(arg) == "-p") {
    if(!p_toggle) {
      p_toggle = true;
    } else {
      cout << "Usage: ./mkdir [-p] [-m mode] dir..." << endl;
      exit(EXIT_FAILURE);
    } // if/else
  } else if(string(arg) == "-m") {
    if(!m_toggle) {
      m_toggle = true;
    } else {
      cout << "Usage: ./mkdir [-p] [-m mode] dir..." << endl;
      exit(EXIT_FAILURE);
    } // if/else
  } else if(string(arg) == "-mp") {
    if(!m_toggle && !p_toggle) {
      m_toggle = true;
      p_toggle = true;
    } else {
      cout << "Usage: ./mkdir [-p] [-m mode] dir..." << endl;
      exit(EXIT_FAILURE);
    } // if/else
  } else if(string(arg) == "-pm") {
    if(!p_toggle && !m_toggle) {
      p_toggle = true;
      m_toggle = true;
    } else {
      cout << "Usage: ./mkdir [-p] [-m mode] dir..." << endl;;
      exit(EXIT_FAILURE);
    } // if/else
  } // if/else
} // parse_arg

bool isNumber(const char * arg) {
  for(int i = 0; arg[i] != 0; i++) {
    if(!isdigit(arg[i])) { //if (number[i] > '9' || number[i] < '0')
      return false;
    } // if
  } // for
  return true;
} // isNumber


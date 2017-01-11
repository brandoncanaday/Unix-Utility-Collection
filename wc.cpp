#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <cerrno>
#include <limits.h>
#include <sys/stat.h>

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

/**
 * Determines from the set toggles if the program should output the default lines, words, and bytes.
 *
 * @return true if none of the toggles have been set, false if not
 */
bool isDefaultOutput();

// ENUMS/STRUCTS

enum states { SPACE, LETTER };

// GLOBALS

bool c_toggle = false, m_toggle = false, l_toggle = false, w_toggle = false;

// MAIN

int main(int argc, const char * argv[]) {

  cout.setf(std::ios::unitbuf);
  cin.setf(std::ios::unitbuf);
  
  int file_total = 0;
  int b_total = 0;
  int c_total = 0;
  int l_total = 0;
  int w_total = 0;

  // parses args
  for(int i = 1; i < argc; i++) {
    parse_arg(argv[i]); // sets the global flag toggles
  } // for
  
  // loops through args to look for files
  for(int i = 1; i < argc; i++) { // starts at arg following './wc'
    int fd = -1;
    struct stat info;
    char buf[1];
    int nBytes = 0;
    int b_count = 0;
    int c_count = 0;
    int l_count = 0;
    int w_count = 0;
    states current = SPACE, previous = SPACE;
    //skips arg if a flag
    if(string(argv[i]) == "-c" || string(argv[i]) == "-m" || 
       string(argv[i]) == "-l" || string(argv[i]) == "-w" || 
       string(argv[i]) == "-lw" || string(argv[i]) == "-wl") {
      continue;
    } // if
    if(string(argv[i]) == "-") { // stdin
      fd = STDIN_FILENO;
      file_total++;
    } else { // reg file
      if((fd = open(argv[i],O_RDONLY)) == -1) { // open fails
	continue; // skip this arg
      } // if
      if(stat(argv[i],&info) == -1) {
	continue;
      } // if
      file_total++;
    } // if/else
    while((nBytes = read(fd,buf,1)) > 0) {
      previous = current;
      b_count+=nBytes;
      c_count+=nBytes;
      if(buf[0] == '\n') { 
	l_count++; 
	current = SPACE;
      } else if(buf[0] == ' ' || buf[0] == '\t') {
	current = SPACE;
      } else if(buf[0] != '\n' && buf[0] != ' ') {
	current = LETTER;
      } // if/else
      if(current == LETTER && previous == SPACE) { w_count++; } // if
    } // while
    if(fd != STDIN_FILENO) {
      if(close(fd) == -1) { failure(); } // if
    } // if
    if(fd == STDIN_FILENO) {
      if(isDefaultOutput()) {
	cout << l_count << " " << w_count << " " << b_count << " ";
      } else {
	if(c_toggle) { cout << b_count << " "; } // if
	if(m_toggle) { cout << c_count << " "; } // if
	if(l_toggle) { cout << l_count << " "; } // if
	if(w_toggle) { cout << w_count << " "; } // if
      } // if/else
      cout << "-" << endl;
    } else {
      if(S_ISDIR(info.st_mode)) {
	cout << "./wc: " << argv[i] << ": " << "Is a directory" << endl;
      } // if
      if(isDefaultOutput()) {
	cout << l_count << " " << w_count << " " << b_count << " ";
      } else {
	if(c_toggle) { cout << b_count << " "; } // if
	if(m_toggle) { cout << c_count << " "; } // if
	if(l_toggle) { cout << l_count << " "; } // if
	if(w_toggle) { cout << w_count << " "; } // if
      } // if/else
      cout << argv[i] << endl;
    } // if/else
    b_total += b_count;
    c_total += c_count;
    l_total += l_count;
    w_total += w_count;
  } // for
  
  // file_total > 1: prints totals. file_total == 1: does nothing. file_total == 0: reads from stdin
  if(file_total > 1) {
    if(isDefaultOutput()) {
      cout << l_total << " " << w_total << " " << b_total << " ";      
    } else {
      if(c_toggle) { cout << b_total << " "; } // if
      if(m_toggle) { cout << c_total << " "; } // if
      if(l_toggle) { cout << l_total << " "; } // if
      if(w_toggle) { cout << w_total << " "; } // if
    } // if/else
    cout << "total" << endl;
  } else if(file_total == 0) { // user input either './wc' or './wc [flags...]'. no files either way
    char buf[1];
    int nBytes = 0;
    int b_count = 0;
    int c_count = 0;
    int l_count = 0;
    int w_count = 0;
    states current = SPACE, previous = SPACE;
    while((nBytes = read(STDIN_FILENO,buf,1)) > 0) {
      previous = current;
      b_count+=nBytes;
      c_count+=nBytes;
      if(buf[0] == '\n') { 
	l_count++; 
	current = SPACE;
      } else if(buf[0] == ' ' || buf[0] == '\t') {
	current = SPACE;
      } else if(buf[0] != '\n' && buf[0] != ' ') {
	current = LETTER;
      } // if/else
      if(current == LETTER && previous == SPACE) { w_count++; } // if
    } // while
    if(argc == 1) { // user input './wc'
      cout << "l: " << l_count << " " << "w: " << w_count << " " << "b: " << b_count;
    } else { // user input './wc [flags...]'
      if(c_toggle) { cout << "b: " << b_count << " "; } // if
      if(m_toggle) { cout << "c: " << c_count << " "; } // if
      if(l_toggle) { cout << "l: " << l_count << " "; } // if
      if(w_toggle) { cout << "w: " << w_count << " "; } // if
    } // if/else
    cout << endl;
  } // if/else

  return EXIT_SUCCESS;
} // main

// DEFINITIONS

void failure() {
  int err = errno;
  cout << strerror(err) << endl;
  exit(EXIT_FAILURE);
} // failure

void parse_arg(const char * arg) {
  if(string(arg) == "-c") {
    if(!c_toggle && !m_toggle) {
      c_toggle = true;
    } else {
      cout << "Usage: ./wc [-c | -m] [-lw] [file...]" << endl;
      exit(EXIT_FAILURE);
    } // if/else
  } else if(string(arg) == "-m") {
    if(!m_toggle && !c_toggle) {
      m_toggle = true;
    } else {
      cout << "Usage: ./wc [-c | -m] [-lw] [file...]" << endl;
      exit(EXIT_FAILURE);
    } // if/else
  } else if(string(arg) == "-l") {
    if(!l_toggle) {
      l_toggle = true;
    } else {
      cout << "Usage: ./wc [-c | -m] [-lw] [file...]" << endl;
      exit(EXIT_FAILURE);
    } // if/else
  } else if(string(arg) == "-w") {
    if(!w_toggle) {
      w_toggle = true;
    } else {
      cout << "Usage: ./wc [-c | -m] [-lw] [file...]" << endl;
      exit(EXIT_FAILURE);
    } // if/else
  } else if(string(arg) == "-lw" || string(arg) == "-wl") {
    if(!l_toggle && !w_toggle) {
      l_toggle = true;
      w_toggle = true;
    } else {
      cout << "Usage: ./wc [-c | -m] [-lw] [file...]" << endl;
      exit(EXIT_FAILURE);
    } // if/else
  } // if/else
} // parse_arg

bool isDefaultOutput() {
  return (c_toggle == false && m_toggle == false && l_toggle == false && w_toggle == false);
} // isDefaultOutput


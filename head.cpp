#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

void nope_out(const string & prefix);                

/**
 * the main function first sets stdout and stdin to unbuffered. A for loop is then used to check for flags and set the correct
 * number of lines to be printed. If statements are then used to cycle between the different argument combinations, as well as 
 * to account for arguments that call standard output. 
 * Usage: ./head [-n number] [file...]
 */

int main(const int argc, const char * argv[]){

  cout.setf(std::ios::unitbuf);
  cin.setf(std::ios::unitbuf);

  const char * filename = nullptr;
  char buffer[4096] = "";
  char output[4096] = "";
  bool ntoggle = false;
  int nbytes = 0;
  int count = 0;
  int ofd;
  int nVal = -1;

  for(int i = 1; i < argc; ++i){
    if(string(argv[i]) == "-n") ntoggle = true; 
    if(isdigit(*argv[i])) nVal = atoi(argv[i]);
  }//for
  if(nVal == -1 && ntoggle){
    cout << "head: option requires an argument -- 'n'" << endl;
    exit(EXIT_FAILURE);
  }//if     
 
  if(argc >= 2 && !ntoggle){
    for(int i = 1; i < argc; ++i){
      if(string(argv[i]) != "-") {
	filename = argv[i];
	if(argc != 2) cout << "==> " << argv[i] << " <==" << endl;
	if ((ofd = open(filename, O_RDONLY)) == -1) {
	  nope_out("open");
	} // if    
	
	while((nbytes = read(ofd, buffer, 1)) > 0 && count < 10){
	  if(buffer[0] == '\n') count++;
	  cout << buffer;
	}//while
      }//if
      else{
	cout << "==> " << "standard output" << " <==" << endl;
	while((nbytes = read(0, output, 4096)) > 0 && count < 10) {
	  count++;
	  write(1, output, nbytes);
	  if(count == 10) break;
	}//while
      }//else
	count = 0;
	if(i != argc-1) cout << endl;
    }//if
  }//if
  else if(argc >= 4 && ntoggle){
    for(int i = 1; i < argc; ++i){
      if(string(argv[i]) != "-n" && !isdigit(*argv[i])){
	if(string(argv[i]) != "-") {
	  filename = argv[i];
	  cout << "==> " << argv[i] << " <==" << endl;
	  if((ofd = open(filename, O_RDONLY)) == -1) {
	    nope_out("open");
	  } // if    
	  while((nbytes = read(ofd, buffer, 1)) > 0 && count < nVal){
	    if(buffer[0] == '\n') count++;
	    cout << buffer;
	  }//if
	}//if
	else{
	  cout << "==> " << "standard output" << " <==" << endl;
	  while((nbytes = read(0, output, 4096)) > 0 && count < nVal) {
	    count++;
	    write(1, output, nbytes);
	    if(count == nVal) break;
	  }//while	  
	}//else
	count = 0;
	if(i != argc-1) cout << endl;
      }//if
    }//for
  }//elseif
  else if(argc == 1 || (argc == 2 && string(argv[1]) == "-")){
     while((nbytes = read(0, buffer, 4096)) > 0 && count < 9) {
       count++;
       write(1, buffer, nbytes);
     }//while
     count = 0;
  }//elseif
    else if(argc == 3 && ntoggle){
     while((nbytes = read(0, buffer, 4096)) > 0 && count < nVal) {
       count++;
       write(1, buffer, nbytes);
       if(count == nVal) break;
     }//while
     count = 0;
  }//elseif

  close(ofd);
  return EXIT_SUCCESS;
}//main

void nope_out(const string & prefix) {
  perror(prefix.c_str());
  //exit(EXIT_FAILURE);
} // nope_out

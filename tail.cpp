#include <cstdlib>
#include <ctype.h>
#include <iostream>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

const char * filename = nullptr;
const char * temp = nullptr;
char input[4096] = "";
bool ftoggle = false, ctoggle = false, ntoggle = false;
int nVal = -1, cVal = -1, fd = -1, nBytes = 0;

void tail(const char *, const char *, int);
void nope_out(const string & prefix);                

/**
 * the main function first sets stdout and stdin to unbuffered. A for loop is then used to check for flags and set the correct
 * number of lines/bytes to be printed. If statements are then used to cycle between the different argument combinations, 
 * as well as to account for arguments that call standard output. 
 * Usage: ./tail [-c number | -n number] [file...]
 */


int main(const int argc, const char * argv[]){

  cout.setf(std::ios::unitbuf);
  cin.setf(std::ios::unitbuf);

   for(int i = 1; i < argc; ++i){
     if(string(argv[i]) == "-f") ftoggle = true;
     if(string(argv[i]) == "-c") ctoggle = true;
     if(string(argv[i]) == "-n") ntoggle = true; 
     if(isdigit(*argv[i])) nVal = atoi(argv[i]);
     if(isdigit(*argv[i])) cVal = atoi(argv[i]);
   }//for

  if(argc >= 2 && !ftoggle && !ctoggle && !ntoggle){
    for(int i = 1; i < argc; ++i){
      if(argc != 2) {
	if(string(argv[i]) != "-") cout << "==> " << argv[i] << " <==" << endl;
	else cout << "==> " << "standard output" << " <==" << endl;
      }//if	 
      tail(argv[i], argv[i], 10);
    }//for
  }//if
   else if(argc >= 3 && ntoggle){
     if(nVal == -1){
       cout << "tail: option requires an argument -- 'n'" << endl;
       exit(EXIT_FAILURE);
     }//if     
     for(int i = 1; i < argc; ++i){
       if(string(argv[i]) != "-n" && string(argv[i]) != "-f" && !isdigit(*argv[i])) {
	 if(argc != 2) {
	   if(string(argv[i]) != "-") cout << "==> " << argv[i] << " <==" << endl;
	   else cout << "==> " << "standard output" << " <==" << endl;
	 }//if
	 tail(argv[i], argv[i], nVal);
       }//if
     }//for
     if(argc == 3){
       tail("-", "-", nVal);
     }//if    
     if(ftoggle){
       while((nBytes = read(fd, input, 4096)) > 0 || true){write(STDOUT_FILENO, input, nBytes);}
     }//if
  }//elseif
   else if(argc >= 3 && ctoggle){
     if(cVal == -1){
       cout << "tail: option requires an argument -- 'c'" << endl;
       exit(EXIT_FAILURE);
     }//if     
     for(int i = 1; i < argc; ++i){
       if(string(argv[i]) != "-c" && string(argv[i]) != "-f" && !isdigit(*argv[i])) {
	 if(argc != 2) {
	   if(string(argv[i]) != "-") cout << "==> " << argv[i] << " <==" << endl;
	   else cout << "==> " << "standard output" << " <==" << endl;
	 }//if
	 tail(argv[i], argv[i], cVal);
       }//if
     }//for
     if(argc == 3){
       tail("-", "-", cVal);
     }//if
     if(ftoggle){
       while((fd = read(0, input, 4096) || true) > 0){write(STDOUT_FILENO, input, nBytes);}
     }//if
  }//elseif
   else if(argc >= 2 && !ctoggle && !ntoggle){
      for(int i = 1; i < argc; ++i){
       if(string(argv[i]) != "-c" && string(argv[i]) != "-f" && !isdigit(*argv[i])) {
	 if(argc != 2) {
	   if(string(argv[i]) != "-") cout << "==> " << argv[i] << " <==" << endl;
	   else cout << "==> " << "standard output" << " <==" << endl;
	 }//if
	 tail(argv[i], argv[i], 10);
       }//if
     }//for
      if(ftoggle){
	while((nBytes = read(fd, input, 4096)) > 0 || true){write(STDOUT_FILENO, input, nBytes);}
      }//if
   }//elseif
  else if(argc == 1){
    char temp[4096];
    int index = 0;
    while((nBytes = read(0, input, 4096)) > 0){temp[index] = input[index]; ++index;}   
    for(unsigned int i = 0; i < sizeof(temp); ++i){
      cout << temp[i];
    }//for
    cout << endl;
  }//elseif

  return EXIT_SUCCESS;
}//main

/**
 * nope_out is the perror checker, which takes in a prefix, which is then translated into the appropriate error.
 */

void nope_out(const string & prefix) {
  perror(prefix.c_str());
} // nope_out

/**
 * tail is a helper function that takes in a file name, a copy, and the number of lines or bytes.
 * If the file name is "-", then most of the function is omitted and standard output waits for user input.
 */

void tail(const char * file1, const char * copy, int lines){
  char buffer[4096] = "", trunc[4096] = "";
  int nbytes = 0, nbytes2 = 0;
  int count = 0, count2 = 0;
  int ofd, ofd2;
  if(string(file1) != "-"){
    if((ofd = open(file1, O_RDONLY)) == -1) {
      nope_out("open");
    } // if    
    
    ofd2 = open(copy, O_RDONLY);
    
    while((nbytes = read(ofd, buffer, 1)) > 0){
      if(!ctoggle && buffer[0] == '\n') {
	count++;
      }//if	
      if(ctoggle) count++;
    }//while
    while((nbytes2 = read(ofd2, trunc, 1)) > 0){
      if(!ctoggle && trunc[0] == '\n') count2++;
      if(ctoggle) count2++;
      if(count2 >= count-lines && !ctoggle) {
	cout << trunc;
      }//if
      if(count2 > count-lines && ctoggle) {
	cout << trunc;
      }//if
    }//while
    count = 0;
  }//if
  else{
    ofd2 = open(copy, O_RDONLY);
    while((ofd = read(0, buffer, 4096)) > 0){write(ofd2, buffer, 4096);}
    while((ofd2 = read(ofd2, trunc, 4096)) > 0){write(ofd2, trunc, 4096);}
    cout << trunc;
  }//else
}//tail

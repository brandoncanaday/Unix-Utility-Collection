#include<iostream>
#include<unistd.h>
#include<cerrno>
#include<cstring>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<cstdlib>
#include<pwd.h>
#include<grp.h>
#include<string>
#include<dirent.h>
#include<vector>
#include<algorithm>
#include<time.h>

using namespace std;

void nope_out(const string & prefix);
void vectorize(const char * dirname);
void ls(const char * str);

struct stat st;
DIR * dirp = nullptr;               // directory pointer
struct dirent * direntp = nullptr;  // directory entry pointer
struct tm * tmp;

bool atoggle = false, ltoggle = false, dirtoggle = false;
vector<string> files;
string filename = "";
int fileStart = -1, total = 0;
time_t t;
char buffer[512] = "";
char outstr[80] = "";

/**
 * The main function of ls.cpp first sets stdout/stdin to unbuffered. A for-loop flag checker is then used and the appropriate
 * boolean toggles for flags are set appropriately. The argument combinations are then checked to see how the files are to be 
 * printed. The helper function ls (self-made) then prints out all necessary file info if the -l flag is set.
 * Usage: ./ls [-al] [file...]
 **/

int main(const int argc, const char * argv []){

  cout.setf(std::ios::unitbuf);
  cin.setf(std::ios::unitbuf);
  
  for(int i = 1; i < argc; ++i){
    if(string(argv[i]) == "-a") atoggle = true;
    if(string(argv[i]) == "-l") ltoggle = true;
    if(string(argv[i]) == "-al" || string(argv[i]) == "-la") { atoggle = true; ltoggle = true;}
  }//for
  
  for(int i = 1; i < argc; ++i){
    if(string(argv[i]) != "-al" 
       && string(argv[i]) != "-la"
       && string(argv[i]) != "-l"
       && string(argv[i]) != "-a") { 
      fileStart = i;
      filename = argv[i];
      break;
    }//if
  }//for

  if(argc == 1) vectorize(".");
  else{
    if(atoggle && !ltoggle) {
      if(fileStart == -1) vectorize(".");
      else {
	for(int i = fileStart; i < argc; ++i) {
	  stat(argv[i], &st);
	  if((st.st_mode & S_IFMT) == S_IFDIR) { dirtoggle = true; vectorize(argv[i]);}
	  else ls(argv[i]);
	}//for
      }//else
    }//if
    else if(!atoggle && ltoggle) {
      if(fileStart == -1) vectorize(".");
      else {
	for(int i = fileStart; i < argc; ++i) {
	  stat(argv[i], &st);
	  if((st.st_mode & S_IFMT) == S_IFDIR) { dirtoggle = true; vectorize(argv[i]);}	  
	  else ls(argv[i]);
	}//for
      }//else
    }//elseif
    else if(atoggle && ltoggle) {
      if(fileStart == -1) vectorize(".");
      else {
	for(int i = fileStart; i < argc; ++i) {
	  stat(argv[i], &st);
	  if((st.st_mode & S_IFMT) == S_IFDIR) { dirtoggle = true; vectorize(argv[i]);}
	  else ls(argv[i]);
	}//for
      }//else
    }//elseif
    else if(!atoggle && !ltoggle){
      for(int i = 1; i < argc; ++i){ 
	stat(argv[i], &st);
	if((st.st_mode & S_IFMT) == S_IFDIR) { dirtoggle = true; vectorize(argv[i]);}
	else ls(argv[i]); 
      }//for
    }//elseif
  }//else

  return EXIT_SUCCESS;
}//main

/**
 * ls is a helper function that prints out the necessary file info for a specific file.
 * A const char * is taken in as the file name. The correct file info is printed out according to the flags.
 **/

void ls(const char * str){
  if(access(str, F_OK) == 0){
    if (stat(str, &st) == -1) { perror("stat"); exit(EXIT_FAILURE); }
    if(stat(str, &st) == -1) { 
      if(lstat(str, &st) == -1) {
   	if(string(str) == "-") { 
   	  if(fstat(STDIN_FILENO, &st) == -1) {
   	    cout << strerror(errno) << endl;
   	    exit(EXIT_FAILURE);
   	  }//if
   	}//if
      }//if
    }//if
  }//if
  else {
    int err = errno;
    fprintf(stderr, "%s\n", strerror(err));
    exit(EXIT_FAILURE);
  }
  if((ltoggle && atoggle) || (ltoggle && !atoggle)){
    lstat(str, &st);
    switch(st.st_mode & S_IFMT){
    case S_IFDIR:
      cout << "d";
      break;
    case S_IFLNK:
      cout << "l";
      break;
    case S_IFREG:
      cout << "-";
      break;
    }
    
    cout << ((st.st_mode & S_IRUSR) ? "r" : "-"); 
    cout << ((st.st_mode & S_IWUSR) ? "w" : "-");     
    
    if((st.st_mode & S_IXUSR) && (st.st_mode &  S_ISUID)) { //changed from | to &
      cout << "s";
    } else if(st.st_mode & S_ISUID) {
      cout << "S";
    } else if(st.st_mode & S_IXUSR) { // was originally last
      cout << "x";
    }  else {
      cout << "-";
    }
    cout << ((st.st_mode & S_IRGRP) ? "r" : "-"); 
    cout << ((st.st_mode & S_IWGRP) ? "w" : "-"); 
    if((st.st_mode & S_IXOTH) && (st.st_mode & S_ISVTX)) {
      cout << "s";
    } else if(st.st_mode & S_ISGID) {
      cout << "S";
    } else if(st.st_mode & S_IXGRP) {
      cout << "x";
    }  else {
      cout << "-";
    }
    cout << ((st.st_mode & S_IROTH) ? "r" : "-");
    cout << ((st.st_mode & S_IWOTH) ? "w" : "-");
  if((st.st_mode & S_IXOTH) && (st.st_mode & S_ISVTX)) {
    cout << "t";
  } else if(st.st_mode & S_ISVTX) {
    cout << "T";
  } else if(st.st_mode & S_IXOTH) {
    cout << "x";
  } else {
    cout << "-";
  }
  cout << ". ";
  cout << (long) st.st_nlink << " ";
  cout << getpwuid(st.st_uid)->pw_name << " " << getgrgid(st.st_gid)->gr_name << " ";
  cout << (long long) st.st_size << " ";
  
  t = time(NULL);
  tmp = localtime(&st.st_mtime);
  if (tmp == NULL) nope_out("localtime");

  if (strftime(outstr, sizeof(outstr), "%b %d %H:%M", tmp) == 0) {
    nope_out("strftime");
    exit(EXIT_FAILURE);
  }
  cout << outstr << " ";

   filename = str;
   if(dirtoggle) { size_t pos = filename.find("/"); filename = filename.substr(pos+1); }
   if((st.st_mode & S_IFMT) == S_IFLNK) {
     cout << filename << " -> ";
     int count = readlink(str, buffer, sizeof(str));
     buffer[count] = '\0';
     cout << buffer << endl;
     
   }
   else cout << filename << endl;
  }//if

   else if(!atoggle && !ltoggle){
        filename = str;
   if(dirtoggle) { size_t pos = filename.find("/"); filename = filename.substr(pos+1); }
   if((st.st_mode & S_IFMT) == S_IFLNK) {
     cout << filename << " -> ";
     int count = readlink(str, buffer, sizeof(str));
     buffer[count] = '\0';
     cout << buffer << endl;
     
   }
   else cout << filename << endl;
   //cout << str << endl;
   }//elseif
   else if(atoggle && !ltoggle){
     filename = str;
     if(dirtoggle) { size_t pos = filename.find("/"); filename = filename.substr(pos+1); }
     cout << filename << endl;
   }//elseif
}//ls

/**
 * The vectorize function takes in a const char * as the directory name. The total number of blocks/2 are calculated here.
 * The directory is opened and the file contents are then recorded and placed into a vector stack, and then sorted.
 * ls is then used to finish the process by printing the info of each file within the directory.
 **/

void vectorize(const char * dirname){
  string dir = "";
  if ((dirp = opendir(dirname)) == nullptr) nope_out("opendir");
    while ((direntp = readdir(dirp)) != nullptr) {
      stat(direntp->d_name, &st);
      if(atoggle) total += (int) st.st_blocks;
      else if(string(direntp->d_name) != ".." && string(direntp->d_name) != ".") total += (int) st.st_blocks;
      files.push_back(direntp->d_name);            
    }//while  
    
    sort(files.begin(), files.end());
    if(ltoggle) cout << "total " << (total/2)-4 << endl;
    for(vector<string>::iterator i = files.begin(); i != files.end(); ++i){
      if(atoggle){
	if(dirtoggle) {
	  dir = dirname + string("/") + (*i);
	  ls(dir.c_str());
	    }//if
	else ls((*i).c_str());
      }//if
      else {
	if((*i) != ".." && (*i) != "." && (*i).substr(0,1) != "."){
	  if(dirtoggle) {
	    dir = dirname + string("/") + (*i);
	    ls(dir.c_str());
	  }//if
	  else ls((*i).c_str());
	}//if
      }//elseif
    }//for
}//vectorize

/**
 * Just your standard error printing function.
 **/

void nope_out(const string & prefix) {
  perror(prefix.c_str());
  exit(EXIT_FAILURE);
} // nope_out


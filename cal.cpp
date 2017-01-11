
/* 

Difference b/t Julian and Gregorian calendar is
that the Gregorian calendar does not consider
years divisible by 100 to be leap years

*/

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <cerrno>
#include <ctime>
#include <cmath>

using namespace std;

// PROTOTYPES

/** 
 * Checks to see if the given month is a valid option (1-12).
 *
 * @return -1 if invalid month, otherwise, the given month val
 */
int validateMonth(int);

/** 
 * Checks to see if the given year is a valid option (1-9999).
 *
 * @return -1 if invalid year, otherwise, the given year val
 */
int validateYear(int);

/**
 * Sets the curDate obj using <ctime> objs/functions.
 *
 * @return ((time_t) -1), which is the casted version of -1, if system call error, and raw time_t obj if not
 */
time_t setCurDate();

/**
 * Globally sets the type of calendar being used based on the given Date param
 * (true == Julian, false == Gregorian).
 */
void setCalSystem();

/**
 * Determines if a given year is a leap year or not, depending on the calendar system in place.
 *
 * @return true if leap year, false if not
 */
bool isLeapYear();

/**
 * Returns the number of days in the month of the givenDate obj.
 *
 * @return int num days in the month
 */
int daysInMonth();

/**
 * Returns the string version of the month of the givenDate obj.
 *
 * @return string the string version of the month
 */
string monthToString();

/**
 * Determines the century from the year of givenDate (0-based, Ex. 1856 == 18th century, not 19th).
 *
 * @return int the century
 */
int determineCentury();

/**
 * Determines the month code based on the string returned from monthToString() 
 * (month codes from https://en.wikipedia.org/wiki/Zeller%27s_congruence).
 *
 * @return the int representation of the month string
 */
int determineMonthCode();

/**
 * Calculates the day of the week (Sun, Mon, Tues, etc.) on which a given
 * date falls, using the Zeller congruence formulas and the calendar 
 * system in place.
 *
 * @return the int representing the day of the week (0 == Sat, 1 == Sun, ... 6 == Fri)
 */
int firstDayFallsOn();

/**
 * Prints out a formatted calendar for the given month/year. 
 *
 * @param bool true if function is called within printCalYear(), false if not
 */
void printCalMonth(bool);

/**
 * Prints out a formatted calendar for all months of a given year.
 */
void printCalYear();

struct Date {
  int d;
  int m;
  int y;
}; // Date 

// GLOBALS

bool isGreg = true;
struct Date curDate, givenDate;

// MAIN

int main(int argc, char * argv[]) {

  cout.setf(std::ios::unitbuf);
  cin.setf(std::ios::unitbuf);

  if(setCurDate() == ((time_t) -1)) { // check for system call errors when calling time functions
    int err = errno; // captures errno before can be changed by cout
    cout << strerror(err) << endl;
    exit(EXIT_FAILURE);
  }; // if

  switch(argc) {
  case 1: 
    givenDate.d = curDate.d;
    givenDate.m = curDate.m;
    givenDate.y = curDate.y;    
    break;
  case 2:
    givenDate.d = curDate.d;
    givenDate.m = curDate.m;
    givenDate.y = validateYear(stoi(argv[1]));    
    if(givenDate.y == -1) { cout << "Year must be 1-9999" << endl; exit(EXIT_FAILURE); }
    break;
  case 3:
    givenDate.d = curDate.d;
    givenDate.m = validateMonth(stoi(argv[1]));
    givenDate.y = validateYear(stoi(argv[2]));
    if(givenDate.m == -1 || givenDate.y == -1) { cout << "Month must be 1-12, Year must be 1-9999" << endl; exit(EXIT_FAILURE); }
    break;
  default:
    cout << "Usage ./cal [[month] year]" << endl;
    exit(EXIT_FAILURE);
  } // switch
  
  setCalSystem();

  switch(argc) {
  case 1: printCalMonth(false); break; // ./cal
  case 2: printCalYear(); break; // ./cal <year>
  case 3: printCalMonth(false); break; // ./cal <month> <year>
  default: break; // do nothing
  } // switch

  return EXIT_SUCCESS;
} // main

// DEFINITIONS

time_t setCurDate() {
  time_t rawt;
  struct tm * t;
  time(&rawt);
  t = localtime(&rawt);
  curDate.d = t->tm_mday; // 1-31
  curDate.m = t->tm_mon+1; // 0-11 + 1 -> 1-12
  curDate.y = t->tm_year+1900; // {yrs since 1900} + 1900
  return rawt;
} // setCurDate

int validateMonth(int m) {
  if(m >= 1 && m <= 12) { return m; } else { return -1; }
} // validateMonth

int validateYear(int y) {
  if(y >= 1 && y <= 9999) { return y; } else { return -1; }
} // validateYear

void setCalSystem() {
  int m = givenDate.m; int y = givenDate.y;
  isGreg = (y >= 1 && y <= 1752 && m >= 1 && m <= 9) ? false : true; // false == Julian, true == Gregorian
} // setCalSystem

bool isLeapYear() {
  bool isLeap = false;
  if(isGreg) {
    if(givenDate.y % 4 == 0 && givenDate.y % 100 != 0) {
      isLeap = true;
    } // if
  } else {
    if(givenDate.y % 4 == 0) { isLeap = true; }
  } // if/else
  return isLeap;
} // isLeapYear

int daysInMonth() {
  int days = 0;
  switch(givenDate.m) {
  case 1: days = 31; break;
  case 2: days = isLeapYear() ? 29 : 28; break;
  case 3: days = 31; break;
  case 4: days = 30; break;
  case 5: days = 31; break;
  case 6: days = 30; break;
  case 7: days = 31; break;
  case 8: days = 31; break;
  case 9: days = 30; break;
  case 10: days = 31; break;
  case 11: days = 30; break; 
  case 12: days = 31; break;
  default: break; // do nothing
  } // switch
  return days;
} // daysInMonth

string monthToString() {
  string month = "";
  switch(givenDate.m) {
  case 1: month = "January"; break;
  case 2: month = "February"; break;
  case 3: month = "March"; break;
  case 4: month = "April"; break;
  case 5: month = "May"; break;
  case 6: month = "June"; break;
  case 7: month = "July"; break;
  case 8: month = "August"; break;
  case 9: month = "September"; break;
  case 10: month = "October"; break;
  case 11: month = "November"; break; 
  case 12: month = "December"; break;
  default: break; // do nothing
  } // switch
  return month;
} // monthToString

int determineCentury() { // 0-based; Ex. 1920 == 19th century, not 20th as is commonly said
  return givenDate.y/100;
} // determineCentury

int determineMonthCode() {
  int m = 0;
  switch(givenDate.m) { 
  case 1: m = 13; break;
  case 2: m = 14; break;
  default: m = givenDate.m; break;
  } // switch
  return m;
} // determineMonthCode

int firstDayFallsOn() {
  int dayCode, q, m, K, J;
  q = 1; // set to 1 bc trying to determine day of the week of the first day of the month
  m = determineMonthCode(); // March == 3, April == 4, ... February == 14
  K = givenDate.y % 100; // Ex. turns 1856 -> 56
  J = determineCentury(); // zero-based, not ordinal-based 
  if(isGreg) {
    dayCode = ((q + (int) floor((13*(m+1))/5) + K + (int) floor(K/4) + (int) floor(J/4) + 5*J) % 7);
  } else {
    dayCode = ((q + (int) floor((13*(m+1))/5) + K + (int) floor(K/4) + 5 + 6*J) % 7);
  } // if/else
  return dayCode;
} // firstDayFallsOn

void printCalMonth(bool isPrintYear) {
  // special case './cal 9 1752', aka the month that the cal system changed from Julian -> Gregorian
  bool isSpecialCase = (givenDate.y == 1752 && givenDate.m == 9) ? true : false;

  // these 3 lines center the title in all cases
  string header = (isPrintYear) ? monthToString() : monthToString() + " " + to_string(givenDate.y);
  int offset = 10 - (int) floor(header.length()/2); 
  for(int i = 0; i < offset; i++) { cout << " "; } // for

  // these 2 lines are header stuff
  cout << header << endl;
  cout << "Su Mo Tu We Th Fr Sa" << endl;

  // next block of code determines from which day to begin printing 1 thru numDaysInMonth()
  int dayCode = firstDayFallsOn();
  switch(dayCode) {
  case 0: cout << "                  "; break; // 18 spaces for Sat
  case 1: break; // 0 spaces for Sun
  case 2: cout << "   "; break; // 3 spaces for Mon
  case 3: cout << "      "; break; // 6 spaces for Tue
  case 4: cout << "         "; break; // 9 spaces for Wed
  case 5: cout << "            "; break; // 12 spaces for Thu
  case 6: cout << "               "; break; // 15 spaces for Fri
  default: break; // do nothing 
  } // switch
  
  // begins printing from the appropriate day of the week
  int count = dayCode;
  for(int i = 1; i < daysInMonth()+1; i++) {
    if(i < 10) { // 1 digit cal num
      if(givenDate.y == curDate.y && givenDate.m == curDate.m && i == curDate.d) { // if given month & year == cur month & year, highlight today
	cout << " "; 
	cout << "\x1b[37;40m" << i; // white text, black bg
	cout << "\x1b[0;0m "; // changes color back to normal, adds a space
      } else {
	cout << " \x1b[0;0m" << i << " "; // black text, white bg
      } // if/else
    } else { // 2 digit cal num
      if(givenDate.y == curDate.y && givenDate.m == curDate.m && i == curDate.d) { // if given month & year == cur month & year, highlight today
	cout << "\x1b[37;40m" << i; // white text, black bg
	cout << "\x1b[0;0m "; // changes color back to normal, adds a space
      } else {
	cout << "\x1b[0;0m" << i << " "; // black text, white bg
      } // if/else
    } // if/else  
    if(count % 7 == 0) { // reached physical end of calendar output
      cout << endl;
    } // if
    if(i == 2 && isSpecialCase) {
      i+=11;
    } // if
    count++;
  } // for
 
  cout << "\n" << endl; // aesthetic reasons 
} // printCalMonth

void printCalYear() {
  // these 4 lines center the year header in all cases, then print it
  string header = to_string(givenDate.y);
  int offset = 10 - (int) floor(header.length()/2); 
  for(int i = 0; i < offset; i++) { cout << " "; } // for
  cout << header << "\n" << endl;

  givenDate.m = 1;
  for(int i = givenDate.m; i <= 12; i++) {
    printCalMonth(true);
    givenDate.m++;
  } // for
} // printCalYear


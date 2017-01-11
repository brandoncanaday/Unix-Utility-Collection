# Project 3

@author - Brandon Canaday (811 737 467), Nick Angelastro (811 068 256)

## Instructions
   
   To run the programs (all but ./cksum): 

      ```
      $ ./cal [[month] year]
      ```      
      ```
      $ ./date [+format]
      ```      
      ```
      $ ./head [-n number] [file...]
      ```      
      ```
      $ ./ln [-s] target-file link-file
      ```      
      ```
      $ ./ls [-al] [file...]
      ```      
      ```
      $ ./mkdir [-p] [-m mode] dir...
      ```      
      ```
      $ ./env
      ```      
      ```
      $ ./tail [-f] [ -c number | -n number] [file]
      ```      
      ```
      $ ./true
      $ ./false 
      ```      
      ```
      $ ./wc [-c | -m] [-lw] [file...]
      ```      
      ```
      $ ./pwd
      ```      
   
   To compile AND link ALL programs: 

      ```
      $ make
      ```
      
   To compile each program only: 

      ```
      $ make program_name.o
      ```

   To clean dir of .o, executable, and tilda (~) files: 

      ```
      $ make clean
      ```

## Reflection

   As compared to last project, this one was much more manageable to 
deal with, especially when it came to meeting up. Due to the compart-
mentalized nature of this project, being that it was a bunch of smaller
programs, the splitting of the work came much more naturally this time
around. We each took it upon ourselves to do 5 programs, splitting the 
difficulty pretty much down the middle. Along the way, I think we both 
became much more familiar and comfortable with parsing command line args,
especially when it came to setting boolean flags. We also learned more
about implementing recursive functions, and we developed more of an intimacy 
with file permissions and catching system call errors. Also, shout-out to the 
man pages.
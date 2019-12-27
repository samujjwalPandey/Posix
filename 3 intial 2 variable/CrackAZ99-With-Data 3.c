#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 3 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    cc -o CrackAZ99-With-Data 3 CrackAZ99-With-Data 3.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./CrackAZ99-With-Data 3 > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$J5muN9B0M6KlZvP9i/ULruXmQ/I02TBrHcjhXtoTZP4EYqb0ffiaitfdw24asmGO5/9I8nI4nRLtOe7hZnq2j/",
  "$6$KB$E5w.ylYKpSKN.HLeb55q2eqgehHieqzYcNS7ruE78GtQpQnrsCXLMHAaQ6fLcEO76/uhwqCKLW2TBrb1Q/KAa/",
  "$6$KB$H4e7MFv4AkYw.I7WXPgU2ofrQZVefS4Ar1E1McrenSra24CpGzZ.FSLynplJgatDJpX6HIGe7dxeevd8fVJ1q.",
  "$6$KB$cYBKtCk4Aht5rxKXdZDz90REnUFAdy8yNQmtblS8k3kqf34tCNasP5yp59bvNZHquzbBlueXz0WigV24X4/ei."
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void crack(char *salt_and_encrypted){
  int s, a, m,u;     
  char salt[7];    
  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);

  for(s='A'; s<='Z'; s++){
    for(a='A'; a<='Z'; a++){
    for(m='A'; m<='Z';m++){
      for(u=0; u<=99; u++){
        sprintf(plain, "%c%c%c%02d", s, a,m, u); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
   }
  }
  printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start, struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(){
  int p;
  
struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

  for(p=0;p<n_passwords;p<p++) {
    crack(encrypted_passwords[i]);
  }
clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 
  return 0;
}

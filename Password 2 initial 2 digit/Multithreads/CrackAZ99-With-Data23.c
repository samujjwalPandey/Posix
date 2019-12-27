#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <pthread.h>
/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    cc -o CrackAZ99-With-Data23 CrackAZ99-With-Data23.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./CrackAZ99-With-Data23.c > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$yPvAP5BWyF7oqOITVloN9mCAVdel.65miUZrEel72LcJy2KQDuYE6xccHS2ycoxqXDzW.lvbtDU5HuZ733K0X0",
  "$6$KB$G/Nlf9jJdkbjMHMqOD0scV20t6cnhtojQKz8v14fhG26GmKOiz.Bd4QXDLeh9SjiEqsBozC812Yf5bvbFWz131",
  "$6$KB$ciaKBZs/3LSZlaDE.dagGO0ZE9YfYymWUpzFnIuDTUviDxpqsUVE0HE1/HUXgMp0xl6IfVEzDljEW1iy6ttlG.",
  "$6$KB$DY2hmqaRHB3ILUMAMW9k0b.1iHkAfzAe5RtI/stVutttQ.IIV3GcFyQzepzzQhlyGr5nPxIchG6Im8/hYNhzj1"
};


/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void factorization(char *pass) {
  pthread_t thread_1, thread_2;

  void *kernel_function_1();
  void *kernel_function_2();
  
  pthread_create(&thread_1, NULL, kernel_function_1, pass);
  pthread_create(&thread_2, NULL, kernel_function_2, pass);

  pthread_join(thread_1, NULL);
  pthread_join(thread_2, NULL);
}
/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void *kernel_function_1(char *salt_and_encrypted){
  int a, b, c;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='A'; a<='M'; a++){
    for(b='A'; b<='Z'; b++){
     
      for(c=0; c<=99; c++){
        sprintf(plain, "%c%c%02d", a, b,c); 
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
  printf("%d solutions explored\n", count);
}
void *kernel_function_2(char *salt_and_encrypted){
  int a, b, c;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='N'; a<='Z'; a++){
    for(b='A'; b<='Z'; b++){
     
      for(c=0; c<=99; c++){
        sprintf(plain, "%c%c%02d", a, b,c); 
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
  int x;
  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
  
  for(x=0;x<n_passwords;x<x++) {
    factorization(encrypted_passwords[x]);
  }
  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9));

  return 0;
}

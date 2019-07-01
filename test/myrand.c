#include <stdio.h> 
#include <stdlib.h> 
  
// Driver program 
int main(void) 
{ 
    // This program will create same sequence of  
    // random numbers on every program run  
    for(int i = 0; i<5; i++) 
        // fprintf(stderr,"Random Number:  %d\n", rand());
    	rand();
    return 0; 
} 

#include "rand_wrap.h"


/***************************/
/* User wrappers start here */
/***************************/

/* Return type is always void 
The parameter is same as the original function that you want to wrap */ 
void rand_mywrap(void)
{
  fprintf(stderr, "Wrapped rand successfully\n"); 
  return ;
}



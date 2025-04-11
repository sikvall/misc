#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>


typedef struct {
  int64_t a; // numerator
  int64_t b; // denominator
} real_t;

void real_print(real_t *r) {
  printf("%12ld / %-12ld = %18f\n", r->a, r->b,  (double)r->a / (double)r->b);
}
  

int64_t real_gcd(int64_t a, int64_t b) {
  a = abs(a);
  b = abs(b);
  while (b != 0) {
    int64_t tmp = b;
    b = a % b;
    a = tmp;
  }
  return a;
}


void real_simplify(real_t *r) {

  // Throw error and exit if denominator is 0
  if (r->b == 0) {
    fprintf(stderr,
	    "ERR: Denominator connot be zero.\n"
	    "     Division by 0 error - exiting.\n");
    exit(20);
  }

  // Adjust the sign properly, if both nominator and denominator is
  // negative reverse the sign on both.  If b is negative we reverse
  // the sign on both. This will correct to a postive number if both a
  // and b are negative and if a is positive and b is negative it will
  // correct it to a negative a and a positive b!
  if (r->b < 0) {
    r->a = -r->a;
    r->b = -r->b;
  }

  real_print(r);
  
  
}


void double_to_real(double d, real_t *r)
{
  int sign = 1;
  double fraction;
  int64_t integer;
  int64_t figure;
  int iter;
  int max_iter = 12;

  // Initialize structure to zero
  r->a = 0;
  r->b = 1;

  // Nominator is the integer part
  r->a = (int64_t)d;

  // Fraction is what remains now
  fraction = fabs(d - abs(r->a));


  // While the fraction is significant multiply by 10, take the
  // integer part, multiply the nominator by ten, add the integer part
  // to that and multiply the denominator also by ten, then remove the
  // integer part and do the next round.
  while(iter < max_iter) {
    fraction *= 10;
    r->a *= 10;
    r->a  += (int64_t)fraction;
    fraction  = fraction - (int64_t)fraction;
    r->b *= 10;
    printf("%f %ld / %ld \n", fraction, r->a, r->b);
    iter++;
  }

  real_print(r);
 
}



  


int main(int argc, void *argv[]) {
  real_t r;
  double d;


  d = -630.8;

  double_to_real(d, &r);
  real_simplify(&r);

  return 0;
}

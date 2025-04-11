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
  

int64_t real_gcd(int64_t a, int64_t b)
{
  a = abs(a);
  b = abs(b);
  while (b != 0) {
    int64_t tmp = b;
    b = a % b;
    a = tmp;
  }
  return a;
}


void real_simplify(real_t *r)
{
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

  int64_t gcd = 0;

  gcd = real_gcd(r->a, r->b);
  if(gcd) {
    r->a /= gcd;
    r->b /= gcd;
  }

  return;
  

  //  real_print(r);

  return ;
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


void real_mul(real_t *f1, real_t *f2, real_t *res)
{
  res->a = f1->a * f2->a;
  res->b = f1->b * f2->b;
  real_simplify(res);

  return;
}


void real_div(real_t *f1, real_t *f2, real_t *res)
{
  res->a = f1->a * f2->b;
  res->b = f1->b * f2->a;
  real_simplify(res);

  return;
}


void real_add(real_t *t1, real_t *t2, real_t *res)
{
  res->a = (t1->a * t2->b) + (t2->a * t1->b);
  res->b = t1->b * t2->b;
  real_simplify(res);

  return;
}


void real_sub(real_t *f1, real_t *f2, real_t *res)
{
  printf("%ld/%ld - %ld/%ld\n", f1->a, f1->b, f2->a, f2->b);
  res->a = (f1->a*f2->b) - (f2->a*f1->b);
  res->b = f1->b * f2->b;
  //  real_simplify(res);
  
  return;
}


int main(int argc, void *argv[]) {
  real_t f1, f2, res;

  f1.a = 120;
  f1.b = 100;

  f2.a = 120;
  f2.b = 100;

  printf("Rå Input:\n");
  real_print(&f1);
  real_print(&f2);

  real_simplify(&f1);
  real_simplify(&f2);
  
  printf("Input:\n");
  real_print(&f1);
  real_print(&f2);

  printf("Division:\n");
  real_div(&f1, &f2, &res);
  real_print(&res);

  printf("Multiplikation:\n");
  real_mul(&f1, &f2, &res);
  real_print(&res);

  printf("Addition:\n");
  real_add(&f1, &f2, &res);
  real_print(&res);

  printf("Subtraktion:\n");
  real_sub(&f1, &f2, &res);
  real_print(&res);
  
  return 0;
}



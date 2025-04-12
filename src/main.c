#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>


typedef struct {
	int64_t a; // numerator
	int64_t b; // denominator
} real_t;

void real_print(real_t *r) {
	printf("%12ld / %-12ld = %30.15f\n", r->a, r->b,  (double)r->a / (double)r->b);
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
	
	
	/* While the fraction is significant multiply by 10, take the
           integer part, multiply the nominator by ten, add the integer part
           to that and multiply the denominator also by ten, then remove the
           integer part and do the next round. */
	
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


void real_div(real_t *f1, real_t *f2, real_t *res) {
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
	res->a = (f1->a*f2->b) - (f2->a*f1->b);
	res->b = f1->b * f2->b;
	//  real_simplify(res);
	
	return;
}


int64_t int64sqrt(int64_t i)
{
	int64_t left, mid, right, result;
	
	// Can't do negative (yet)
	if (i<0) {
		printf("ERR: Can not calculate the square root of a negative.\n");
		exit(20);
	}
	
	// If input is 0 or 1 the root is by definition the same
	if( i == 0 ||
	    i == 1) return i;
	
	// Prime the calculation
	left = 1;   right = i;   result = 0;
	while (left <= right) {
		mid = left + (right - left) / 2;
		if (mid <= i / mid) {
			result = mid;
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}
	return result;
}


#define REAL_ANTE (10000000L)

void real_up_the_ante(real_t *p)
{
	// Determine exactly how much
	if(p->a < REAL_ANTE) {
		p->a *= REAL_ANTE;
		p->b *= REAL_ANTE;
	}
	
	if(p->b < REAL_ANTE) {
		p->a *= REAL_ANTE;
		p->b *= REAL_ANTE;
	}
}



void real_sqrt(real_t *f, real_t *r)
{
	real_up_the_ante(f);
	r->a = int64sqrt(f->a);
	r->b = int64sqrt(f->b);
	real_simplify(r);
	
	return ;
}


void real_pi(real_t *pi)
{
	pi->a = 5419351;
	pi->b = 1725033;
}


int main(int argc, void *argv[]) {
	real_t f1, f2, res;
	
	f1.a = 436;
	f1.b = 100;
	
	real_pi(&f2);
	
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
	
	printf("Kvadratroten:\n");
	real_sqrt(&f1, &res);
	real_print(&res);
	
	printf("Square root of some numbers:\n"
	       "3 %ld  4 %ld  10 %ld  16 %ld  22 %ld  100 %ld\n",
	       int64sqrt(3), int64sqrt(4), int64sqrt(10), int64sqrt(16),
	       int64sqrt(22), int64sqrt(100));
	
	return 0;
}



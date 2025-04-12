#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "rational.h"


// Create a rational number easily.
void rational_create(int64_t n, int64_t d, rational_t *res)
{
	res->a = n;
	res->b = d;
	rational_simplify(res);

	return;
}


// This function prints a rational to the stdout including the
// corresponding double float value. Mostly used for debugging
// purposes of course.
void rational_print(rational_t *r)
{
	printf("%12ld / %-12ld = %12.4f\n",
	       r->a, r->b,  (double)r->a / (double)r->b);

	return;
}


// This function finds the greatest common denominator of a rational and returns
// this as an int64_t, this is used to simplify a rational.
int64_t rational_gcd(int64_t a, int64_t b)
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


// This function finds the greatest common denominator betweet the numerator
// and denominator and then it divides both with the GCD which simplifies the
// rational It returns nothing and it takes one argument which is a pointer to
// a rational_t object.
void rational_simplify(rational_t *r)
{
	// Throw error and exit if denominator is 0
	if (r->b == 0) {
		fprintf(stderr,
			"ERR: Denominator connot be zero.\n"
			"     Division by 0 error - exiting.\n");
		exit(20);
	}
	
	// Adjust the sign bit to the numerator only. if N and D are
	// negative, the rational is positive. If one of the D or N is
	// negative the rational is negative. To fix this we check the
	// sign bit of the D and if it is negative then we flip the
	// sign on both the N and D and this will be correct.
	if (r->b < 0) {
		r->a = -r->a;
		r->b = -r->b;
	}
	
	// Find the greatest comon denominator GCD and divide both N
	// and D with this.
	int64_t gcd = rational_gcd(r->a, r->b);
	if(gcd) {
		r->a /= gcd;
		r->b /= gcd;
	}
	
	return ;
}

// This converts a double to a rational. It's not the best way of doing things
// but it kind of works. So basically to convert a number like 6543.44882 we
// take the integer part and put it in a rational like 6543/1 and then we add
// the decimals one by one so we get 65434/10, then 654344882/100000 and when
// we are done with that we simplify if possible.
void double_to_rational(double d, rational_t *r)
{
	int sign = 1;
	double fraction;
	int64_t integer;
	int64_t figure;
	int iter;
	
	// Initialize structure to zero
	r->a = 0;
	r->b = 1;
	
	// Nominator is the integer part
	r->a = (int64_t)d;
	
	// Fraction is what remains now
	fraction = fabs(d - abs(r->a));
	
	
	// While the fraction is significant multiply by 10, take the integer
	// part, multiply the nominator by ten, add the integer part to that
	// and multiply the denominator also by ten, then remove the integer
	// part and do the next round.
	while(iter < RATIONAL_DOUBLE_MAX_ITER) {
		fraction *= 10;
		r->a *= 10;
		r->a += (int64_t)fraction;
		fraction  = fraction - (int64_t)fraction;
		r->b *= 10;
		printf("%f %ld / %ld \n", fraction, r->a, r->b);
		iter++;
	}
	
	rational_print(r);
}


// Multiplies two rationals, f1 and f2 and places the result in res which is
// then also simplified before returning.
void rational_mul(rational_t *f1, rational_t *f2, rational_t *res)
{
	res->a = f1->a * f2->a;
	res->b = f1->b * f2->b;
	rational_simplify(res);
	
	return;
}

// Divides two rationals, f1 and f2 and places the result in res which is then
// simplified.
void rational_div(rational_t *f1, rational_t *f2, rational_t *res)
{
	res->a = f1->a * f2->b;
	res->b = f1->b * f2->a;
	rational_simplify(res);
	
	return;
}

// Adds two rationals t1 and t2 and the result is then placed in res which is
// also simplified.
void rational_add(rational_t *t1, rational_t *t2, rational_t *res)
{
	res->a = (t1->a * t2->b) + (t2->a * t1->b);
	res->b = t1->b * t2->b;
	rational_simplify(res);
	
	return;
}

// Subtracts two radionals such as res = f1 - f2 which is then also simplified
// before returning.
void rational_sub(rational_t *f1, rational_t *f2, rational_t *res)
{
	res->a = (f1->a*f2->b) - (f2->a*f1->b);
	res->b = f1->b * f2->b;
	//  rational_simplify(res);
	
	return;
}

// Takes the square root of the rational i which is then simplified. The
// process first makes sure the rational has enough figures to get good
// precision, then it will find the closest possible integer to both N and D
// and when this is done it will simplify the rational and return.
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


// Makes sure both the N and D are of sufficient size before certain
// operations are made to preserve precision.
void rational_up_the_ante(rational_t *p)
{
	if(p->a < RATIONAL_ANTE) {
		p->a *= RATIONAL_ANTE;
		p->b *= RATIONAL_ANTE;
	}
	
	if(p->b < RATIONAL_ANTE) {
		p->a *= RATIONAL_ANTE;
		p->b *= RATIONAL_ANTE;
	}
}


// Take the rational in f, return a reasonable approximation of the square
// root of it in r.
void rational_sqrt(rational_t *f, rational_t *r)
{
	rational_up_the_ante(f);
	r->a = int64sqrt(f->a);
	r->b = int64sqrt(f->b);
	rational_simplify(r);
	
	return ;
}


// Enter a fraction that gives a reasonable value of pi
void rational_pi(rational_t *pi)
{
	pi->a = 5419351;
	pi->b = 1725033;
}


// Just test code so far
int main(int argc, void *argv[]) {
	rational_t f1, f2, res;
	
	rational_create(436, 100, &f1); rational_print(&f1);
	rational_create(-436, 100, &f2); rational_print(&f2);

	rational_add(&f1, &f2, &res);
	rational_print(&res);

	return 0;
}



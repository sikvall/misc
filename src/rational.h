// Defining of magical numbers
#define RATIONAL_ANTE (10000000L)
#define RATIONAL_MAX_ITER (20)

// The main struct for a rational
typedef struct {
	int64_t a; // numerator
	int64_t b; // denominator
} rational_t;


/* Function prototypes */
void rational_print(rational_t *r);
int64_t rational_gcd(int64_t a, int64_t b);
void rational_simplify(rational_t *r);
void double_to_rational(double d, rational_t *r);
void rational_mul(rational_t *f1, rational_t *f2, rational_t *res);
void rational_div(rational_t *f1, rational_t *f2, rational_t *res);
void rational_sub(rational_t *f1, rational_t *f2, rational_t *res);
void rational_add(rational_t *t1, rational_t *t2, rational_t *res);
int64_t int64sqrt(int64_t i);
void rational_up_the_ante(rational_t *p);
void rational_sqrt(rational_t *f, rational_t *r);
void rational_pi(rational_t *pi);

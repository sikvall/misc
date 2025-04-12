// Defining of magical numbers
#define RATIONAL_ANTE (10000000L)

// The main struct for a rational
typedef struct {
	int64_t a; // numerator
	int64_t b; // denominator
} rational_t;


/* Function prototypes */
void rational_print(rational_t *r);
int64_t rational_gcd(int64_t a, int64_t b);
void rational_simplify(rational_t *r);

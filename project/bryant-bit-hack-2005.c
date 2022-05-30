/* Functions extracted from Web page by Sean E. Anderson (seander@cs.stanford.edu) by REB 09/28/2004 */
/* Found several obvious errors: Mismatched parentheses, mispelled words, one wrong comparison */
/* Updated based on web page of 04/26/2005 */
/* Created own definitions of argument constraints, and in many cases own test version */

/* Guide:
   Functions named test_XXX written as specification for following functions.
   Different versions of function named FFF_1, FFF_2, etc.
   Function named FFF_N_fix is corrected version of function FFF_N.

   Range of arguments for N-argument function specified as list of patterns: pat1:pat2:...:patN.
   Each pattern specifies range of one function argument.
   Possible patterns
     [min,max] Inclusive range
     uK        Argument is K-bit, unsigned value
     tK        Argument is K-bit, two's complement value
*/
   
   

/* Compute the sign of an integer, returning -1 for < 0 & 0 for >= 0 */
int test_sign1(int v)
{
  return -(v < 0);
}

/* Verified with BDD checker 03/31/2005 */
int sign1_1(int v)
/* ARGS t32 */
/* TEST test_sign1 */
/* STATUS OK */
/* METHOD BDD */
{
  int sign;   // the result goes here 
  sign = v >> (sizeof(int) * 8 - 1); // if v < 0 then -1, else 0
  return sign;
}

/* ERROR Here's an obviously buggy description:  It *should* be -(v < 0).

The above expression evaluates to sign = v >> 31 for 32-bit integers. This is 
one operation faster than the obvious way, sign = -(v > 0). This trick works 
because when integers are shifted right, the value of the far left bit is copied 
to the other bits. The far left bit is 1 when the value is negative and 0 
otherwise; all 1 bits is -1. 
*/

/* Compute the sign of an integer, returning -1 for < 0 & +1 for >= 0 */
int test_sign2(int v)
{
  return (v < 0) ? -1 : 1;
}

/* Verified with BDD checker 03/31/2005 */
int sign2_1(int v)
/* ARGS t32 */
/* TEST test_sign2 */
/* STATUS OK */
/* METHOD BDD */
{
  int sign = 1 | (v >> (sizeof(int) * 8 - 1));
  return sign;
}

/* Compute the sign of an integer, returning -1 for < 0, 0 for 0, and +1 for > 0 */
int test_sign3(int v)
{
  return v < 0 ? -1 : v == 0 ? 0 : 1;
}

/* Verified with BDD checker 03/31/2005 */
int sign3_1(int v)
/* ARGS t32 */
/* TEST test_sign3 */
/* STATUS OK */
/* METHOD BDD */
{
  return (v != 0) | (v >> (sizeof(int) * 8 - 1));
}


/* Absolute Value */
int test_abs(int v)
{
  return  v < 0 ? -v : v;
}

/* Verified with BDD checker 03/31/2005 */
int abs_1(int v)
/* ARGS [-2147483647,2147483647] */
/* TEST test_abs */
/* STATUS OK */
/* METHOD BDD */
{
  return (v ^ (v >> (sizeof(int) * 8 - 1))) - (v >> (sizeof(int) * 8 - 1));
}

/* Verified with BDD checker 03/31/2005 */
int abs_2(int v)
/* ARGS t32 */
/* TEST test_abs */
/* STATUS OK */
/* METHOD BDD */
{
  return (1|(v>>(sizeof(int)*8-1)))*v;
}

/* Verified with BDD checker 03/31/2005 */
int abs_3(int v)
/* ARGS t32 */
/* TEST test_abs */
/* STATUS OK */
/* METHOD BDD */
{
  return (v ^ (v >> (sizeof(int) * 8 - 1))) + ((unsigned) v >> (sizeof(int) * 8 - 1));
}

/* Verified with BDD checker 03/31/2005 */
int abs_4(int v)
/* ARGS t32 */
/* TEST test_abs */
/* STATUS OK */
/* METHOD BDD */
{
  return (v ^ (v >> (sizeof(int) * 8 - 1))) + (v < 0);
}

/* Find minimum of x & y */
int test_min(int x, int y)
{
  return (x < y) ? x : y;
}

/* ERROR: Original version missing a ')' */
/* ERROR: Gets wrong result when x-y overflows in either direction.
   E.g.: x = -134217728 (0xf8000000), y = 2080374784 (0x7c000000)
   Found by BDD checker 04/07/2005
*/
int min_1(int x, int y)
/* ARGS t32:t32 */
/* TEST test_min */
/* STATUS BUG */
/* METHOD BDD */
{
  return y + ((x - y) & ((x - y) >> (sizeof(int) * 8 - 1))); // min(x, y)
}

/* REB version of the code */
/* Verified with BDD checker 04/23/2005 */
int min_1_fix(int x, int y)
/* ARGS t32:t32 */
/* TEST test_min */
/* STATUS OK */
/* METHOD BDD */
{
  return y + ((x - y) & -(x < y)); // min(x, y)
}

/* Find maximum of x & y */
int test_max(int x, int y)
{
  return (x > y) ? x : y;
}

/* ERROR: Original version missing a ')' */
/* ERROR: Gets wrong result when x-y overflows in either direction.
   E.g.: x = -134217728 (0xf8000000), y = 2080374784 (0x7c000000)
   Found by BDD checker 04/07/2005
*/
int max_1(int x, int y)
/* ARGS t32:t32 */
/* TEST test_max */
/* STATUS BUG */
/* METHOD BDD */
{
  return x - ((x - y) & ((x - y) >> (sizeof(int) * 8 - 1))); // max(x, y)
}

/* REB version of the code */
/* Verified with BDD checker 04/23/2005 */
int max_1_fix(int x, int y)
/* ARGS t32:t32 */
/* TEST test_max */
/* STATUS OK */
/* METHOD BDD */
{
  return y + ((x - y) & -(x > y)); // min(x, y)
}

/* Determine if number is power of 2 */
int test_is_pwr2(unsigned int v)
{
  int cnt = 0;
  while (v) {
    cnt += v & 1;
    v >>= 1;
  }
  return cnt == 1;
}

/* Verified with BDD checker 04/26/2005 */
int is_pwr2_1(unsigned int v)
/* ARGS u32 */
/* TEST test_is_pwr2 */
/* STATUS OK */
/* METHOD BDD */
{
  return v != 0 && (v & (v - 1)) == 0;
}

/* Verified with BDD checker 04/07/2005 */
int is_pwr2_2(unsigned int v)
/* ARGS u32 */
/* TEST test_is_pwr2 */
/* STATUS OK */
/* METHOD BDD */
{
  return !(v & (v - 1)) && v;
}

/* Extend number from width bits to sizeof(int)*8 */
/* Can assume 1 <= width <= sizeof(int) * 8 */

/* First class works correctly only if high order bits are zero.
   Implemented in checker by inserting wrapper code to zero out
   high order bits
*/

/* Copy new sign bit to the left */
int test_extend1(int x, unsigned width)
{
  /* Wrapper */
  if (width < 8*sizeof(int))
    x &= ((1<<width) -1);
  {
    int mask = 1 << (width-1);
    int bit = (x & mask) << 1;
    mask <<= 1;
    while (mask) {
      x = bit | (~mask & x);
      mask <<= 1;
      bit <<= 1;
    } 
    return x;
  }
}

/* Verified with BDD checker 03/31/2005 */
int extend1_1(int x, unsigned width)
/* ARGS t32:[1,31] */
/* TEST test_extend1 */
/* STATUS OK */
/* METHOD BDD */
{
  /* Wrapper */
  if (width < 8*sizeof(int))
    x &= ((1<<width) -1);
  {
    int m = 1 << (width - 1); // mask can be pre-computed if b is fixed
    return -(x & m) | x;
  }
}

/* Verified with BDD checker 03/31/2005 */
int extend1_2(int x, unsigned width)
/* ARGS t32:[1,31] */
/* TEST test_extend1 */
/* STATUS OK */
/* METHOD BDD */
{
  /* Wrapper */
  if (width < 8*sizeof(int))
    x &= ((1<<width)-1);
  {
    int m = (1 << (width - 1)) - 1;
    return -(x & ~m) | x;
  }
}

/* Second class *should* work even when high order bits not
   initially zero.  No wrapper code required.
*/

/* Copy new sign bit to the left */
int test_extend2(int x, unsigned width)
{
  int mask = 1 << (width-1);
  int bit = (x & mask) << 1;
  mask <<= 1;
  while (mask) {
    x = bit | (~mask & x);
    mask <<= 1;
    bit <<= 1;
  } 
  return x;
}


#define M(B) (1 << ((sizeof(x) << 3) - B))
  const int multipliers[] = 
    {
      0,     M(1),  M(2),  M(3),  M(4),  M(5),  M(6),  M(7),
      M(8),  M(9),  M(10), M(11), M(12), M(13), M(14), M(15),
      M(16), M(17), M(18), M(19), M(20), M(21), M(22), M(23),
      M(24), M(25), M(26), M(27), M(28), M(29), M(30), M(31),
      M(32)
    }; // (add more for 64 bits)

/* ERROR: Doesn't work correctly for (1,1).  Problem is that m is INTMIN
   1*m/m gives +1.
   Detected with BDD Checker.  4/1/2005.
*/

int extend2_1(int x, unsigned width)
/* ARGS t32:[1,31] */
/* TEST test_extend2 */
/* STATUS BUG */
/* METHOD BDD */
{
  /* After macro expansion */
  const int multipliers[33] =
    {
      0, (1 << ((sizeof(int) << 3) - 1)), (1 << ((sizeof(int) << 3) - 2)), (1 << ((sizeof(int) << 3) - 3)), (1 << ((sizeof(int) << 3) - 4)), (1 << ((sizeof(int) << 3) - 5)), (1 << ((sizeof(int) << 3) - 6)), (1 << ((sizeof(int) << 3) - 7)),
      (1 << ((sizeof(int) << 3) - 8)), (1 << ((sizeof(int) << 3) - 9)), (1 << ((sizeof(int) << 3) - 10)), (1 << ((sizeof(int) << 3) - 11)), (1 << ((sizeof(int) << 3) - 12)), (1 << ((sizeof(int) << 3) - 13)), (1 << ((sizeof(int) << 3) - 14)), (1 << ((sizeof(int) << 3) - 15)),
      (1 << ((sizeof(int) << 3) - 16)), (1 << ((sizeof(int) << 3) - 17)), (1 << ((sizeof(int) << 3) - 18)), (1 << ((sizeof(int) << 3) - 19)), (1 << ((sizeof(int) << 3) - 20)), (1 << ((sizeof(int) << 3) - 21)), (1 << ((sizeof(int) << 3) - 22)), (1 << ((sizeof(int) << 3) - 23)),
      (1 << ((sizeof(int) << 3) - 24)), (1 << ((sizeof(int) << 3) - 25)), (1 << ((sizeof(int) << 3) - 26)), (1 << ((sizeof(int) << 3) - 27)), (1 << ((sizeof(int) << 3) - 28)), (1 << ((sizeof(int) << 3) - 29)), (1 << ((sizeof(int) << 3) - 30)), (1 << ((sizeof(int) << 3) - 31)),
      (1 << ((sizeof(int) << 3) - 32))
    };
  const int m = multipliers[width];
  return (x * m) / m;
}

/* REB Version */
/* Verified with BDD checker 04/23/2005 */
int extend2_1_fix(int x, unsigned width)
/* ARGS t32:[1,31] */
/* TEST test_extend2 */
/* STATUS OK */
/* METHOD BDD */
{
  /* After macro expansion */
  const int multipliers[33] =
    {
      0, (1 << ((sizeof(int) << 3) - 1)), (1 << ((sizeof(int) << 3) - 2)), (1 << ((sizeof(int) << 3) - 3)), (1 << ((sizeof(int) << 3) - 4)), (1 << ((sizeof(int) << 3) - 5)), (1 << ((sizeof(int) << 3) - 6)), (1 << ((sizeof(int) << 3) - 7)),
      (1 << ((sizeof(int) << 3) - 8)), (1 << ((sizeof(int) << 3) - 9)), (1 << ((sizeof(int) << 3) - 10)), (1 << ((sizeof(int) << 3) - 11)), (1 << ((sizeof(int) << 3) - 12)), (1 << ((sizeof(int) << 3) - 13)), (1 << ((sizeof(int) << 3) - 14)), (1 << ((sizeof(int) << 3) - 15)),
      (1 << ((sizeof(int) << 3) - 16)), (1 << ((sizeof(int) << 3) - 17)), (1 << ((sizeof(int) << 3) - 18)), (1 << ((sizeof(int) << 3) - 19)), (1 << ((sizeof(int) << 3) - 20)), (1 << ((sizeof(int) << 3) - 21)), (1 << ((sizeof(int) << 3) - 22)), (1 << ((sizeof(int) << 3) - 23)),
      (1 << ((sizeof(int) << 3) - 24)), (1 << ((sizeof(int) << 3) - 25)), (1 << ((sizeof(int) << 3) - 26)), (1 << ((sizeof(int) << 3) - 27)), (1 << ((sizeof(int) << 3) - 28)), (1 << ((sizeof(int) << 3) - 29)), (1 << ((sizeof(int) << 3) - 30)), (1 << ((sizeof(int) << 3) - 31)),
      (1 << ((sizeof(int) << 3) - 32))
    };
  const int divisors[33] =
    {
      /* Trick: Make divisors[1] INT_MAX rather than INT_MIN */
      0, ~(1 << ((sizeof(int) << 3) - 1)), (1 << ((sizeof(int) << 3) - 2)), (1 << ((sizeof(int) << 3) - 3)), (1 << ((sizeof(int) << 3) - 4)), (1 << ((sizeof(int) << 3) - 5)), (1 << ((sizeof(int) << 3) - 6)), (1 << ((sizeof(int) << 3) - 7)),
      (1 << ((sizeof(int) << 3) - 8)), (1 << ((sizeof(int) << 3) - 9)), (1 << ((sizeof(int) << 3) - 10)), (1 << ((sizeof(int) << 3) - 11)), (1 << ((sizeof(int) << 3) - 12)), (1 << ((sizeof(int) << 3) - 13)), (1 << ((sizeof(int) << 3) - 14)), (1 << ((sizeof(int) << 3) - 15)),
      (1 << ((sizeof(int) << 3) - 16)), (1 << ((sizeof(int) << 3) - 17)), (1 << ((sizeof(int) << 3) - 18)), (1 << ((sizeof(int) << 3) - 19)), (1 << ((sizeof(int) << 3) - 20)), (1 << ((sizeof(int) << 3) - 21)), (1 << ((sizeof(int) << 3) - 22)), (1 << ((sizeof(int) << 3) - 23)),
      (1 << ((sizeof(int) << 3) - 24)), (1 << ((sizeof(int) << 3) - 25)), (1 << ((sizeof(int) << 3) - 26)), (1 << ((sizeof(int) << 3) - 27)), (1 << ((sizeof(int) << 3) - 28)), (1 << ((sizeof(int) << 3) - 29)), (1 << ((sizeof(int) << 3) - 30)), (1 << ((sizeof(int) << 3) - 31)),
      (1 << ((sizeof(int) << 3) - 32))
    };
  const int m = multipliers[width];
  const int d =divisors[width];
  return (x * m) / d;
}

/* Verified with BDD checker 04/23/2005 */
int extend2_2(int x, unsigned width)
/* ARGS t32:[1,31] */
/* TEST test_extend2 */
/* STATUS OK */
/* METHOD BDD */
{
  int shift = (8*sizeof(int) - width);
  int left = x << shift;
  return left >> shift;
}


/* Conditionally set or clear bits without branching */
/* 0 <= f <= 1 */
int test_bit_set_clear(int f, unsigned m, unsigned w)
{
  if (f) w |= m; else w &= ~m;
  return w;
}

/* Verified with BDD checker 04/07/2005 */
int bit_set_clear_1(int f, unsigned m, unsigned w)
/* ARGS [0,1]:u32:u32 */
/* TEST test_bit_set_clear */
/* STATUS OK */
/* METHOD BDD */
{
  w ^= (-f ^ w) & m;
  return w;
}

int test_bit_count(unsigned int v)
{
  unsigned int c;
  for (c = 0; v; v >>= 1)
    c += v & 1;
  return c;
}

/* Verified with BDD checker 04/07/2005 */
int bit_count_1(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_count */
/* STATUS OK */
/* METHOD BDD */
{
  unsigned int c;
  const unsigned char BitsSetTable256[256] = 
    {
      0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
      4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
    };
  c = BitsSetTable256[v & 0xff] + 
    BitsSetTable256[(v >> 8) & 0xff] + 
    BitsSetTable256[(v >> 16) & 0xff] + 
    BitsSetTable256[v >> 24];
  return c;
}

/* Verified by exhaustive evaluation.  05/03/2005 */
int bit_count_2(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_count */
/* STATUS OK */
/* METHOD EXHAUSTIVE */
{
  unsigned int c;
  const unsigned char BitsSetTable256[256] = 
    {
      0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
      4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
    };
  unsigned char * p = (unsigned char *) &v;
  c = BitsSetTable256[p[0]] + 
    BitsSetTable256[p[1]] + 
    BitsSetTable256[p[2]] +	
    BitsSetTable256[p[3]];
  return c;
}

/* Verified with BDD checker 04/07/2005 */
/* Counting bits set, Brian Kernighan's way */
int bit_count_3(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_count */
/* STATUS OK */
/* METHOD BDD */
{
  unsigned int c;
  for (c = 0; v; c++)
    v &= v - 1; // clear the least significant bit set
  return c;
}


/* ERROR: Doesn't work correctly for 16447488 (0xfaf800).  Gives 10.  Should give 11 */
/* Detected with BDD Checker 04/25/2005 */
int bit_count_4(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_count */
/* STATUS BUG */
/* METHOD BDD */
{
  unsigned int c;
  c = (((v & 0xfff) * 
	0x1001001001001ULL & 0x84210842108421ULL) +
       ((v & 0xfff000) >> 12) * 
       0x1001001001001ULL & 0x84210842108421ULL) % 0x1f; // ERROR: Original had extra ')' here
  c += ((v >> 24) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f; 
  return c;
}

/* REB: Fixed up version */
/* Verified with BDD Checker 04/25/2005 */
int bit_count_4_fix(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_count */
/* STATUS OK */
/* METHOD BDD */
{
  unsigned int c;
  c =  ((v & 0xfff) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
  c += (((v & 0xfff000) >> 12) *  0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
  c += ((v >> 24) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f; 
  return c;
}

/* Verified with BDD checker 04/07/2005 */
/* Counting bits set, in parallel */
int bit_count_5(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_count */
/* STATUS OK */
/* METHOD BDD */
{
  unsigned int c; // store the total here
  const int S[5] = {1, 2, 4, 8, 16}; // Magic Binary Numbers
  const int B[5] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF};
  c = v;
  c = ((c >> S[0]) & B[0]) + (c & B[0]);
  c = ((c >> S[1]) & B[1]) + (c & B[1]);
  c = ((c >> S[2]) & B[2]) + (c & B[2]);
  c = ((c >> S[3]) & B[3]) + (c & B[3]);
  c = ((c >> S[4]) & B[4]) + (c & B[4]);
  return c;
}

/* Verified with BDD Checker 04/25/2005 */
/* For 0 <= v < 1<<12 */
int bit_count12_1(unsigned int v)
/* ARGS u12 */
/* TEST test_bit_count */
/* STATUS OK */
/* METHOD BDD */
{
  unsigned int c; // c accumulates the total bits set in v
  c = (v * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
  return c;
}

/* ERROR: Doesn't work correctly for 8388608 (0x800000). Gives 0.  Should give 1 */
/* Detected with BDD Checker 04/25/2005 */
/* For 0 <= v < 1<<24 */
int bit_count24_1(unsigned int v)
/* ARGS u24 */
/* TEST test_bit_count */
/* STATUS BUG */
/* METHOD BDD */
{
  unsigned int c; // c accumulates the total bits set in v
  c =  (v & 0xfff) * 0x1001001001001ULL & 0x84210842108421ULL;
  c += ((v & 0xfff000) >> 12) * 0x1001001001001ULL & 0x84210842108421ULL;
  c %= 0x1f;
  return c;
}

/* REB: Fixed up version */
/* Verified with BDD Checker 04/25/2005 */
int bit_count24_1_fix(unsigned int v)
/* ARGS u24 */
/* TEST test_bit_count */
/* STATUS OK */
/* METHOD BDD */
{
  unsigned int c; // c accumulates the total bits set in v
  c =  ((v & 0xfff) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
  c += (((v & 0xfff000) >> 12) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
  return c;
}

int test_bit_parity(unsigned int v)
{
  int parity = 0;  // parity will be the parity of b
  while (v)
    {
      parity ^= (v & 1);
      v >>= 1;
    }
  return parity;
}

/* Verified with BDD checker 04/07/2005 */
int bit_parity_1(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_parity */
/* STATUS OK */
/* METHOD BDD */
{
  int parity = 0;  // parity will be the parity of b
  while (v)
    {
      parity = !parity;
      v = v & (v - 1);
    }
  return parity;
}

/* Verified with BDD checker 04/07/2005 */
int bit_parity_2(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_parity */
/* STATUS OK */
/* METHOD BDD */
{
  v ^= v >> 16;
  v ^= v >> 8;
  v ^= v >> 4;
  v &= 0xf;
  return (0x6996 >> v) & 1;
}

/* Verified with BDD checker 04/07/2005 */
int bit_parity_3(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_parity */
/* STATUS OK */
/* METHOD BDD */
{
  const unsigned ParityTable[256] = 
    {
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0
    };
  int parity = ParityTable[(v & 0x000000ff)] ^ 
    ParityTable[(v & 0x0000ff00) >>  8] ^ 
    ParityTable[(v & 0x00ff0000) >> 16] ^ 
    ParityTable[(v & 0xff000000) >> 24];
  return parity;
}

/* An obvious variation not in the bit hacks page */
/* Verfied by exhaustive evaluation.  05/03/2005 */
int bit_parity_4(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_parity */
/* STATUS OK */
/* METHOD EXHAUSTIVE */
{
  const unsigned ParityTable[256] = 
    {
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0
    };
  unsigned char *p = (unsigned char *) &v; /* REB: Added cast to avoid warning from GCC */
  int parity = ParityTable[p[0]] ^
    ParityTable[p[1]] ^ 
    ParityTable[p[2]] ^
    ParityTable[p[3]];
  return parity;
}

/* ERROR (sort of): Long long constant not suffixed with 'ULL'.  GCC does implicit conversion,
   but not generally reliable.
*/
/* Detected with BDD Checker 04/25/2005 */
int bit_parity8_1(unsigned char b)
/* ARGS u8 */
/* TEST test_bit_parity */
/* STATUS BUG */
/* METHOD BDD */
{
  /* Compute parity of a byte using 64-bit multiply and modulus division */
  int parity = (((b * 0x0101010101010101ULL) & 0x8040201008040201) % 0x1FF) & 1;
  return parity;
}

/* Verified with BDD Checker 04/25/2005 */
int bit_parity8_1_fix(unsigned char b)
/* ARGS u8 */
/* TEST test_bit_parity */
/* STATUS OK */
/* METHOD BDD */
{
  /* Compute parity of a byte using 64-bit multiply and modulus division */
  int parity = (((b * 0x0101010101010101ULL) & 0x8040201008040201ULL) % 0x1FF) & 1;
  return parity;
}

/* Verified with BDD checker 04/26/2005 */
int bit_parity8_2(unsigned char v)
/* ARGS u8 */
/* TEST test_bit_parity */
/* STATUS OK */
/* METHOD BDD */
/* Args: [0,255] */
{
  const unsigned ParityTable[256] = 
    {
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0
    };
  return ParityTable[v];
}

/* Swapping individual bits with XOR */
/* Swap the n-bit block between positions i & j in b */
/* Assumes: (i > (j+b-1)) OR (j > (i+b-1)) */
/* Swap the n bit between positions i & j in b */
unsigned int test_bit_swap(unsigned int b, unsigned int i, unsigned int j, unsigned int n)
{
  /* Wrapper */
  if (i <= j) {
    if (j < i+n || j+n > sizeof(int)*8)
      return 0;
  } else {
    if (i < j+n || i+n > sizeof(int)*8)
      return 0;
  }
  {
    /* Regular code */
    int mask = (1 << n) -1;
    int bi = (b >> i) & mask;
    int bj = (b >> j) & mask;
    int maski = mask << i;
    int maskj = mask << j;
    b = (b & ~maski) | (bj << i);
    b = (b & ~maskj) | (bi << j);
    return b;
  }
}

/* Verified with BDD Checker 04/26/2004 */
unsigned int bit_swap_1(unsigned int b, unsigned int i, unsigned int j, unsigned int n)
/* ARGS u32:[0,31]:[0,31]:[0,31] */
/* TEST test_bit_swap */
/* STATUS OK */
/* METHOD BDD */
{
  /* Wrapper */
  if (i <= j) {
    if (j < i+n || j+n > sizeof(int)*8)
      return 0;
  } else {
    if (i < j+n || i+n > sizeof(int)*8)
      return 0;
  }
  {
    /* Regular code */
    int x = ((b >> i) ^ (b >> j)) & ((1 << n) - 1); // XOR temporary
    return b ^ ((x << i) | (x << j));
  }
}

unsigned char test_bit_reverse8(unsigned char v)
{
  unsigned char result = 0;
  int w = sizeof(char)*8;
  int i;
  for (i = 0; i < w; i++) {
    if (v & (1<<i))
      result |= 1<<(w-i-1);
  }
  return result;
}


/* Verified with BDD Checker 04/25/2005 */
unsigned char bit_reverse8_1(unsigned char b)
/* ARGS u8 */
/* TEST test_bit_reverse8 */
/* STATUS OK */
/* METHOD BDD */
{
  return (b * 0x0202020202ULL & 0x010884422010ULL) % 1023;  
}
  

/* Verified with BDD Checker 04/25/2005 */
unsigned char bit_reverse8_2(unsigned char b)
/* ARGS u8 */
/* TEST test_bit_reverse8 */
/* STATUS OK */
/* METHOD BDD */
{
  return ((b * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
}

/* Verified with BDD Checker 04/25/05 */
unsigned char bit_reverse8_3(unsigned char b)
/* ARGS u8 */
/* TEST test_bit_reverse8 */
/* STATUS OK */
/* METHOD BDD */
{
  return ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
}


unsigned int test_bit_reverse(unsigned int v)
{
  unsigned int result = 0;
  int w = sizeof(int)*8;
  int i;
  for (i = 0; i < w; i++) {
    if (v & (1<<i))
      result |= 1<<(w-i-1);
  }
  return result;
}

/* Version of 09/2004 from bit hacks page.  Later corrected on 10/15/2004 */
/* ERROR: Gets wrong answer when MSB of v = 1 */
unsigned int bit_reverse_1(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_reverse */
/* STATUS BUG */
/* METHOD BDD */
{
  unsigned int t = v;     // t will have the reversed bits of v
  int i;
  for (i = sizeof(int) * 8 - 1; i; i--)
    {
      t |= v & 1;
      t <<= 1;
      v >>= 1;
    }
  return t;
}


/* Verified with BDD checker 04/07/2005 */
/* Corrected version (slightly simpler than correction posted) */
unsigned int bit_reverse_1_fix(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_reverse */
/* STATUS OK */
/* METHOD BDD */
{
  unsigned int t = v;     // t will have the reversed bits of v
  int i;
  for (i = sizeof(int) * 8 - 1; i; i--)
    {
      t |= v & 1;
      t <<= 1;
      v >>= 1;
    }
  return t|v;
}

/* Verified with BDD checker 04/07/2005 */
unsigned int bit_reverse_2(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_reverse */
/* STATUS OK */
/* METHOD BDD */
{
  const unsigned char BitReverseTable256[256] = 
    {
      0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
      0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
      0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
      0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
      0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
      0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
      0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
      0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
      0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
      0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
      0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
      0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
      0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
      0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
      0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
      0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
    };
  unsigned int c; // c will get v reversed
  c = (BitReverseTable256[v & 0xff] << 24) | 
    (BitReverseTable256[(v >> 8) & 0xff] << 16) | 
    (BitReverseTable256[(v >> 16) & 0xff] << 8) |
    (BitReverseTable256[(v >> 24) & 0xff]);
  return c;
}

/* Verified by exhaustive evaluation 05/03/2005 */
unsigned int bit_reverse_3(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_reverse */
/* STATUS OK */
/* METHOD EXHAUSTIVE */
{
  const unsigned char BitReverseTable256[256] = 
    {
      0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
      0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
      0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
      0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
      0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
      0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
      0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
      0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
      0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
      0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
      0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
      0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
      0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
      0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
      0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
      0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
    };
  unsigned int c; // c will get v reversed
  unsigned char * p = (unsigned char *) &v;
  unsigned char * q = (unsigned char *) &c;
  q[3] = BitReverseTable256[p[0]]; 
  q[2] = BitReverseTable256[p[1]]; 
  q[1] = BitReverseTable256[p[2]]; 
  q[0] = BitReverseTable256[p[3]];
  return c;
}

/* Verified with BDD checker 04/07/2005 */
unsigned int bit_reverse_4(unsigned int v)
/* ARGS u32 */
/* TEST test_bit_reverse */
/* STATUS OK */
/* METHOD BDD */

/* Args: all */
{
  const int S[5] = {1, 2, 4, 8, 16}; // Magic Binary Numbers
  const int B[5] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF};

  v = ((v >> S[0]) & B[0]) | ((v << S[0]) & ~B[0]); // swap odd and even bits
  v = ((v >> S[1]) & B[1]) | ((v << S[1]) & ~B[1]); // swap consecutive pairs
  v = ((v >> S[2]) & B[2]) | ((v << S[2]) & ~B[2]); // swap nibbles ...
  v = ((v >> S[3]) & B[3]) | ((v << S[3]) & ~B[3]);
  v = ((v >> S[4]) & B[4]) | ((v << S[4]) & ~B[4]);
  return v;
}

/* Compute modulus division by 1 << s without a division operator */
/* Assume 0 <= s < 8*sizeof(int) */
unsigned int test_mod_p2(unsigned int n, unsigned int s)
{
  return n % (1U << s); 
}

/* Verified with BDD checker 04/16/2005 */
unsigned int mod_p2_1(unsigned int n, unsigned int s)
/* ARGS u32:[0,31] */
/* TEST test_mod_p2 */
/* STATUS OK */
/* METHOD BDD */
{
  const unsigned int d = 1 << s; // So d will be one of: 1, 2, 4, 8, 16, 32, ...
  return n & (d - 1); 
}

/* Compute modulus division by (1 << s)-1 without a division operator */
unsigned int test_mod_p2m1(unsigned int n, unsigned int s)
{
  return n % ((1U << s)-1); 
}

/* Not yet verified (Timeout) */
unsigned int mod_p2m1_1(unsigned int n, unsigned int s)
/* ARGS u32:[1,31] */
/* TEST test_mod_p2m1 */
/* STATUS NOTYET */
{
  unsigned m;
  const unsigned int d = (1 << s) - 1; // so d is either 1, 3, 7, 15, 31, ...).
  if (!s)
    return n;
  for (m = n; n > d; n = m) {
    for (m = 0; n; n >>= s)
      {
	m += n & d;
      }
  }
  // Now m is a value from 0 to d, but since with modulus division
  // we want m to be 0 when it is d.
  return m == d ? 0 : m;
}

/* Not yet verified (need pointers) */
unsigned int mod_p2m1_2(unsigned int n, unsigned int s)
/* ARGS u32:[1,31] */
/* TEST test_p2m1 */
/* STATUS NOTYET */
{
  const unsigned int M[32] = 
    {
      0x00000000, 0x55555555, 0x33333333, 0xc71c71c7,  
      0x0f0f0f0f, 0xc1f07c1f, 0x3f03f03f, 0xf01fc07f, 
      0x00ff00ff, 0x07fc01ff, 0x3ff003ff, 0xffc007ff,
      0xff000fff, 0xfc001fff, 0xf0003fff, 0xc0007fff,
      0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff, 
      0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff,
      0x00ffffff, 0x01ffffff, 0x03ffffff, 0x07ffffff,
      0x0fffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff
    };

  const unsigned int Q[32][6] = 
    {
      { 0,  0,  0,  0,  0,  0}, {16,  8,  4,  2,  1,  1}, {16,  8,  4,  2,  2,  2},
      {15,  6,  3,  3,  3,  3}, {16,  8,  4,  4,  4,  4}, {15,  5,  5,  5,  5,  5},
      {12,  6,  6,  6 , 6,  6}, {14,  7,  7,  7,  7,  7}, {16,  8,  8,  8,  8,  8},
      { 9,  9,  9,  9,  9,  9}, {10, 10, 10, 10, 10, 10}, {11, 11, 11, 11, 11, 11},
      {12, 12, 12, 12, 12, 12}, {13, 13, 13, 13, 13, 13}, {14, 14, 14, 14, 14, 14},
      {15, 15, 15, 15, 15, 15}, {16, 16, 16, 16, 16, 16}, {17, 17, 17, 17, 17, 17},
      {18, 18, 18, 18, 18, 18}, {19, 19, 19, 19, 19, 19}, {20, 20, 20, 20, 20, 20},
      {21, 21, 21, 21, 21, 21}, {22, 22, 22, 22, 22, 22}, {23, 23, 23, 23, 23, 23},
      {24, 24, 24, 24, 24, 24}, {25, 25, 25, 25, 25, 25}, {26, 26, 26, 26, 26, 26},
      {27, 27, 27, 27, 27, 27}, {28, 28, 28, 28, 28, 28}, {29, 29, 29, 29, 29, 29},
      {30, 30, 30, 30, 30, 30}, {31, 31, 31, 31, 31, 31}
    };

  const unsigned int R[32][6] = 
    {
      {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},
      {0x0000ffff, 0x000000ff, 0x0000000f, 0x00000003, 0x00000001, 0x00000001},
      {0x0000ffff, 0x000000ff, 0x0000000f, 0x00000003, 0x00000003, 0x00000003},
      {0x00007fff, 0x0000003f, 0x00000007, 0x00000007, 0x00000007, 0x00000007},
      {0x0000ffff, 0x000000ff, 0x0000000f, 0x0000000f, 0x0000000f, 0x0000000f},
      {0x00007fff, 0x0000001f, 0x0000001f, 0x0000001f, 0x0000001f, 0x0000001f},
      {0x00000fff, 0x0000003f, 0x0000003f, 0x0000003f, 0x0000003f, 0x0000003f},
      {0x00003fff, 0x0000007f, 0x0000007f, 0x0000007f, 0x0000007f, 0x0000007f},
      {0x0000ffff, 0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff},
      {0x000001ff, 0x000001ff, 0x000001ff, 0x000001ff, 0x000001ff, 0x000001ff}, 
      {0x000003ff, 0x000003ff, 0x000003ff, 0x000003ff, 0x000003ff, 0x000003ff}, 
      {0x000007ff, 0x000007ff, 0x000007ff, 0x000007ff, 0x000007ff, 0x000007ff}, 
      {0x00000fff, 0x00000fff, 0x00000fff, 0x00000fff, 0x00000fff, 0x00000fff}, 
      {0x00001fff, 0x00001fff, 0x00001fff, 0x00001fff, 0x00001fff, 0x00001fff}, 
      {0x00003fff, 0x00003fff, 0x00003fff, 0x00003fff, 0x00003fff, 0x00003fff}, 
      {0x00007fff, 0x00007fff, 0x00007fff, 0x00007fff, 0x00007fff, 0x00007fff}, 
      {0x0000ffff, 0x0000ffff, 0x0000ffff, 0x0000ffff, 0x0000ffff, 0x0000ffff}, 
      {0x0001ffff, 0x0001ffff, 0x0001ffff, 0x0001ffff, 0x0001ffff, 0x0001ffff}, 
      {0x0003ffff, 0x0003ffff, 0x0003ffff, 0x0003ffff, 0x0003ffff, 0x0003ffff}, 
      {0x0007ffff, 0x0007ffff, 0x0007ffff, 0x0007ffff, 0x0007ffff, 0x0007ffff},
      {0x000fffff, 0x000fffff, 0x000fffff, 0x000fffff, 0x000fffff, 0x000fffff}, 
      {0x001fffff, 0x001fffff, 0x001fffff, 0x001fffff, 0x001fffff, 0x001fffff}, 
      {0x003fffff, 0x003fffff, 0x003fffff, 0x003fffff, 0x003fffff, 0x003fffff}, 
      {0x007fffff, 0x007fffff, 0x007fffff, 0x007fffff, 0x007fffff, 0x007fffff}, 
      {0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff},
      {0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff}, 
      {0x03ffffff, 0x03ffffff, 0x03ffffff, 0x03ffffff, 0x03ffffff, 0x03ffffff}, 
      {0x07ffffff, 0x07ffffff, 0x07ffffff, 0x07ffffff, 0x07ffffff, 0x07ffffff},
      {0x0fffffff, 0x0fffffff, 0x0fffffff, 0x0fffffff, 0x0fffffff, 0x0fffffff},
      {0x1fffffff, 0x1fffffff, 0x1fffffff, 0x1fffffff, 0x1fffffff, 0x1fffffff}, 
      {0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff}, 
      {0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff}
    };

  const unsigned int d = (1 << s) - 1; // so d is either 1, 3, 7, 15, 31, ...).
  unsigned int m;       // n % d goes here.   /* ERROR: Typo here: Says 'unsinged' */
  unsigned int *q;

  m = (n & M[s]) + ((n & ~M[s]) >> s);

  for (q = &Q[s][0], (*r = &R[s][0]); m > d; q++, r++)
    m = (m >> *q) + (m & *r);
  m = m == d ? 0 : m; // Or: m = ((m + 1) & d) - 1;
  return m;
}

/* Find the log base 2 of an integer with the MSB N set in O(N) operations (the obvious way) */

unsigned int test_log2(unsigned int v)
{
  unsigned c = 0; // c will be lg(v)
  while (v >>= 1) 
    {
      c++;
    }
  return c;
}

/* Verified with BDD checker 04/16/2005 */
unsigned int log2_1(unsigned int v)
/* ARGS u32 */
/* TEST test_log2 */
/* STATUS OK */
{
  const char LogTable256[256] = 
    {
      0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
      4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
      5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
      5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
    };
  unsigned c = 0; // c will be lg(v)
  unsigned int t, tt; // temporaries
  if (tt = v >> 16)
    c = (t = v >> 24) ? 24 + LogTable256[t] : 16 + LogTable256[tt & 0xFF];
  else 
    c = (t = v & 0xFF00) ? 8 + LogTable256[t >> 8] : LogTable256[v & 0xFF];
  return c;
}

/* Verified with BDD checker 04/16/2005 */
unsigned int log2_2(unsigned int v)
/* ARGS u32 */
/* TEST test_log2 */
/* STATUS OK */
/* METHOD BDD */
{
  const unsigned int b[5] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
  const unsigned int S[5] = {1, 2, 4, 8, 16};
  int i;

  unsigned int c = 0; // result of log2(v) will go here
  for (i = 4; i >= 0; i--) // unroll for speed...
    if (v & b[i])
      {
	v >>= S[i];
	c |= S[i];
      }
  return c;
}

/* Compute log2(v) when v is a power of 2.  Implement as identify function */
unsigned int test_p2_log2(unsigned char p)
{
  unsigned v = 1<<p; /* This is the real function argument */
  return p;
}

/* Verified with BDD checker 04/26/2005 */
unsigned int p2_log2(unsigned char p)
/* ARGS [0,31] */
/* TEST test_p2_log2 */
/* STATUS OK */
{
  unsigned v = 1<<p; /* This is the real function argument */
  const unsigned int b[5] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 0xFF00FF00, 
			    0xFFFF0000};
  unsigned int c = (v & b[0]) != 0;
  int i;
  for (i = 4; i >= 1; i--) // unroll for speed...
    c |= ((v & b[i]) != 0) << i;
  return c;
}

/* Find integer log base 2 of a 32-bit IEEE float */

/* Use wrapper to make it look like operating on unsigned */
int test_float_log2(unsigned uv)
{
  int ilogbf(float);
  float f = *(float *) &uv;
  double rbase = 1.0/log(2.0);
  return ilogbf(f);
}

/* Verified by exhaustive evaluation, 05/03/2005 */
/* Works for normalized, positive numbers (hex 0x00800000 to 0x7effffff) */
/* Doesn't handle denormalized numbers properly */
int float_log2_1(unsigned u)
/* ARGS [8388608,2130706431] */
/* TEST NONE */
/* STATUS OK */
/* METHOD EXHAUSTIVE */
{
  float v = *(float *) & u; /* Wrapper */
  int c;         // c gets the result;
  c = ((*(int *) &v) >> 23) - 127;
  return c;
}

/* Verified by exhaustive evaluation 05/03/2005 */
/* Works for all positive numbers (hex 0x00000001 to 0x7effffff) */
int float_log2_2(unsigned u)
/* ARGS [1,2130706431] */
/* TEST NONE */
/* STATUS OK */
/* METHOD EXHAUSTIVE */
{
  float v = *(float *) & u; /* Wrapper */
  const char LogTable256[] = 
    {
      0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
      4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
      5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
      5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
    };
  const int x = *(int *) &v; 
  int c = x >> 23; // c gets the result;
  if (c)
    c -= 127;
  else
    { // subnormal, so recompute using mantissa: c = intlog2(x) - 149;
      unsigned int t; // temporary
      if (t = x >> 16)
	c = LogTable256[t] - 133;
      else
	c = (t = x >> 8) ? LogTable256[t] - 141 : LogTable256[x] - 149;
    }
  return c;
}

/* SKIP THIS
Find integer log base 2 of the pow(2, r)-root of a 32-bit IEEE float (for 
unsigned integer r) 
const int r;
const float v; // find int(log2(pow(v, 1. / pow(2, r))), where isnormal(v)
int c;         // c gets the result;

c = (((((*(int *) &v) - 0x3f800000) >> r) + 0x3f800000) >> 23) - 127;
So, if r is 0, for example, we have c = int(log2(v)). If r is 1, then we have c 
= int(log2(sqrt(r))). If r is 2, then we have c = int(log2(pow(r, 1./4))). 
*/


/* Count the consecutive zero bits (trailing) on the right in parallel */
/* so if v is 01101000 (base 2), then c will be 3 as output. */
int test_rightzero_cnt(unsigned int v)
{
  int c = sizeof(int)*8;
  while (v) {
    v <<= 1;
    c--;
  }
  return c;
}

/* Verified with BDD checker 04/15/2005 */
int rightzero_cnt_1(unsigned int v)
/* ARGS u32 */
/* TEST test_rightzero_cnt */
/* STATUS OK */
/* METHOD BDD */
{
  unsigned int c = 32; // c will be the number of zero bits on the right,
  const unsigned int B[5] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF};
  const unsigned int S[5] = {1, 2, 4, 8, 16}; // Our Magic Binary Numbers
  if (v & B[4])
    {
      v <<= S[4];
      c -= S[4];
    }
  if (v & B[3])
    {
      v <<= S[3];
      c -= S[3];   
    }
  if (v & B[2])
    {
      v <<= S[2];
      c -= S[2];   
    }
  if (v & B[1])
    {
      v <<= S[1];
      c -= S[1];   
    }
  if (v & B[0])
    {
      v <<= S[0];
      c -= S[0];
    }
  if (v)
    {
      c--;
    }
  return c;
}

/* Verified with BDD checker 04/16/2005 */
int rightzero_cnt_2(unsigned int v)
/* ARGS u32 */
/* TEST test_rightzero_cnt */
/* STATUS OK */
/* METHOD BDD */
{
  int r;           // put the result in r
  const int Mod37BitPosition[37] = // maps a bit value mod 37 to its position
    {
      32,0,1,26,2,23,27,0,3,16,24,30,28,11,0,13,4,
      7,17,0,25,22,31,15,29,10,12,6,0,21,14,9,5,
      20,8,19,18
    };
  r = Mod37BitPosition[(-v & v) % 37];
  return r;
}


/* Verified with BDD checker 04/16/2005 */
/* Round up to the next highest power of 2 */
unsigned int test_p2_roundup(unsigned int v)
{
  unsigned p = 1;
  while (p < v)
    p <<= 1;
  return p;
}

/* Requires v <= 2^31 */
unsigned int p2_roundup_1(unsigned int v)
/* ARGS [1,2147483647] */
/* TEST test_p2_roundup */
/* STATUS OK */
/* METHOD BDD */
{
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

unsigned int test_interleave(unsigned short x, unsigned short y)
{
  unsigned int z = 0; // z gets the resulting 32-bit Morton Number.
  int i;
  for (i = 0; i < sizeof(unsigned short) * 8; i++) // unroll for more speed...
    z |= (x & 1 << i) << i | (y & 1 << i) << (i + 1);
  return z;
}

/* Verified with BDD checker 04/15/2005 */
unsigned int interleave_1(unsigned short x, unsigned short y)
/* ARGS u16:u16 */
/* TEST test_interleave */
/* STATUS OK */
/* METHOD BDD */
{
  const unsigned short MortonTable256[256] = 
    {
      0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015, 
      0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055, 
      0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115, 
      0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155, 
      0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415, 
      0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455, 
      0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515, 
      0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555, 
      0x1000, 0x1001, 0x1004, 0x1005, 0x1010, 0x1011, 0x1014, 0x1015, 
      0x1040, 0x1041, 0x1044, 0x1045, 0x1050, 0x1051, 0x1054, 0x1055, 
      0x1100, 0x1101, 0x1104, 0x1105, 0x1110, 0x1111, 0x1114, 0x1115, 
      0x1140, 0x1141, 0x1144, 0x1145, 0x1150, 0x1151, 0x1154, 0x1155, 
      0x1400, 0x1401, 0x1404, 0x1405, 0x1410, 0x1411, 0x1414, 0x1415, 
      0x1440, 0x1441, 0x1444, 0x1445, 0x1450, 0x1451, 0x1454, 0x1455, 
      0x1500, 0x1501, 0x1504, 0x1505, 0x1510, 0x1511, 0x1514, 0x1515, 
      0x1540, 0x1541, 0x1544, 0x1545, 0x1550, 0x1551, 0x1554, 0x1555, 
      0x4000, 0x4001, 0x4004, 0x4005, 0x4010, 0x4011, 0x4014, 0x4015, 
      0x4040, 0x4041, 0x4044, 0x4045, 0x4050, 0x4051, 0x4054, 0x4055, 
      0x4100, 0x4101, 0x4104, 0x4105, 0x4110, 0x4111, 0x4114, 0x4115, 
      0x4140, 0x4141, 0x4144, 0x4145, 0x4150, 0x4151, 0x4154, 0x4155, 
      0x4400, 0x4401, 0x4404, 0x4405, 0x4410, 0x4411, 0x4414, 0x4415, 
      0x4440, 0x4441, 0x4444, 0x4445, 0x4450, 0x4451, 0x4454, 0x4455, 
      0x4500, 0x4501, 0x4504, 0x4505, 0x4510, 0x4511, 0x4514, 0x4515, 
      0x4540, 0x4541, 0x4544, 0x4545, 0x4550, 0x4551, 0x4554, 0x4555, 
      0x5000, 0x5001, 0x5004, 0x5005, 0x5010, 0x5011, 0x5014, 0x5015, 
      0x5040, 0x5041, 0x5044, 0x5045, 0x5050, 0x5051, 0x5054, 0x5055, 
      0x5100, 0x5101, 0x5104, 0x5105, 0x5110, 0x5111, 0x5114, 0x5115, 
      0x5140, 0x5141, 0x5144, 0x5145, 0x5150, 0x5151, 0x5154, 0x5155, 
      0x5400, 0x5401, 0x5404, 0x5405, 0x5410, 0x5411, 0x5414, 0x5415, 
      0x5440, 0x5441, 0x5444, 0x5445, 0x5450, 0x5451, 0x5454, 0x5455, 
      0x5500, 0x5501, 0x5504, 0x5505, 0x5510, 0x5511, 0x5514, 0x5515, 
      0x5540, 0x5541, 0x5544, 0x5545, 0x5550, 0x5551, 0x5554, 0x5555
    };
  unsigned int z;   // z gets the resulting 32-bit Morton Number.
  z = (MortonTable256[y & 0xFF] << 1) | MortonTable256[x & 0xFF] |
    (MortonTable256[y >> 8] << 1 | MortonTable256[x >> 8]) << 16;
  return z;
}

/* Verified with BDD checker 04/15/2005 */
unsigned int interleave_2(unsigned short x, unsigned short y)
/* ARGS u16:u16 */
/* TEST test_interleave */
/* STATUS OK */
/* METHOD BDD */
{
  const unsigned int B[4] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
  const unsigned int S[4] = {1, 2, 4, 8};
  unsigned int z; // z gets the resulting 32-bit Morton Number.
  /* Need full word versions of x and y */
  unsigned int tx = x;
  unsigned int ty = y;
  tx = (tx | (tx << S[3])) & B[3];
  tx = (tx | (tx << S[2])) & B[2];
  tx = (tx | (tx << S[1])) & B[1];
  tx = (tx | (tx << S[0])) & B[0];
  ty = (ty | (ty << S[3])) & B[3];
  ty = (ty | (ty << S[2])) & B[2];
  ty = (ty | (ty << S[1])) & B[1];
  ty = (ty | (ty << S[0])) & B[0];
  z = tx | (ty << 1);
  return z;
}

unsigned short test_interleave16(unsigned char x, unsigned char y)
{
  unsigned int z = 0; // z gets the resulting 16-bit Morton Number.
  int i;
  for (i = 0; i < sizeof(unsigned char) * 8; i++) // unroll for more speed...
    z |= (x & 1 << i) << i | (y & 1 << i) << (i + 1);
  return z;
}

/* Verified with BDD Checker 04/26/2005 */
unsigned short interleave16_1(unsigned char x, unsigned char y)
/* ARGS u8:u8 */
/* TEST test_interleave16 */
/* STATUS OK */
/* METHOD BDD */
{
  unsigned short z; // z gets the resulting 16-bit Morton Number.

  z =
    ((x * 0x0101010101010101ULL & 0x8040201008040201ULL) * 
     0x0102040810204081ULL >> 49) & 0x5555 |
    ((y * 0x0101010101010101ULL & 0x8040201008040201ULL) * 
     0x0102040810204081ULL >> 48) & 0xAAAA;
  return z;
}

int test_has_zero_byte(unsigned int v)
{
  int i;
  for (i = 0; i < sizeof(int); i++)
    if (!((v>>(8*i)) & 0xFF))
      return 1;
  return 0;
}


/* Verified with BDD checker 04/15/2005 */
int has_zero_byte_1(unsigned int v)
/* ARGS u32 */
/* TEST test_has_zero_byte */
/* STATUS OK */
/* METHOD BDD */
{
  int hasZeroByte = ~((((v & 0x7F7F7F7F) + 0x7F7F7F7F) | v) | 0x7F7F7F7F) != 0;
  return hasZeroByte;
}

/* Verified with BDD checker 04/15/2005 */
int has_zero_byte_2(unsigned int v)
/* ARGS u32 */
/* TEST test_has_zero_byte */
/* STATUS OK */
/* METHOD BDD */
{
  int hasNoZeroByte = ((v & 0xff) && (v & 0xff00) && (v & 0xff0000) && (v & 0xff000000));
  return !hasNoZeroByte;
}

/* Verified by exhaustive evaluation */
int has_zero_byte_3(unsigned int v)
/* ARGS u32 */
/* TEST test_has_zero_byte */
/* STATUS OK */
/* METHOD EXHAUSTIVE */
{
  unsigned char * p = (unsigned char *) &v;  
  int hasNoZeroByte = *p && *(p + 1) && *(p + 2) && *(p + 3);
  return !hasNoZeroByte;
}

/*
first zeroing the high bits of the 4 bytes in the word. Next, it adds a number 
that will result in an overflow to the high bit of a byte if any of the low bits 
were initialy set. Next the high bits of the original word are ORed with these 
values; thus, the high bit of a byte is set iff any bit in the byte was set. 
Finally, we determine if any of these high bits are zero by ORing with ones 
everywhere except the high bits and inverting the result. Extending to 64 bits 
is trivial; simply increase the constants to be 0x7F7F7F7F7F7F7F7F. 
*/

/* Verified 04/26/2005 with BDD Checker */
int has_zero_byte_4(unsigned int v)
/* ARGS u32 */
/* TEST test_has_zero_byte */
/* STATUS OK */
/* METHOD BDD */
{
  int mayHaveZeroByte = ((v + 0x7efefeff) ^ ~v) & 0x81010100;
  if (mayHaveZeroByte) // or may just have 0x80 in the high byte
    {
      return (~((((v & 0x7F7F7F7F) + 0x7F7F7F7F) | v) | 0x7F7F7F7F)) != 0;
    }
  return 0;
}

int test_has_byte_less(unsigned int x, unsigned char n)
{
  int i;
  for (i = 0; i < sizeof(int); i++)
    if (((x>>(8*i)) & 0xFF) < n)
      return 1;
  return 0;
}

/* Verified with BDD Checker 04/26/2005 */
int has_byte_less_1(unsigned int x, unsigned char n)
/* ARGS u32:[0,128] */
/* TEST test_has_byte_less */
/* STATUS OK */
/* METHOD BDD */
{
  return (((x)-~0UL/255*(n))&~(x)&~0UL/255*128) != 0;
}

int test_count_byte_less(unsigned int x, unsigned char n)
{
  int i;
  int r = 0;
  for (i = 0; i < sizeof(int); i++)
    if (((x>>(8*i)) & 0xFF) < n)
      r++;
  return r;
}

/* Verified with BDD Checker 04/26/2005 */
int count_byte_less_1(unsigned int x, unsigned char n)
/* ARGS u32:[0,128] */
/* TEST test_count_byte_less */
/* STATUS OK */
/* METHOD BDD */
{
  return (((~0UL/255*(127+(n))-((x)&~0UL/255*127))&~(x)&~0UL/255*128)/128%255);
}

int test_has_byte_greater(unsigned int x, unsigned char n)
{
  int i;
  for (i = 0; i < sizeof(int); i++)
    if (((x>>(8*i)) & 0xFF) > n)
      return 1;
  return 0;
}

/* Verified with BDD Checker 04/28/2005 */
int has_byte_greater_1(unsigned int x, unsigned char n)
/* ARGS u32:[0,127] */
/* TEST test_has_byte_greater */
/* STATUS OK */
/* METHOD BDD */
{
  return (((x)+~0UL/255*(127-(n))|(x))&~0UL/255*128) != 0;
}

int test_count_byte_greater(unsigned int x, unsigned char n)
{
  int i;
  int r = 0;
  for (i = 0; i < sizeof(int); i++)
    if (((x>>(8*i)) & 0xFF) > n)
      r++;
  return r;
}

/* Verified with BDD Checker 04/26/2005 */
int count_byte_greater_1(unsigned int x, unsigned char n)
/* ARGS u32:[0,127] */
/* TEST test_count_byte_greater */
/* STATUS OK */
/* METHOD BDD */
{
  return (((((x)&~0UL/255*127)+~0UL/255*(127-(n))|(x))&~0UL/255*128)/128%255);
}

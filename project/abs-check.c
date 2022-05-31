#include <stdint.h>



int32_t abs_ref(int32_t x) {
    return x < 0 ? -x : x;
}

int32_t abs_bits(int32_t x) {
    int32_t m = x>> (sizeof(int32_t) * 8 - 1);
    return (x^m) + ~m + 1;

}

int32_t arbitrary();

int32_t main() {
    int32_t t = arbitrary();
    int32_t ar = abs_ref(t);
    int32_t ab = abs_bits(t);
    int32_t err = ar != ab;
    assert(!err);
}

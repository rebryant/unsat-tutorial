int abs_ref(int x) {
    return x < 0 ? -x : x;
}

int abs_bits(int x) {
  int m = x>>31;
  return (x^m) + ~m + 1;

}

int main() {
    int t = random();
    int ar = abs_ref(t);
    int ab = abs_bits(t);
    int err = ar != ab;
    assert(!err);
}

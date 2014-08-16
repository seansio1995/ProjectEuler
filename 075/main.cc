#include <iostream>

using namespace std;

int gcd(int a, int b) {
  return b == 0 ? a : gcd(b, a % b);
}

const int MAX = 1500000;

// Every primitive Pythagorean triple is generated by a unique pair of coprime
// integers n < m, exactly one of which is even using Euclid's formula:
//
// a = m^2 - n^2    b = 2mn    c = m^2 + n^2

int main() {
  int counts[MAX + 1];

  for (int i = 0; i <= MAX; i++) {
    counts[i] = 0;
  }

  bool done = false;
  for (int m = 2; !done; m++) {
    for (int n = 1; n < m; n++) {
      bool m_even = (m % 2 == 0);
      bool n_even = (n % 2 == 0);
      bool one_even = (m_even != n_even);
      if (gcd(m, n) == 1 && one_even) {
        int a = m * m - n * n;
        int b = 2 * m * n;
        int c = m * m + n * n;
        int L = a + b + c;
        if (L > MAX) {
          done = (n == 1);
          break;
        }
        for (int t = 1; t * L <= MAX; t++) {
          counts[t * L]++;
        }
      }
    }
  }

  int count = 0;
  for (int i = 1; i <= MAX; i++) {
    if (counts[i] == 1) count++;
  }

  cout << count << endl;

  return 0;
}
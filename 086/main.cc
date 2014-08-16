#include <cassert>

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#include <utility>

using namespace std;

int gcd(int a, int b) {
  return b == 0 ? a : gcd(b, a % b);
}

bool IsSquare(int n) {
  int next_square = 0;
  int num_squares = 0;
  while (next_square <= n) {
    if (next_square == n) return true;
    next_square += 2 * num_squares + 1;
    num_squares++;
  }
  return false;
}

void GetAllSolutionsSlow(int M, vector<vector<int>>* solutions) {
  for (int a = 1; a <= M; a++) {
    for (int b = 1; b <= a; b++) {
      for (int c = 1; c <= b; c++) {
        // The shortest path is sqrt(a^2 + (b+c)^2)
        if (IsSquare(a * a + b * b + c * c + 2 * b * c)) {
          vector<int> solution = {a, b, c};
          solutions->push_back(solution);
        } 
      }
    }
  }
}

int GetSolutionsCount(int x, int y, int M) {
  // If y <= M then there are floor(x / 2) possible solutions with a = y.
  int count = (y <= M ? x / 2 : 0);

  // The number of solutions with a = x is max(0, 1 + x - ceil(y / 2))
  count += max(0, 1 + x - (y % 2 == 0 ? y / 2 : (y + 1) / 2));

  return count;
}

void GetSolutions(int x, int y, int M, vector<vector<int>>* solutions) {
  // cout << "x = " << x << ", y = " << y << endl;
  assert(x <= y);
  int a = y;
  int b = x - 1;
  int c = 1;
  while (a <= M && c <= b) {
    vector<int> solution = {a, b, c};
    solutions->push_back(solution);
    // cout << a << " " << b << " " << c << endl;
    b--;
    c++;
  }

  a = x;
  b = x;
  c = y - x;
  while (a <= M && c <= b) {
    vector<int> solution = {a, b, c};
    solutions->push_back(solution);
    // cout << a << " " << b << " " << c << endl;
    b--;
    c++;
  }
}

void GetAllSolutionsFast(int M, vector<vector<int>>* solutions) {
  bool done = false;
  for (int m = 2; !done; m++) {
    for (int n = 1; n < m; n++) {
      if (gcd(n, m) == 1 && (m - n) % 2 == 1) {
        int x = m * m - n * n;
        int y = 2 * m * n ;
        int z = m * m + n * n;
        if (x > y) swap(x, y);
        // Now we have x < y < z

        int t = 1;
        while (t * x <= M && t * y <= 2 * M) {
          GetSolutions(t * x, t * y, M, solutions);
          t++;
        }
        if (t == 1 && n == 1) done = true;
      }
    }
  }
}

int GetAllSolutionsCount(int M) {
  int count = 0;
  bool done = false;
  for (int m = 2; !done; m++) {
    for (int n = 1; n < m; n++) {
      if (gcd(n, m) == 1 && (m - n) % 2 == 1) {
        int x = m * m - n * n;
        int y = 2 * m * n ;
        int z = m * m + n * n;
        if (x > y) swap(x, y);
        // Now we have x < y < z

        int t = 1;
        while (t * x <= M && t * y <= 2 * M) {
          count += GetSolutionsCount(t * x, t * y, M);
          t++;
        }
        if (t == 1 && n == 1) done = true;
      }
    }
  }
  return count;
}

// If we can find the number of solutions for a given M, then we can perform a
// binary search on M to find the minimal M that is above the threshold.
//
// The naive solution is then to loop over all side lengths (a, b, c) with
// M >= a >= b >= c; then the shortest path is given by sqrt(a^2 + (b+c)^2) so
// the triple (a, b, c) is a solution if a^2+(b+c)^2 = a^2+b^2+c^2+2bc is an a
// square. This method is implemented by GetAllSolutionsSlow.
//
// Unfortunately this is waaaay too slow, so we need a different approach.
// Note that for any solution (a, b, c) that (a, b+c) form part of a unique
// pythagorean triple. Therefore to generate all solutions (a, b, c), we
// generate all possible pythagorean triples (x, y, z) (with x <= y < z).
// Then for each pythagorean triple, we count the number of ways that x and y
// can be divided into a solution (a, b, c). There are a few cases to consider;
// these cases are made explicit in GetSolutions(int, int, int, vector<vector<int>>*).
// With a bit of thought we avoid looping and just write down an explicit
// formula for the number of solutions generated by a pythagorean triple;
// this is done in GetSolutionsCount(int, int, int).

int main() {
  int target = 1000000;
  int lowM = 1;
  int highM = 1;
  
  while (GetAllSolutionsCount(highM) < target) {
    highM *= 2;
    cout << lowM << " " << highM << endl;
  }

  while (lowM + 1 != highM) {
    int mid = (lowM + highM) / 2;
    if (GetAllSolutionsCount(mid) < target) {
      lowM = mid;
    } else {
      highM = mid;
    }
    cout << lowM << " " << highM << endl;
  }

  cout << highM << endl;

  return 0;
}
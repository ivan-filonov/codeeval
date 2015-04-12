#include <fstream>
#include <iostream>
#include <vector>

#define TEST

namespace {
#ifdef TEST
  void test();
#endif //#ifdef TEST
  void process_file(char*);
}

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

namespace {
  void process(std::string s);

#ifdef TEST
  void test() {
    std::vector<std::string> v_test {
      "250*14.3",
      "3^6 / 117",
      "(2.16 - 48.34)^-1",
      "(59 - 15 + 3*6)/21",
      "lg(10) + ln(e)",
      "15*5 mod 2",
    };
    std::vector<std::string> v_expect {
      "3575",
      "6.23077",
      "−0.02165",
      "2.95238",
      "2",
      "15",
    };
    for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
      process(v_test[i]);
      std::cout << v_expect[i] << "\n";
    }
  }
#endif //#ifdef TEST

  void process_file(char* path) {
    std::ifstream stream(path);
    for(std::string line; std::getline(stream, line); ) {
      process(line);
    }
  }

/*
    Pi        Pi number
    e         Euler's number
    sqrt()    Square root
    cos()     Cosine (using radians as an argument)
    sin()     Sine (using radians as an argument)
    tan()     Tangent (using radians as an argument)
    lg()      Decimal logarithm
    ln()      Natural logarithm
1   ()        Brackets
2   ||        Absolute value, e.g. |-5 - 10|
3   !         Factorial
4   -         Unary minus
5   ^         Exponent
6   mod       Modulus divide, e.g. 5 mod 2 = 1 (only integers will be supplied here)
7   *, /      Multiply, Divide (left-to-right precedence)
8   +, -      Add, Subtract (left-to-right precedence) 

Constraints:
Each number in input expression is greater than -20,000 and less than 20,000.
Each output number is greater than -20,000 and less than 20,000.
If output number is a float number it should be rounded to the 5th digit after the dot.
E.g 14.132646 gets 14.13265, 14.132644 gets 14.13264, 14.132645 gets 14.13265.

If output number has less than 5 digits after the dot you don't need to add zeros.
E.g. you need to print 16.34 (and not 16.34000) in case the answer is 16.34.
And you need to print 16 (and not 16.00000) in case the answer is 16. 
 * */

  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  //    std::istringstream ss { line };
  }
}

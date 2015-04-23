#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>

#ifdef TEST
void test();
#endif //#ifdef TEST
void process_file(char*);

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

using std::string;
template<typename V> using vector = std::vector<V>;

bool buildings = true;

struct building {
  string name;
  vector<double> x;
  vector<double> y;
};

vector<building> vb;

// NAME x;y ...
void process_building(string line) {
#ifdef TEST
  std::cout << "b = '" << line << "'\n";
#endif //#ifdef TEST
  std::istringstream ss { line };
  vb.emplace_back();
  std::getline(ss, vb.back().name, ' ');
  for(string tx, ty; std::getline(ss, tx, ';') && std::getline(ss, ty, ' ');) {
    vb.back().x.push_back(std::stod(tx));
    vb.back().y.push_back(std::stod(ty));
  }
}

struct spot {
  string MAC;
  vector<double> x;
  vector<double> y;
  vector<double> dir;
};

vector<spot> vs;

// coords MAX;asimut
void process_point(string line) {
#ifdef TEST
  std::cout << "p = '" << line << "'\n";
#endif //#ifdef TEST
  std::istringstream ss { line };
  string t;
  std::getline(ss, t, ';');
  auto x = std::stod(t);
  std::getline(ss, t, ' ');
  auto y = std::stod(t);
  for(string mac, dir; std::getline(ss, mac, ';') && std::getline(ss, dir, ' ');) {
    auto d = std::stod(dir);
    spot * s = nullptr;
    for(auto & sp : vs) {
      if(sp.MAC == mac) {
        s = &sp;
        break;
      }
    }
    if(nullptr == s) {
      vs.emplace_back();
      s = &vs.back();
      s->MAC = mac;
    }
    s->x.push_back(x);
    s->y.push_back(y);
    s->dir.push_back(90 - d);
  }
}

void process(string line) {
  if(line.empty()) {
    buildings = false;
  } else {
    if(buildings) {
      process_building(line);
    } else {
      process_point(line);
    }
  }
}

void print_results() {
  using std::sin;
  using std::cos;
  for(auto & s : vs) {
    if(s.x.size() < 2) {
      continue;
    }
    std::cout << "MAC = '" << s.MAC << "', detects: " << s.x.size() << "\n";
    double c = 0;
    double ax = 0;
    double ay = 0;
    for(size_t i = 1; i != s.x.size(); ++i) {
      auto x1 = s.x[i-1];
      auto y1 = s.y[i-1];
      auto d1 = s.dir[i-1] * M_PI / 180.0;
      auto x2 = s.x[i];
      auto y2 = s.y[i];
      auto d2 = s.dir[i] * M_PI / 180.0;
      // x1 + t1 * cos(d1) = x2 + t2 * cos(d2)
      // y1 + t1 * sin(d1) = y2 + t2 * sin(d2)
      // t1 * cos(d1) - t2 * cos(d2) = x2 - x1
      // t1 * sin(d1) - t2 * sin(d2) = y2 - y1
      // t1 * cos(d1) * sin(d1) - t2 * cos(d2) * sin(d1) = (x2 - x1) * sin(d1)
      // t1 * sin(d1) * cos(d1) - t2 * sin(d2) * cos(d1) = (y2 - y1) * cos(d1)
      // t2 * sin(d2) * cos(d1) - t2 * cos(d2) * sin(d1) = (x2 - x1) * sin(d1) - (y2 - y1) * cos(d1)
      auto v1 = sin(d2) * cos(d1) - cos(d2) * sin(d1);
      auto v2 = (x2 - x1) * sin(d1) - (y2 - y1) * cos(d1);
      auto t2 = v2 / v1;
      x2 += t2 * cos(d2);
      y2 += t2 * sin(d2);
      c += 1.0;
      ax += x2;
      ay += y2;
    }
    ax /= c;
    ay /= c;
    std::cout << "X = " << ax << ", Y = " << ay << "\n";
    // now find building
    ;
  }
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "B001 14.88;8.94 14.88;33.23 25.29;33.23 25.29;15.88 32.23;15.88 32.23;8.94 14.88;8.94",
    "B002 14.88;33.23 14.88;43.64 49.58;43.64 49.58;26.29 39.17;26.29 39.17;33.23 14.88;33.23",
    "B010 63.45;50.58 70.39;50.58 70.39;43.64 63.45;43.64 63.45;50.58",
    "",
    "56.51;5.47 56-4c-18-eb-13-8b;59.3493 88-fe-14-a4-aa-2a;303.0239",
    "42.64;5.47 88-fe-14-a4-aa-2a;0.0000",
    "28.76;5.47 88-fe-14-a4-aa-2a;56.9761",
    "14.88;5.47 88-fe-14-a4-aa-2a;71.9958",
    "11.41;15.88",
    "11.41;29.76",
    "56.51;64.45 f9-aa-de-15-28-46;277.5946 de-c2-8e-34-08-17;214.6952",
  };
  for(auto & s : v_test) {
    process(s);
  }
  print_results();
  vector<string> v_expect {
    "B003",
    "B005",
    "B007",
  };
  for(auto & s : v_expect) {
    std::cout << s << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    process(line);
  }
}

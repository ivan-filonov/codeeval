#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
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
template<typename V> using set = std::set<V>;
template<typename K, typename V> using map = std::map<K,V>;

using std::move;
using std::swap;

vector<string> seed_words;
vector<string> words;

const string SEED_END { "END OF INPUT" };
bool reading_seeds = true;
void add_line(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  if(reading_seeds) {
    if(SEED_END != line) {
      seed_words.emplace_back(move(line));
    } else {
      reading_seeds = false;
    }
  } else {
    words.emplace_back(move(line));
  }
}

vector<int> vngrams;
vector<int> & ngrams(const string & w) {
  vngrams.clear();
  int v = '$';
  for(char c : w) {
    v = ((v & 0xff) << 8) | c;
    vngrams.push_back(v);
  }
  v = ((v & 0xff) << 8) | '$';
  vngrams.push_back(v);
  return vngrams;
}

map<int,std::shared_ptr<set<int>>> refs;

bool fw (const string & s1, const string & s2) {
  size_t e1 = s1.length();
  size_t e2 = s2.length();
  if(e1 == e2) {
    for(size_t i = 0, j = 0; i != e1; ++i) {
      if(s1[i] != s2[i]) {
        if(j) {
          return false;
        }
        ++j;
      }
    }
    return true;
  }
  if(e1 - e2 == 1 || e2 - e1 == 1) {
    size_t i1 = 0;
    size_t i2 = 0;
    for(int j = 0; i1 != e1 && i2 != e2; ++i1, ++i2) {
      if(s1[i1] != s2[i2]) {
        if(j) {
          return false;
        }
        ++j;
        ((e1 > e2) ? i1 : i2)++;
      }
    }
    return true;
  }
  return false;
}

void run() {
  // 1. build index
  for(int i = 0; i != words.size(); ++i) {
    const auto & w = words[i];
    const auto wl = w.length();
    for(auto ng : ngrams(w)) {
      auto ngx = ng << 8 | wl;
      if(!refs.count(ngx)) {
        refs[ngx] = std::make_shared<set<int>>();
      }
      refs[ngx]->insert(i);
    }
  }
  // 2. process seed words
  for(auto & sw : seed_words) {
    vector<string> nw { sw };
    set<int> friends;
    while(!nw.empty()) {
      auto cw = move(nw.back());
      nw.pop_back();
      auto cwl = cw.length();
      set<int> tt;
      for(int ng : ngrams(cw)) {
        auto gs = ng << 8 | cwl;
        for(auto g = gs - 1; g != gs + 2; ++g) {
          if(!refs.count(g)) {
            continue;
          }
          for(auto i : *refs[g]) {
            if(!friends.count(i)) {
              tt.insert(i);
            }
          }
        }
      }
      for(int wi : tt) {
        if(fw(cw, words[wi])) {
          friends.insert(wi);
          nw.push_back(words[wi]);
        }
      }
    }
    std::cout << (friends.size() + 1) << "\n";
  }
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "recursiveness",
    "elastic",
    "macrographies",
    "END OF INPUT",
    "aa",
    "elastics",
    "elasticss",
    "aahed",
    "aahs",
    "aalii",
    "zymoses",
    "zymosimeters",
  };
  for(auto & t : v_test) {
    add_line(t);
  }
  run();
  for(int i = 0; i < 80; ++i)std::cout<<'-';std::cout<<"\n";
  vector<string> v_expect {
    "1",
    "3",
    "1",
  };
  for(auto & e : v_expect) {
    std::cout << e << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    add_line(line);
  }
  run();
}

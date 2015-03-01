#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

#define VERBOSE
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
  using std::string;
  using std::vector;
  using std::shared_ptr;
  template<typename K,typename V> using map = std::unordered_map<K,V>;
  template<typename K,typename V> using multimap = std::unordered_multimap<K,V>;

  void process(string s);

#ifdef TEST
  void test() {
    vector<string> v_test {
"|deEva|lan t|to ha|evil |ankin|il-ev|o hac| to h|vil p|an to|The e|CodeE| evil|plan |hack |Eval |ack C|l ran|king.|l-evi|evil-|-evil|l pla|il pl| hack|al ra|vil-e|odeEv|he ev|n to |ck Co|eEval|nking| rank| Code|e evi|ranki|k Cod| plan|val r|"
    };
    vector<string> v_expect {
"The evil-evil plan to hack CodeEval ranking."
    };
    for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
      process(v_test[i]);
      std::cout << v_expect[i] << "\n";
    }
  }
#endif //#ifdef TEST

  void process_file(char* path) {
    std::ifstream stream(path);
    for(string line; std::getline(stream, line); ) {
      process(line);
    }
  }

  struct item_t {
    const string s;
    const int left;
    const int right;
    item_t(string &&_s, int lm, int rm) : s(std::move(_s)), left(lm), right(rm) {}
  };

  struct worker {
    int n;
    const string initial;
    map<int,vector<shared_ptr<item_t>>> by_left;
    map<int,vector<shared_ptr<item_t>>> by_right;
    shared_ptr<item_t> last_glued;

    worker(const string& s) : initial(s) {}
  
    void run();

    void add_item(shared_ptr<item_t> &&item);
    void remove_item(shared_ptr<item_t> item);
    void glue(shared_ptr<item_t> left, shared_ptr<item_t> right);
    
    void prepare();
    bool step();
    string result() const;
  };

  void worker::add_item(shared_ptr<item_t> &&item) {
    by_left[item->left].push_back(item);
    by_right[item->right].push_back(item);
  }

  void remove_item_from_map(map<int,vector<shared_ptr<item_t>>> &m, shared_ptr<item_t> item, int key) {
    auto &vec = m[key];
    vec.erase(std::find(vec.begin(), vec.end(), item));
    if(vec.empty()) {
      m.erase(key);
    }
  }

  void worker::remove_item(shared_ptr<item_t> item) {
    remove_item_from_map(by_left, item, item->left);
    remove_item_from_map(by_right, item, item->right);
  }

  void worker::glue(shared_ptr<item_t> left, shared_ptr<item_t> right) {
    auto new_item = std::make_shared<item_t>(left->s + right->s.substr(n), left->left, right->right);
#ifdef VERBOSE
    std::cout << "new_item: {left=" << new_item->left << ", right=" << new_item->right << ", s='" << new_item->s << "'}\n";
#endif//#ifdef VERBOSE
    last_glued = new_item;
    add_item(std::move(new_item));
    remove_item(left);
    remove_item(right);
  }

  int map_marker(map<string,int> &m, string &&s) {
    auto it = m.find(s);
    if(m.end() != it) {
      return it->second;
    }
    int res = m.size();
    m.insert({ std::move(s), res });
    return res;
  }

  void worker::prepare() {
    string t;
    std::istringstream ss { initial };
    map<string,int> markers;
    while(std::getline(ss, t, '|')) {
      if(t.empty()) {
        continue;
      }
      auto tr = t.substr(1);
      auto tl = t.substr(0, n = t.length() - 1);

      auto mr = map_marker(markers, std::move(tr));
      auto ml = map_marker(markers, std::move(tl));
      
      add_item(std::make_shared<item_t>(std::move(t), ml, mr));
    }
  }
  
  bool worker::step() {
    return false;
  }

  string worker::result() const {
    return last_glued ? last_glued->s : "<ERROR>";
  }

  void worker::run() {
    prepare();
    while(step());
#ifdef VERBOSE
    std::cout << "by_left.size() = " << by_left.size() << ", by_right.size() = " << by_right.size() << "\n";
#endif//#ifdef VERBOSE
    std::cout << result() << "\n";
  }
  
  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    worker w { line };
    w.run();
  }
}

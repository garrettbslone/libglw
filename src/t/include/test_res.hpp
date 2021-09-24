//
// Created by Garrett on 9/21/21.
//

#ifndef GLW_TEST_RES_HPP
#define GLW_TEST_RES_HPP

#include <vector>
#include <ostream>

using std::vector;
using std::string;
using std::ostream;

namespace glw {

class test_res {
public:
    void pass() { this->passed++; }
    void fail(const string& err) { this->failed++; errs.push_back(err); }
    void fail(int failed, const string& err) { this->failed += failed; errs.push_back(err); }
    void set_total(int tot) { this->total = tot; }
    void inc_total(int inc) { this->total += inc; }

    int get_passed() const { return this->passed; }
    int get_failed() const { return this->failed; }
    int get_total() const { return this->total; }
    const vector<string>& get_errs() const { return this->errs; }

    friend ostream& operator<<(ostream& os, const test_res& res)
    {
        os << "total: " << res.total << "\npassed: " << res.passed << "\nfailed: " << res.failed << "\nerrs:\n";

        for (const auto& err: res.errs) {
            os << err << "\n";
        }

        return os;
    }

private:
  int passed = 0;
  int failed = 0;
  int total = 0;

  vector<string> errs = vector<string>();
};

}

#endif //GLW_TEST_RES_HPP

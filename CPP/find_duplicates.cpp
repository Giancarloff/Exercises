#include <iostream>
#include <vector>

using namespace std;

vector<int> find_duplicates(vector<int> v) {
    int v_size = v.size();
    vector<int> rv;
    
    // Nobody needs optimization
    int count = 0;
    for (int i : v) {
        for (int x : v) {
            if (i == x) {
                count += 1;
            }
        }
        if (2 <= count) {
            bool found = false;
            for (int p : rv) {
                if (i == p) {
                    found = true;
                }
            }

            if (!found) {
                rv.push_back(i);
            }
        }
        count = 0;
    }

    return rv;
}

int main() {

    vector<int> vec = {1, 2, 3, 4, 5, 1, 1, 2, 5, 7, 8, 9, 10, 10};

    for (int x : vec) {
        cout << x << ", ";
    }
    cout << endl;

    for (int p : find_duplicates(vec)) {
        cout << p << ", ";
    }
    cout << endl;

    return 0;
}
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

template<typename IterSource, typename IterTarget>
void copiar(IterSource start, IterSource stop, IterTarget dest) {
    while (start != stop) {
        *dest++ = *start++;  //UB = Undefined Behavior
//        advance(dest, 1);
//        advance(start, 1);
    }
}

template<typename T, template <typename ...> class Container>
ostream& operator<<(ostream& out, const Container<T>& cnt) {
    for (const auto& item: cnt)
        out << item << " ";
    return out;
}

void ejemplo_1() {
    vector<int> v1 = {10, 20, 30, 40, 50};
    vector<int> v2(size(v1));
    copiar(begin(v1), end(v1), begin(v2));
    cout << v2 << endl;
    vector<int> v3(size(v1));
    copy(begin(v1), end(v1), begin(v3));
    cout << v3 << endl;
}



template<typename IterSource,
        typename BackInserterIterator,
        typename UnaryPredicate>
void copiar_si(IterSource start, IterSource stop, BackInserterIterator dest, UnaryPredicate pred) {
    while (start != stop) {
        if (pred(*start) == true) {
            *dest = *start;  //UB = Undefined Behavior
//            advance(dest, 1);
        }
        advance(start, 1);
    }
}

bool is_odd(int value) {
    return value % 2 == 0;
}

void ejemplo_2 () {
    vector<int> v1 = {1, 2, 3, 4, 5};
    vector<int> v2;
    copiar_si(begin(v1), end(v1), back_inserter(v2), is_odd);
    cout << v2 << endl;
}



int main() {
//    ejemplo_1();
    ejemplo_2();
    return 0;
}

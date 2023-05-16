#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <type_traits>
#include <functional>
using namespace std;

template<typename IterSource,
        typename IterTarget
        >
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
        template <typename ...> class IteratorTarget,
        typename T,
        typename UnaryPredicate>
void copiar_si(IterSource start, IterSource stop, IteratorTarget<T> dest, UnaryPredicate pred) {
    while (start != stop) {
        if (pred(*start) == true) {
            *dest = *start;  //UB = Undefined Behavior
            if constexpr (
                    !is_same<IteratorTarget<T>, back_insert_iterator<T>>::value
            )
               advance(dest, 1);
        }
        advance(start, 1);
    }
}

template<typename IterSource,
        typename IteratorTarget,
        typename UnaryPredicate>
void copiar_si(IterSource start, IterSource stop, IteratorTarget dest, UnaryPredicate pred) {
    while (start != stop) {
        if (pred(*start) == true) {
            *dest = *start;  //UB = Undefined Behavior
            advance(dest, 1);
        }
        advance(start, 1);
    }
}

bool is_even(int value) {
    return value % 2 == 0;
}

int sumar(int a, int b) {
    return a + b;
}

int restar(int a, int b) {
    return a - b;
}

int multiplicar (int a, int b) {
    return a * b;
}

void ejemplo_2 () {
    vector<int> v1 = {1, 2, 3, 4, 5};
    vector<int> v2;
    copiar_si(begin(v1), end(v1), back_inserter(v2), is_even);
    cout << v2 << endl;
    vector<int> v3(3);
    copiar_si(begin(v1), end(v1), begin(v3), is_even);
    cout << v3 << endl;
}

void ejemplo_puntero_funcion() {
    bool (*ptr_is_even)(int) = nullptr;
    int (*ptr_operacion_aritmetica)(int, int) = nullptr;

    ptr_is_even = is_even;
    ptr_operacion_aritmetica = sumar;

    cout << boolalpha << ptr_is_even(3) << endl;
    cout << ptr_operacion_aritmetica(10, 20) << endl;

    ptr_operacion_aritmetica = restar;
    cout << ptr_operacion_aritmetica(10, 20) << endl;
}

//typedef int(*)(int,int) type_fun;
//typedef vector<type_fun> type_container_funs;

using type_fun = int(*)(int,int);
using type_container_funs = vector<type_fun> ;

int procesar_container_funciones(const type_container_funs& cnt,
                                 int a, int b,
                                 const type_fun fn_proc = sumar,
                                 int initial = 0) {
    int resultado = initial;
    for (const auto& fn: cnt)
        resultado = fn_proc(resultado, fn(a, b));
    return resultado;
}

void ejemplo_de_vector_puntero_funciones() {
    type_container_funs vec_funs;
    vec_funs.push_back(sumar);          // (30 + 20) = 50
    vec_funs.push_back(restar);         // (30 - 20) = 10
    vec_funs.push_back(multiplicar);    // (30 * 20) = 600
    auto total = procesar_container_funciones(vec_funs,
                                              30, 20,
                                              multiplicar,
                                              1);
    cout << total << endl; // (30 + 20) * (30 - 20) * (30 * 20) = 660
}

bool is_multiple_2param(int value, int base) {
    return value % base == 0;
}

bool is_multiple_1param(int value) {
    return value % 2 == 0;
}

void ejemplo_de_lambdas() {
    vector<int> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    vector<int> v2;
    copy_if(begin(v1), end(v1),
            back_inserter(v2),
            is_multiple_1param);
    cout << v2 << endl;

    // Llamada L-value
    int base = 3;
    auto is_multiple_lvalue = [&base](int value) {return value % base == 0;};
    vector<int> v3;
    copy_if(begin(v1), end(v1),
            back_inserter(v3),
            is_multiple_lvalue);
    cout << v3 << endl;
    // Llamada R-value
    vector<int> v4;
    base = 5;
    copy_if(begin(v1), end(v1),
            back_inserter(v4),
            [base](int value) {return value % base == 0; });
    cout << v4 << endl;

    vector<int> v5;
    base = 4;
    copy_if(begin(v1), end(v1),
            back_inserter(v5),
            is_multiple_lvalue);
    cout << v5 << endl;

}

void ejemplo_lambda_capture_constante() {
    int x = 10;
    int y = 10;
    cout << [x, y](int a)mutable {
        x = 20;
        return x * a * y;
    }(10) << endl;
    cout << [=](int a)mutable {
        x = 20;
        return x * a * y;
    }(10) << endl;
    cout << x << endl;
}

int procesar_container_lambdas(const vector<function<int(int, int)>>& cnt,
                                 int a, int b,
                                 const function<int(int, int)> callable_proc = sumar,
                                 int initial = 0) {
    int resultado = initial;
    for (const auto& fn: cnt)
        resultado = callable_proc(resultado, fn(a, b));
    return resultado;
}
void ejemplo_de_vector_puntero_lambdas() {
    vector<function<int(int, int)>> vec_callables;
    vec_callables.emplace_back(sumar);          // (30 + 20) = 50
    vec_callables.emplace_back([](int a, int b){ return a - b;});         // (30 - 20) = 10
    auto lambda_multiplicar = [](int a, int b) { return a * b; };
    vec_callables.emplace_back(lambda_multiplicar);    // (30 * 20) = 600
    auto total = procesar_container_lambdas(vec_callables,
                                              30, 20,
                                              lambda_multiplicar,
                                              1);
    cout << total << endl; // (30 + 20) * (30 - 20) * (30 * 20) = 660
}

template <typename T>
struct is_multiple_functor {
    is_multiple_functor(T &base) : base(base) {}
    bool operator()(T value) { return value % base == 0; }
private:
    T& base;
};

void ejemplo_de_funciones_objeto() {
    vector<int> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    vector<int> v2;

    // Llamada L-value
    int base = 3;
    vector<int> v3;
    copy_if(begin(v1), end(v1),
            back_inserter(v3),
            is_multiple_functor<int>(base));
    cout << v3 << endl;
    // Llamada R-value
    vector<int> v5;
    base = 4;
    copy_if(begin(v1), end(v1),
            back_inserter(v5),
            is_multiple_functor<int>(base));
    cout << v5 << endl;
}

template<typename T>
struct menor {
    bool operator()(T a, T b) { return a < b; }
};

template<typename T>
struct mayor {
    bool operator()(T a, T b) { return a > b; }
};

void ejemplo_ordenar() {
    vector<int> v1 = {1, 3, 2, 7, 5, 4, 10, 20, 11};
    sort(begin(v1), end(v1), menor<int>());
    cout << v1 << endl;
    sort(begin(v1), end(v1), mayor<int>());
}

bool is_multiple_3(int value, int base) {
    return value % base == 0;
}

void ejemplo_funcion_parcial() {
    vector<int> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    vector<int> v2;

    using namespace std::placeholders;

    auto is_multiple_fpartial = bind(is_multiple_3, _1, 3);

    // Llamada L-value
    vector<int> v3;
    copy_if(begin(v1), end(v1),
            back_inserter(v3),
            is_multiple_fpartial);
    cout << v3 << endl;
}

int suma_triple(int a, int b, int c) { return a + b + c; }

void ejemplo2_funcion_parcial() {
    using namespace std::placeholders;
    auto sumar_doble = bind(suma_triple, _1, _2, 10);
    cout << sumar_doble(10, 20) << endl;
    int value = 10;
    auto sumar_unico = bind(suma_triple, _1, ref(value), 10);
    cout << sumar_unico(10) << endl;
    value = 20;
    cout << sumar_unico(10) << endl;
}

int main() {
//    ejemplo_1();
//    ejemplo_2();
//    ejemplo_puntero_funcion();
//    ejemplo_de_vector_puntero_funciones();
//    ejemplo_de_lambdas();
//    ejemplo_lambda_capture_constante();
//    ejemplo_ordenar();
//    ejemplo_de_funciones_objeto();
//    ejemplo_funcion_parcial();
    ejemplo2_funcion_parcial();
    return 0;
}

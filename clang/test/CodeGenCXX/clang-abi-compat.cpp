// RUN: %clang_cc1 -std=c++98 -triple x86_64-linux-gnu -fclang-abi-compat=3.0 %s -emit-llvm -o -    | FileCheck --check-prefixes=CHECK,PRE39,PRE5,PRE12 %s
// RUN: %clang_cc1 -std=c++17 -triple x86_64-linux-gnu -fclang-abi-compat=3.0 %s -emit-llvm -o -    | FileCheck --check-prefixes=CHECK,PRE39,PRE5,PRE12 %s
// RUN: %clang_cc1 -std=c++17 -triple x86_64-linux-gnu -fclang-abi-compat=3.8 %s -emit-llvm -o -    | FileCheck --check-prefixes=CHECK,PRE39,PRE5,PRE12 %s
// RUN: %clang_cc1 -std=c++17 -triple x86_64-linux-gnu -fclang-abi-compat=3.9 %s -emit-llvm -o -    | FileCheck --check-prefixes=CHECK,V39,PRE5,PRE12 %s
// RUN: %clang_cc1 -std=c++17 -triple x86_64-linux-gnu -fclang-abi-compat=4.0 %s -emit-llvm -o -    | FileCheck --check-prefixes=CHECK,V39,PRE5,PRE12 %s
// RUN: %clang_cc1 -std=c++17 -triple x86_64-linux-gnu -fclang-abi-compat=5 %s -emit-llvm -o -      | FileCheck --check-prefixes=CHECK,V39,V5,PRE12,PRE12-CXX17 %s
// RUN: %clang_cc1 -std=c++17 -triple x86_64-linux-gnu -fclang-abi-compat=11 %s -emit-llvm -o -     | FileCheck --check-prefixes=CHECK,V39,V5,PRE12,PRE12-CXX17 %s
// RUN: %clang_cc1 -std=c++98 -triple x86_64-linux-gnu -fclang-abi-compat=latest %s -emit-llvm -o - | FileCheck --check-prefixes=CHECK,V39,V5,V12 %s
// RUN: %clang_cc1 -std=c++20 -triple x86_64-linux-gnu -fclang-abi-compat=latest %s -emit-llvm -o - | FileCheck --check-prefixes=CHECK,V39,V5,V12,V12-CXX17 %s

typedef __attribute__((vector_size(8))) long long v1xi64;
void clang39(v1xi64) {}
// PRE39: @_Z7clang39Dv1_x(i64
// V39: @_Z7clang39Dv1_x(double

struct A {
  A(const A&) = default;
  A(A&&);
};
void clang5(A) {}
// PRE5: @_Z6clang51A()
// V5: @_Z6clang51A(%{{.*}}*

namespace mangle_template_prefix {
  // PRE12: @_ZN22mangle_template_prefix1fINS_1TEEEvNT_1UIiE1VIiEENS4_S5_IfEE(
  // V12: @_ZN22mangle_template_prefix1fINS_1TEEEvNT_1UIiE1VIiEENS5_IfEE(
  template<typename T> void f(typename T::template U<int>::template V<int>, typename T::template U<int>::template V<float>);
  struct T { template<typename I> struct U { template<typename J> using V = int; }; };
  void g() { f<T>(1, 2); }
}

int arg;
template<const int *> struct clang12_unchanged {};
// CHECK: @_Z4test17clang12_unchangedIXadL_Z3argEEE
void test(clang12_unchanged<&arg>) {}

#if __cplusplus >= 201703L
// PRE12-CXX17: @_Z4test15clang12_changedIXadL_Z3argEEE
// V12-CXX17: @_Z4test15clang12_changedIXcvPKiadL_Z3argEEE
template<auto> struct clang12_changed {};
void test(clang12_changed<(const int*)&arg>) {}
#endif

// PRE12: @_Z9clang12_aIXadL_Z3argEEEvv
// V12: @_Z9clang12_aIXcvPKiadL_Z3argEEEvv
template<const int *> void clang12_a() {}
template void clang12_a<&arg>();

// PRE12: @_Z9clang12_bIXadL_Z3arrEEEvv
// V12: @_Z9clang12_bIXadsoKcL_Z3arrEEEEvv
extern const char arr[6] = "hello";
template<const char *> void clang12_b() {}
template void clang12_b<arr>();

// CHECK: @_Z9clang12_cIXadL_Z3arrEEEvv
template<const char (*)[6]> void clang12_c() {}
template void clang12_c<&arr>();

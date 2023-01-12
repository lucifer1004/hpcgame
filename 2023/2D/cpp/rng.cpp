#include <algorithm>
#include <cstdio>
#include <iomanip>

#define data_t unsigned long long
data_t *a;
data_t gen_A, gen_B, gen_C, n;

/*
    data数组的生成的示意代码如下：
    */
data_t gen_next() {
  gen_A ^= gen_A << 31;
  gen_A ^= gen_A >> 17;
  gen_B ^= gen_B << 13;
  gen_B ^= gen_B >> 5;
  gen_C++;
  gen_A ^= gen_B;
  gen_B ^= gen_C;
  return gen_A;
}
void data_init() {
  scanf("%llu%llu%llu%llu", &gen_A, &gen_B, &gen_C, &n);
  a = (data_t *)malloc(sizeof(data_t) * n);
  for (data_t i = 0; i < n; i++) {
    a[i] = gen_next();
  }
}
/*
归一数的计算方式如下：
*/
data_t get_res(data_t *a, data_t n) {
  std::sort(a, a + n);
  data_t res = 0;
  for (data_t i = 0; i < n; i++) {
    res ^= i * a[i];
  }
  return res;
}
#undef data_t

int main() {
  data_init();
  printf("%llu", get_res(a, n));
  return 0;
}

// 请在程序开头引用此头文件
#include "minesweeper_helpers.h"
#include "pthread.h"
#include <atomic>
#include <bitset>
#include <iostream>
#include <vector>

constexpr int MAXN = 65536;

#ifdef LOCAL
constexpr int THREADS = 4;
constexpr int RR = 2;
constexpr int CC = 2;
#else
constexpr int THREADS = 8;
constexpr int RR = 2;
constexpr int CC = 4;
#endif

// N: 地图的边长
// K: 地图中的雷的数量
long N, K;
int constant_A;
std::bitset<MAXN> mine[MAXN], safe[MAXN], clicked[MAXN];
std::atomic<long long> hit, skip;

std::vector<std::pair<int, int>> get_neighbors(int x, int y) {
  std::vector<std::pair<int, int>> ret;
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      if ((i || j) && (x + i >= 0) && (x + i < N) && (y + j >= 0) &&
          (y + j < N)) {
        ret.push_back({x + i, y + j});
      }
    }
  }
  return ret;
}

void dfs(Channel &channel, int x, int y, int u, int d, int l, int r) {
  ClickResult result = channel.click(x, y, true);
  clicked[x][y] = true;

  if (result.is_skipped) {
  } else if (result.is_mine) {
    hit++;
  } else {
    for (int k = 0; k < result.open_grid_count; ++k) {
      unsigned short x = (*result.open_grid_pos)[k][0];
      unsigned short y = (*result.open_grid_pos)[k][1];
      safe[x][y] = true;
    }

    for (int k = 0; k < result.open_grid_count; ++k) {
      unsigned short x = (*result.open_grid_pos)[k][0];
      unsigned short y = (*result.open_grid_pos)[k][1];
      unsigned short v = (*result.open_grid_pos)[k][2];
      safe[x][y] = true;

      if (v) {
        int nei = 0, s = 0, m = 0;
        for (int i = -1; i <= 1; ++i) {
          for (int j = -1; j <= 1; ++j) {
            if ((i || j) && (x + i >= 0) && (x + i < N) && (y + j >= 0) &&
                (y + j < N)) {
              nei++;
              if (safe[x + i][y + j]) {
                s++;
              }
              if (mine[x + i][y + j]) {
                m++;
              }
            }
          }
        }

        // Can determine mines
        if (s + v == nei) {
          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              if ((i || j) && (x + i >= 0) && (x + i < N) && (y + j >= 0) &&
                  (y + j < N)) {
                if (!safe[x + i][y + j])
                  mine[x + i][y + j] = true;
              }
            }
          }
        }

        // Can determine safe cells
        if (v == m) {
          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              if ((i || j) && (x + i >= 0) && (x + i < N) && (y + j >= 0) &&
                  (y + j < N)) {
                if (!mine[x + i][y + j]) {
                  safe[x + i][y + j] = true;
                  if (!clicked[x + i][y + j] && (x + i >= u) && (x + i <= d) &&
                      (y + j >= l) && (y + j <= r)) {
                    dfs(channel, x + i, y + j, u, d, l, r);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

static void *thread_body(void *arg) {
  int id = (unsigned long long)arg;
  int i = (id / CC) * (N / RR), j = (id % CC) * (N / CC);
  Channel channel = create_channel();

  for (long r = 0; r < N / RR; ++r) {
    for (long c = 0; c < N / CC; ++c) {
      if (mine[i + r][j + c]) {
        skip++;
        continue;
      }

      dfs(channel, i + r, j + c, i, c, j, r);
    }
  }

  pthread_exit(NULL);
}

pthread_t threads[THREADS];

int main() {
  // 请在程序开始时调用此函数来初始化 & 获得 N 和 K 的值
  minesweeper_init(N, K, constant_A);
  hit = 0;

  for (int i = 0; i < THREADS; ++i) {
    pthread_create(threads + i, NULL, thread_body,
                   (void *)(unsigned long long)i);
  }

  // 写代码吧！加油！
  for (int i = 0; i < THREADS; ++i) {
    pthread_join(threads[i], NULL);
  }

  std::cout << "Hit " << hit << " Times" << std::endl;
  std::cout << "Skip " << skip << " Times" << std::endl;

  return 0;
}

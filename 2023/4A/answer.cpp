// 请在程序开头引用此头文件
#include "minesweeper_helpers.h"
#include "pthread.h"
#include <atomic>
#include <bitset>
#include <cassert>
#include <iostream>
#include <random>
#include <unordered_set>
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
static long N, K, REP;
int constant_A;
std::bitset<MAXN> mine[MAXN], safe[MAXN], clicked[MAXN];
std::atomic<unsigned int> hit, skip;
std::vector<int8_t> score[MAXN], nei_safe[MAXN], nei_mine[MAXN],
    nei_count[MAXN];
std::vector<std::vector<std::pair<unsigned short, unsigned short>>> neis[MAXN];

std::vector<std::pair<unsigned short, unsigned short>> get_neighbors(int x,
                                                                     int y) {
  std::vector<std::pair<unsigned short, unsigned short>> ret;
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
#ifdef LOCAL
    hit++;
#endif
    mine[x][y] = true;
    for (auto [nx, ny] : neis[x][y]) {
      nei_mine[nx][ny]++;
    }
  } else {
    std::unordered_set<long> margin, nxt;

    for (int k = 0; k < result.open_grid_count; ++k) {
      unsigned short x = (*result.open_grid_pos)[k][0];
      unsigned short y = (*result.open_grid_pos)[k][1];
      unsigned short v = (*result.open_grid_pos)[k][2];
      if (!safe[x][y]) {
        safe[x][y] = clicked[x][y] = true;
        for (auto [nx, ny] : neis[x][y]) {
          nei_safe[nx][ny]++;
          if (!safe[nx][ny] && !mine[nx][ny] && score[nx][ny] >= 0)
            margin.insert(N * nx + ny);
        }
      }

      score[x][y] = v;
      if (v) {
        margin.insert(N * x + y);
      }
    }

    for (int rep = 0; rep < REP; rep++) {
      nxt.clear();
      for (auto xy : margin) {
        int x = xy / N, y = xy % N;
        assert(score[x][y] >= 0);

        // Can determine mines
        if (nei_safe[x][y] + score[x][y] == nei_count[x][y]) {
          for (auto [nx, ny] : neis[x][y]) {
            if (!safe[nx][ny] && !mine[nx][ny]) {
              mine[nx][ny] = true;
              for (auto [nnx, nny] : neis[nx][ny]) {
                nei_mine[nnx][nny]++;
                if (score[nnx][nny] >= 0)
                  nxt.insert(N * nnx + nny);
              }
            }
          }
        }

        // Can determine safe cells
        if (score[x][y] == nei_mine[x][y]) {
          for (auto [nx, ny] : neis[x][y]) {
            if (!safe[nx][ny] && !mine[nx][ny]) {
              safe[nx][ny] = true;
              for (auto [nnx, nny] : neis[nx][ny]) {
                nei_safe[nnx][nny]++;
                if (score[nnx][nny] >= 0)
                  nxt.insert(N * nnx + nny);
              }
            }
          }
        }
      }
    }

    std::swap(nxt, margin);
  }
}

void dfs_original(Channel &channel, int x, int y, int u, int d, int l, int r) {
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
                  // if (!clicked[x + i][y + j]) {
                  //   dfs(channel, x + i, y + j, u, d, l, r);
                  // }
                }
              }
            }
          }
        }
      }
    }
  }
}

static void *thread_original(void *arg) {
  int id = (unsigned long long)arg;
  int i = (id / CC) * (N / RR), j = (id % CC) * (N / CC);
  Channel channel = create_channel();
  std::mt19937 mt(time(nullptr));

  for (long t = 0; t < N / 16 * N / 8; ++t) {
    int r = mt() % (N / RR), c = mt() % (N / CC);
    if (mine[i + r][j + c]) {
      skip++;
      continue;
    }
    dfs_original(channel, i + r, j + c, i, i + r, j, j + c);
  }

  for (long r = 0; r < N / RR; ++r) {
    for (long c = 0; c < N / CC; ++c) {
      if (mine[i + r][j + c]) {
        skip++;
        continue;
      }
      dfs_original(channel, i + r, j + c, i, i + r, j, j + c);
    }
  }

  pthread_exit(NULL);
}

static void *thread_naive(void *arg) {
  int id = (unsigned long long)arg;
  int i = (id / CC) * (N / RR), j = (id % CC) * (N / CC);
  Channel channel = create_channel();

  for (long r = 0; r < N / RR; ++r) {
    for (long c = 0; c < N / CC; ++c) {
      channel.click(i + r, j + c, true);
    }
  }

  pthread_exit(NULL);
}

static void *thread_init(void *arg) {
  int id = (unsigned long long)arg;
  for (int i = id; i < N; i += THREADS) {
    score[i].assign(N, -1);
    nei_safe[i].assign(N, 0);
    nei_mine[i].assign(N, 0);
    nei_count[i].assign(N, 0);
    neis[i].assign(N, {});
    for (int j = 0; j < N; ++j) {
      neis[i][j] = get_neighbors(i, j);
      nei_count[i][j] = neis[i][j].size();
    }
  }

  pthread_exit(NULL);
}

static void *thread_body(void *arg) {
  int id = (unsigned long long)arg;

  int i = (id / CC) * (N / RR), j = (id % CC) * (N / CC);
  Channel channel = create_channel();
  std::mt19937 mt(time(nullptr));

  for (long t = 0; t < N / RR / 4 * N / CC / 4; ++t) {
    int r = mt() % (N / RR), c = mt() % (N / CC);
    if (mine[i + r][j + c] || clicked[i + r][j + c]) {
#ifdef LOCAL
      skip++;
#endif
      continue;
    }
    dfs(channel, i + r, j + c, i, i + r, j, j + c);
  }

  for (long r = 0; r < N / RR; ++r) {
    for (long c = 0; c < N / CC; ++c) {
      if (mine[i + r][j + c] || clicked[i + r][j + c]) {
#ifdef LOCAL
        skip++;
#endif
        continue;
      }
      dfs(channel, i + r, j + c, i, i + r, j, j + c);
    }
  }

  pthread_exit(NULL);
}

pthread_t threads[THREADS];

int main() {
  // 请在程序开始时调用此函数来初始化 & 获得 N 和 K 的值
  minesweeper_init(N, K, constant_A);
  if (constant_A == 0) {
    for (int i = 0; i < THREADS; ++i) {
      pthread_create(threads + i, NULL, thread_naive,
                     (void *)(unsigned long long)i);
    }

    for (int i = 0; i < THREADS; ++i) {
      pthread_join(threads[i], NULL);
    }

    return 0;
  } else if (N >= 16384) {
    for (int i = 0; i < THREADS; ++i) {
      pthread_create(threads + i, NULL, thread_original,
                     (void *)(unsigned long long)i);
    }

    for (int i = 0; i < THREADS; ++i) {
      pthread_join(threads[i], NULL);
    }

    return 0;
  }

  REP = 10;

  for (int i = 0; i < THREADS; ++i) {
    pthread_create(threads + i, NULL, thread_init,
                   (void *)(unsigned long long)i);
  }

  for (int i = 0; i < THREADS; ++i) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < THREADS; ++i) {
    pthread_create(threads + i, NULL, thread_body,
                   (void *)(unsigned long long)i);
  }

  for (int i = 0; i < THREADS; ++i) {
    pthread_join(threads[i], NULL);
  }

#ifdef LOCAL
  std::cout << "Hit " << hit << " Times" << std::endl;
  std::cout << "Skip " << skip << " Times" << std::endl;
#endif

  return 0;
}

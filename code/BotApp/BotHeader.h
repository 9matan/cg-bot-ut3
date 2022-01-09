#if defined(__GNUC__) || defined(__GNUG__)
#	pragma GCC optimize("O3")
#	pragma GCC optimize("inline")
#	pragma GCC optimize("omit-frame-pointer")
#	pragma GCC optimize("unroll-loops")
#	pragma GCC optimize("Ofast")
/*
#pragma GCC optimize("thread-jumps")
#pragma GCC optimize("align-functions")
#pragma GCC optimize("align-jumps")
#pragma GCC optimize("align-loops")
#pragma GCC optimize("align-labels")
#pragma GCC optimize("caller-saves")
#pragma GCC optimize("crossjumping")
#pragma GCC optimize("cse-follow-jumps")
#pragma GCC optimize("cse-skip-blocks")
#pragma GCC optimize("delete-null-pointer-checks")
#pragma GCC optimize("expensive-optimizations")
#pragma GCC optimize("gcse")
#pragma GCC optimize("gcse-lm")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("indirect-inlining")
#pragma GCC optimize("ipa-sra")
#pragma GCC optimize("optimize-sibling-calls")
#pragma GCC optimize("peephole2")
#pragma GCC optimize("regmove")
#pragma GCC optimize("reorder-blocks")
#pragma GCC optimize("reorder-functions")
#pragma GCC optimize("rerun-cse-after-loop")
#pragma GCC optimize("sched-interblock")
#pragma GCC optimize("sched-spec")
#pragma GCC optimize("schedule-insns")
#pragma GCC optimize("schedule-insns2")
#pragma GCC optimize("strict-aliasing")
#pragma GCC optimize("strict-overflow")
#pragma GCC optimize("tree-switch-conversion")
#pragma GCC optimize("tree-pre")
#pragma GCC optimize("tree-vrp")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("unswitch-loops")
#pragma GCC optimize("predictive-commoning")
#pragma GCC optimize("gcse-after-reload")
#pragma GCC optimize("tree-vectorize")
#pragma GCC optimize("ipa-cp-clone")
*/
#endif // defined(__GNUC__) || defined(__GNUG__)

// #define RELEASE_BOT (1) // [Enable]

#if RELEASE_BOT

#   ifndef NDEBUG
#   define NDEBUG
#   endif

#   ifdef _DEBUG
#   undef _DEBUG
#   endif

#   ifdef _ITERATOR_DEBUG_LEVEL
#   undef _ITERATOR_DEBUG_LEVEL
#   endif
#   define _ITERATOR_DEBUG_LEVEL 0

#   ifdef _GLIBCXX_INLINE_VERSION
#   undef _GLIBCXX_INLINE_VERSION
#   endif
#   define _GLIBCXX_INLINE_VERSION 1

#else // RELEASE_BOT

#   ifdef MIMAX_MCTS_DEBUG
#   undef MIMAX_MCTS_DEBUG
#   endif
#   define MIMAX_MCTS_DEBUG (1)

#endif // RELEASE_BOT

#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
#include <functional>
#include <future>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC optimize("O3")
#pragma GCC optimize("inline")
#pragma GCC optimize("omit-frame-pointer")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("Ofast")
#endif 

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

namespace mimax {namespace common {unsigned int GenerateRandomSeed();unsigned int UpdateRandomSeed(unsigned int seed = 0);unsigned int GetRandomUInt32(unsigned int const minVal, unsigned int const maxVal);template<typename TIter>inline TIter GetRandomItem(TIter first, TIter last){size_t const rangeSize = static_cast<size_t>(last - first);return first + GetRandomUInt32(0, static_cast<unsigned int>(rangeSize));}template<typename TEnum>inline TEnum GetRandomEnumValue(){return static_cast<TEnum>(GetRandomUInt32(0, static_cast<unsigned int>(TEnum::Count)));}template<typename TIter>inline void RandomShuffle(TIter first, TIter last){if (first == last) return;TIter current = last;--current;for (; current != first; --current){std::swap(*current, *(first + GetRandomUInt32(0, (unsigned int)(current - first + 1))));}}}}
namespace mimax {namespace common {class CTimeProfiler{public:CTimeProfiler(char const* const name);~CTimeProfiler();private:std::string m_name;decltype(std::chrono::high_resolution_clock::now()) m_startTime;};}}
#ifndef DISABLE_PROFILER
#define PROFILE_TIME(name) mimax::common::CTimeProfiler timeProfiler(name);
#else
#define PROFILE_TIME(name)
#endif 

#include <assert.h>
namespace mimax {namespace common {template<typename TComponent>struct SVec2Base{static const size_t ms_vecSize = 2;using ValueType = TComponent;template<typename TOtherComponent>explicit SVec2Base(SVec2Base<TOtherComponent> const other): m_components{ static_cast<TComponent>(other[0]), static_cast<TComponent>(other[1]) }{}template<typename TOtherComponent>SVec2Base(TOtherComponent const x, TOtherComponent const y): m_components{ static_cast<TComponent>(x), static_cast<TComponent>(y) }{}template<typename TOtherComponent>SVec2Base(std::initializer_list<TOtherComponent> const& data){assert(data.size() <= ms_vecSize);std::transform(data.begin(), data.end(), m_data,[](TOtherComponent const otherComp){return static_cast<TComponent>(otherComp);});}SVec2Base() : m_components{0, 0} {}SVec2Base(TComponent const x, TComponent const y) : m_components{x, y} {}SVec2Base(std::initializer_list<TComponent> const& data){assert(data.size() <= ms_vecSize);std::copy(data.begin(), data.end(), m_data);}inline SVec2Base& operator=(SVec2Base const other) { m_data[0] = other.m_data[0]; m_data[1] = other.m_data[1]; return *this; }inline bool operator==(SVec2Base const other) const { return other[0] == m_data[0] && other[1] == m_data[1]; }inline bool operator!=(SVec2Base const other) const { return !(*this == other); }inline TComponent operator[](size_t const index) const { return m_data[index]; }inline TComponent& operator[](size_t const index) { return m_data[index]; }inline SVec2Base operator+(SVec2Base const other) const { return SVec2Base(m_data[0] + other.m_data[0], m_data[1] + other.m_data[1]); }inline SVec2Base operator-(SVec2Base const other) const { return SVec2Base(m_data[0] - other.m_data[0], m_data[1] - other.m_data[1]); }inline bool operator<(SVec2Base const other) const { if (m_data[0] == other[0]) { return m_data[1] < other[1]; } return m_data[0] < other[0]; }union{struct{TComponent m_x;TComponent m_y;} m_components;TComponent m_data[ms_vecSize];};};using SVec2i = SVec2Base<int>;using SVec2u = SVec2Base<unsigned int>;using SVec2si = SVec2Base<short>;using SVec2su = SVec2Base<unsigned short>;using SVec2char = mimax::common::SVec2Base<char>;}}using SVec2 = mimax::common::SVec2char;
namespace ut3 {namespace game {
#   define UT3_FIRST_BITS_MASK(bitsCount)  ((~(1ULL)) >> (64 - bitsCount))

#   define UT3_SET_BIT(number, bitPos, bitVal) number = ((number & (~(1ULL << (bitPos)))) | ((unsigned long long)((bitVal) == 0 ? 0 : 1) << (bitPos)))
#   define UT3_GET_BIT(number, bitPos) (((number) >> (bitPos)) & 1)

#   define UT3_SET_BITS(number, bitStartPos, bitsCount, bitsVal) number = ((number & (~(UT3_FIRST_BITS_MASK((unsigned long long)(bitsCount)) << (bitStartPos)))) | (((unsigned long long)(bitsVal)) << (bitStartPos)))
#   define UT3_GET_BITS(number, bitStartPos, bitsCount) (((number) >> (bitStartPos)) & UT3_FIRST_BITS_MASK((unsigned long long)(bitsCount)))

#   define UT3_COUNT_BITS_32(number) (std::bitset<32>(number).count())
#   define UT3_COUNT_BITS_64(number) (std::bitset<64>(number).count())

#   define GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(playerIndex) (1ULL << (playerIndex))
#   define GAME_STATE_PLAYER_ELEMENT_TO_PLAYER_INDEX(playerElement) ((playerElement) >> 1)

#   define GAME_STATE_ELEMENT_EMPTY 0
#   define GAME_STATE_ELEMENT_X 1
#   define GAME_STATE_ELEMENT_O 2

#   define GAME_STATE_ELEMENT_DRAW 3

#   define GAME_STATE_SET_PLAYER(gameState, playerIndex) UT3_SET_BIT(gameState.m_data[2], 63, (playerIndex))
#   define GAME_STATE_GET_PLAYER(gameState) UT3_GET_BIT((gameState).m_data[2], 63)
#   define GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) (UT3_GET_BIT((gameState).m_data[2], 36 + (2 * blockIndex)) | (UT3_GET_BIT((gameState).m_data[2], 36 + 1 + (2 * blockIndex)) << 1))
#   define GAME_STATE_SET_BLOCK_STATUS(gameState, blockIndex, value) do \
{ \
UT3_SET_BIT(gameState.m_data[2], 36 + (2 * blockIndex), value & 1); \
UT3_SET_BIT(gameState.m_data[2], 36 + 1 + (2 * blockIndex), value & 2); \
} while(false)
#   define GAME_STATE_SET_LAST_OPP_TURN_X(gameState, value) UT3_SET_BITS(gameState.m_data[2], 54, 4, (value))
#   define GAME_STATE_SET_LAST_OPP_TURN_Y(gameState, value) UT3_SET_BITS(gameState.m_data[2], 58, 4, (value))
#   define GAME_STATE_GET_LAST_OPP_TURN_X(gameState) UT3_GET_BITS((gameState).m_data[2], 54, 4)
#   define GAME_STATE_GET_LAST_OPP_TURN_Y(gameState) UT3_GET_BITS((gameState).m_data[2], 58, 4)
#   define GAME_STATE_INVALIDATE_LAST_OPP_TURN(gameState) UT3_SET_BITS((gameState).m_data[2], 54, 4, 15)
#   define GAME_STATE_IS_VALID_LAST_OPP_TURN(gameState) (UT3_GET_BITS((gameState).m_data[2], 54, 4) != 15)

#   define GAME_STATE_SET_GAME_WINNER(gameState, value) UT3_SET_BITS(gameState.m_data[1], 62, 2, (value))
#   define GAME_STATE_GET_GAME_WINNER(gameState) UT3_GET_BITS((gameState).m_data[1], 62, 2)

#   define GAME_BLOCK_STATE_POS_TO_INDEX(posX, posY) ((posX) + (posY) * 3)
#   define GAME_BLOCK_STATE_SET_ELEMENT(blockState, index, value) do \
{ \
UT3_SET_BIT(blockState.m_data, 2 * index, (value & GAME_STATE_ELEMENT_X)); \
UT3_SET_BIT(blockState.m_data, 2 * index + 1, (value & GAME_STATE_ELEMENT_O)); \
} while(false)
#   define GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, posX, posY, value) GAME_BLOCK_STATE_SET_ELEMENT(blockState, GAME_BLOCK_STATE_POS_TO_INDEX(posX, posY), value)
#   define GAME_BLOCK_STATE_GET_ELEMENT(blockState, index) (((blockState).m_data >> (2 * index)) & 3ULL)
#   define GAME_BLOCK_STATE_GET_ELEMENT_BY_POS(blockState, posX, posY) GAME_BLOCK_STATE_GET_ELEMENT(blockState, GAME_BLOCK_STATE_POS_TO_INDEX(posX, posY))

#   define GAME_STATE_GET_GLOBAL_BLOCK(gameState) ut3::game::SGameBlockState{ ((gameState).m_data[2] >> 36) & UT3_FIRST_BITS_MASK(18) }
constexpr unsigned int BLOCK_PLAYER_ELEMENTS_MASKS[2] = {87381,174762};
#   define GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(blockState, playerIndex) ((int)UT3_COUNT_BITS_32((blockState).m_data & ut3::game::BLOCK_PLAYER_ELEMENTS_MASKS[(playerIndex)]) - (int)GAME_STATE_BLOCK_COUNT_DRAW_ELEMENTS(blockState))
#   define GAME_STATE_BLOCK_COUNT_DRAW_ELEMENTS(blockState) UT3_COUNT_BITS_32(((blockState).m_data) & (((blockState).m_data & ut3::game::BLOCK_PLAYER_ELEMENTS_MASKS[1]) >> 1))
#   define GAME_STATE_ELEMENTS_COUNT(gameState) (UT3_COUNT_BITS_64((gameState).m_data[0]) + UT3_COUNT_BITS_64((gameState).m_data[1] & UT3_FIRST_BITS_MASK(62)) + UT3_COUNT_BITS_64((gameState).m_data[2] & UT3_FIRST_BITS_MASK(36)))
struct SGameState{SGameState(){m_data[0] = m_data[1] = m_data[2] = 0;GAME_STATE_INVALIDATE_LAST_OPP_TURN(*this);}unsigned long long m_data[3];};static_assert(sizeof(SGameState) == 8 * 3, "SGameState has invalid size");struct SGameBlockState{unsigned int m_data = 0;inline bool operator==(SGameBlockState other) const { return m_data == other.m_data; }};static_assert(sizeof(SGameBlockState) == 4, "SGameBlockState has invalid size");inline SGameBlockState GetBlockState(SGameState const gameState, size_t const blockIndex){assert(blockIndex < 9);if (blockIndex < 3){return SGameBlockState{ UT3_GET_BITS(gameState.m_data[0], blockIndex * 18, 18) };}if (blockIndex >= 4){if (blockIndex < 7){return SGameBlockState{ UT3_GET_BITS(gameState.m_data[1], (blockIndex - 4) * 18 + 8, 18) };}else{return SGameBlockState{ UT3_GET_BITS(gameState.m_data[2], (blockIndex - 7) * 18, 18) };}}return SGameBlockState{ (gameState.m_data[0] >> 54) | ((gameState.m_data[1] & UT3_FIRST_BITS_MASK(8)) << 10) };}inline void SetBlockState(SGameState& gameState, size_t const blockIndex, SGameBlockState const blockState){assert(blockIndex < 9);if (blockIndex < 3){UT3_SET_BITS(gameState.m_data[0], blockIndex * 18, 18, blockState.m_data);}else if (blockIndex >= 4){if (blockIndex < 7){UT3_SET_BITS(gameState.m_data[1], (blockIndex - 4) * 18 + 8, 18, blockState.m_data);}else{UT3_SET_BITS(gameState.m_data[2], (blockIndex - 7) * 18, 18, blockState.m_data);}}else{unsigned long long const firstBits = (blockState.m_data & UT3_FIRST_BITS_MASK(10));unsigned long long const secondBits = UT3_GET_BITS(blockState.m_data, 10, 8);UT3_SET_BITS(gameState.m_data[0], 54, 10, firstBits);UT3_SET_BITS(gameState.m_data[1], 0, 8, secondBits);}}}}
namespace ut3{struct SInputData{int m_oppTurnX;int m_oppTurnY;};};
namespace ut3{struct SOutputData{int m_turnX;int m_turnY;};};
namespace ut3 {namespace bot {class IBot{public:virtual ~IBot() {}virtual SOutputData FirstUpdate(SInputData initData) = 0;virtual SOutputData Update(SInputData turnData) = 0;virtual void SetDebugIsEnabled(bool const enableDebug) = 0;virtual char const* GetBotName() const = 0;virtual void Reset() = 0;};}}
namespace ut3 {namespace bot {class CBotBase : public IBot{public:SOutputData FirstUpdate(SInputData initData) override;SOutputData Update(SInputData turnData) override;void SetDebugIsEnabled(bool const enableDebug) override { m_isDebugEnabled = enableDebug; }void Reset();protected:virtual SVec2 FindTurn(game::SGameState const& gameState) = 0;protected:game::SGameState m_gameState;int m_myPlayer = -1;bool m_isDebugEnabled = false;};}}
namespace mimax {namespace mt {class ITask;class CTasksRunner{public:void RunTasksAndWait(std::vector<ITask*> const& tasks, std::chrono::microseconds const waitingTime);private:std::vector<ITask*> m_tasks;std::vector<std::future<void>> m_futures;private:void RunTasks();void Wait(std::chrono::microseconds const time);void StopTasks();void WaitForTasksCompleted();};} } 
namespace ut3 {namespace bot {class CMinimaxBot_v1 : public CBotBase{public:static SVec2 FindTurn(game::SGameState const& gameState, int const myPlayer, bool const debugEnabled = false);public:char const* GetBotName() const override { return "CMinimaxBot_v1"; }protected:SVec2 FindTurn(game::SGameState const& gameState) override;};}}
void ReadData(ut3::SInputData& inputData){std::cin >> inputData.m_oppTurnY >> inputData.m_oppTurnX;int validActionCount;scanf("%i", &validActionCount);for (int i = 0; i < validActionCount; ++i){int x, y;scanf("%i%i", &x, &y);}}void WriteData(ut3::SOutputData const& outputData){std::cout << outputData.m_turnY << " " << outputData.m_turnX << "\n";}int main(){int const randomSeed = mimax::common::UpdateRandomSeed();std::cerr << "Random seed: " << randomSeed << "\n";std::cerr << "Hardware concurrency: " << std::thread::hardware_concurrency() << "\n";ut3::SInputData inData;ReadData(inData);ut3::bot::CMinimaxBot_v1 bot;bot.SetDebugIsEnabled(true);WriteData(bot.FirstUpdate(inData));while (true){ReadData(inData);{PROFILE_TIME("Update");WriteData(bot.Update(inData));}}return 0;}

#define _CRT_SECURE_NO_WARNINGS
namespace mimax {namespace common {template <typename TVec>inline int GetDistanceL1(TVec const lhs, TVec const rhs) { return abs(static_cast<int>(lhs[0] - rhs[0])) + abs(static_cast<int>(lhs[1] - rhs[1])); }char const* ToString(SVec2i const vec2);char const* ToString(SVec2 const vec2);std::ostream& operator<<(std::ostream& out, SVec2char const& vec);std::ostream& operator<<(std::ostream& out, SVec2char const& vec);}}

#define MIMAX_MINIMAX_DEBUG (1)

#if MIMAX_MINIMAX_DEBUG
namespace mimax {namespace dma {struct SMinimaxDebugInfo{static constexpr size_t MAX_DEPTH = 64;size_t m_evaluatedNodesCnt;size_t m_visitedNodesCnt[MAX_DEPTH + 1];size_t m_totalVisitedNodesCnt;size_t m_prunedNodesCnt[MAX_DEPTH + 1];size_t m_totalPrunedNodesCnt;size_t m_maxDepth;SMinimaxDebugInfo(){Reset();}inline void EvaluateNode(){++m_evaluatedNodesCnt;}inline void VisitNode(size_t const depth){++m_totalVisitedNodesCnt;m_maxDepth = (m_maxDepth < depth) ? depth : m_maxDepth;if (depth < MAX_DEPTH)++m_visitedNodesCnt[depth];}inline void PruneNodes(size_t const nodesCnt, size_t const depth){m_totalPrunedNodesCnt += nodesCnt;if (depth < MAX_DEPTH)m_prunedNodesCnt[depth] += nodesCnt;}inline void Reset(){m_evaluatedNodesCnt = 0;m_totalVisitedNodesCnt = 0;m_totalPrunedNodesCnt = 0;m_maxDepth = 0;memset(m_visitedNodesCnt, 0, sizeof(m_visitedNodesCnt));memset(m_prunedNodesCnt, 0, sizeof(m_prunedNodesCnt));}};std::ostream& operator<<(std::ostream& o, SMinimaxDebugInfo const& debugInfo);} } 
#endif 

namespace mimax {namespace dma {template<typename TState, typename TMove, typename TMovesContainer, typename TResolver>class CMinimaxBase{public:using State = TState;using Move = TMove;public:struct SConfig{float m_minValue = -1.0f;float m_maxValue = 1.0f;float m_epsilon = std::numeric_limits<float>::epsilon();size_t m_maxDepth = 0;};public:CMinimaxBase(TResolver const& resolver, SConfig const& config): m_resolver(resolver), m_config(config), m_isStopRequested(false){}inline std::optional<TMove> FindSolution(TState const& state){
#if MIMAX_MINIMAX_DEBUG
m_debugInfo.Reset();
#endif 
auto const visitingResult = VisitState(state, 0, m_config.m_minValue, m_config.m_maxValue);auto const move = m_isStopRequested? std::optional<TMove>(): visitingResult.m_move;m_isStopRequested = false;return move;}inline void StopAlgorithm() { m_isStopRequested = true; }
#if MIMAX_MINIMAX_DEBUG
inline SMinimaxDebugInfo const& GetDebugInfo() const { return m_debugInfo; }
#endif 
private:struct STraversalResult{float m_score = 0.0f;TMove m_move;};private:STraversalResult VisitState(TState const& state, size_t const depth, float alpha, float beta){
#if MIMAX_MINIMAX_DEBUG
m_debugInfo.VisitNode(depth);
#endif 
TMovesContainer moves;if(depth != m_config.m_maxDepth){m_resolver.GetPossibleMoves(moves, state);}STraversalResult result;if(moves.empty()){
#if MIMAX_MINIMAX_DEBUG
m_debugInfo.EvaluateNode();
#endif 
int const colorMultiplier = (depth & 1) == 0 ? 1 : -1;result.m_score = m_resolver.EvaluateState(state) * colorMultiplier;return result;}result.m_score = -std::numeric_limits<float>::max();for (size_t i = 0; i < moves.size(); ++i){if (m_isStopRequested) return result;auto const move = moves[i];TState childState = state;m_resolver.MakeMove(childState, move);auto const childResult = VisitState(childState, depth + 1, -beta, -alpha);if (-childResult.m_score > result.m_score){result.m_move = move;result.m_score = -childResult.m_score;alpha = (result.m_score > alpha) ? result.m_score : alpha;if (alpha + m_config.m_epsilon >= beta){
#if MIMAX_MINIMAX_DEBUG
m_debugInfo.PruneNodes(moves.size() - (i + 1), depth + 1);
#endif 
break;}}}return result;}private:TResolver m_resolver;SConfig m_config;
#if MIMAX_MINIMAX_DEBUG
SMinimaxDebugInfo m_debugInfo;
#endif 
bool m_isStopRequested;};} } 
namespace mimax {namespace mt {class ITask{public:virtual ~ITask() = default;virtual void RunTask() = 0;virtual void StopTask() = 0;};} } 
namespace mimax {namespace dma {template<typename TMinimax>class CMinimaxTask : public mimax::mt::ITask{public:using State = typename TMinimax::State;using Move = typename TMinimax::Move;public:CMinimaxTask(TMinimax* minimax, State const& state): m_minimax(minimax), m_state(state){}void RunTask() override{m_result = m_minimax->FindSolution(m_state);}void StopTask() override{m_minimax->StopAlgorithm();}inline std::optional<Move> const& GetResult() const { return m_result; }private:State m_state;TMinimax* m_minimax;std::optional<Move> m_result;};} } 
namespace mimax {namespace common {template<typename T, size_t TSize>class CStaticVector{public:static const size_t MAX_SIZE = TSize;public:CStaticVector() : m_nxtSlotIndex(0) {}CStaticVector(size_t const size) { resize(size); }CStaticVector(std::initializer_list<T> const& items){resize(items.size());std::copy(items.begin(), items.end(), begin());m_nxtSlotIndex = items.size();}inline void push_back(T const& elem) { assert(m_nxtSlotIndex < TSize); m_data[m_nxtSlotIndex++] = elem; }template<typename ... TArgs>inline T& emplace_back(TArgs&& ... args) { m_data[m_nxtSlotIndex++] = std::move(T(std::forward<TArgs>(args)...)); return back(); }inline void pop_back() { m_nxtSlotIndex--; }inline T& back() { return m_data[m_nxtSlotIndex - 1]; }inline T& front() { return *m_data; }inline T const& back() const { return m_data[m_nxtSlotIndex - 1]; }inline T const& front() const { return *m_data; }inline size_t size() const { return m_nxtSlotIndex; }inline void resize(size_t const capacity) { m_nxtSlotIndex = capacity; }inline void reserve(size_t const capacity) { assert(capacity <= TSize); }inline size_t max_capacity() const { return TSize; }inline bool is_full() const { return m_nxtSlotIndex >= TSize; }void erase_swap(T* const iter){size_t const index = std::distance(m_data, iter);std::swap(m_data[index], m_data[m_nxtSlotIndex - 1]);pop_back();}inline bool empty() const { return m_nxtSlotIndex == 0; }inline void clear() { m_nxtSlotIndex = 0; }inline T* begin() { return m_data; }inline T* end() { return m_data + m_nxtSlotIndex; }inline T const* begin() const { return m_data; }inline T const* end() const { return m_data + m_nxtSlotIndex; }inline T& operator[](size_t const index) { assert(index < m_nxtSlotIndex); return m_data[index]; }inline T const& operator[](size_t const index) const { assert(index < m_nxtSlotIndex); return m_data[index]; }private:size_t m_nxtSlotIndex = 0;T m_data[TSize];};}}
namespace ut3 {namespace game {using Turns = mimax::common::CStaticVector<SVec2, 9 * 9>;int CalculateWinner(SGameState const& gameState);int CalculateWinner(SGameBlockState const blockState);void MakeTurn(SGameState& gameState, int const worldX, int const worldY);void CollectPossibleTurns(SGameState const& gameState, Turns& turnsOut);}}
namespace ut3 {namespace game {struct SGameStateView{SGameStateView(SGameState const& state);void Print();char m_map[9][10];char m_globalBlockStatuses[3][4];char m_player;char m_oppLastTurnX;char m_oppLastTurnY;char m_gameWinner;unsigned char m_xBlocksCnt;unsigned char m_oBlocksCnt;unsigned char m_elementsCnt;};}}
using namespace std::chrono_literals;namespace ut3 {namespace bot {namespace{class CUT3MinimaxResolver{public:CUT3MinimaxResolver(int const myPlayer): m_myPlayer(myPlayer){}float EvaluateState(game::SGameState const& state){int const gameWinner = GAME_STATE_GET_GAME_WINNER(state);if (gameWinner != GAME_STATE_ELEMENT_EMPTY){if (gameWinner == GAME_STATE_ELEMENT_DRAW) return 0.0f;float const depthScore = GAME_STATE_ELEMENTS_COUNT(state) * 0.01f;if (gameWinner == GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(m_myPlayer)) return 10.0f - depthScore;return -(10.0f - depthScore);}auto const globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(state);int const oppPlayer = ((m_myPlayer + 1) & 1);int const myBlocksCnt = (int)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, m_myPlayer);int const oppBlocksCnt = (int)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, oppPlayer);return (float)(myBlocksCnt - oppBlocksCnt);}void GetPossibleMoves(game::Turns& turnsOut, game::SGameState const& state){game::CollectPossibleTurns(state, turnsOut);mimax::common::RandomShuffle(turnsOut.begin(), turnsOut.end());}void MakeMove(game::SGameState& state, SVec2 move){game::MakeTurn(state, move[0], move[1]);}private:int m_myPlayer;};using CUT3MinimaxAlgo = mimax::dma::CMinimaxBase<game::SGameState, SVec2, game::Turns, CUT3MinimaxResolver>;using CMinimaxTask = mimax::dma::CMinimaxTask<CUT3MinimaxAlgo>;}SVec2 CMinimaxBot_v1::FindTurn(game::SGameState const& gameState){return FindTurn(gameState, m_myPlayer, m_isDebugEnabled);}SVec2 CMinimaxBot_v1::FindTurn(game::SGameState const& gameState, int const myPlayer, bool const debugEnabled){if (GAME_STATE_ELEMENTS_COUNT(gameState) == 0){return { 4, 4 };}auto globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(gameState);size_t const playedBlocksCount =GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 0)+ GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 1)+ GAME_STATE_BLOCK_COUNT_DRAW_ELEMENTS(globalBlock);float const minScoreValue = (playedBlocksCount >= 6 ? (-1.0f) : (-2.0f));std::vector<CUT3MinimaxAlgo> minimaxAlgos;std::vector<CMinimaxTask> minimaxTasks;std::vector<mimax::mt::ITask*> tasksToRun;CUT3MinimaxResolver resolver(myPlayer);CUT3MinimaxAlgo::SConfig config;config.m_minValue = minScoreValue;config.m_maxValue = 10.0f;config.m_epsilon = 0.001f;size_t constexpr minDepth = 6;auto const hardwareConcurrency = std::thread::hardware_concurrency();minimaxAlgos.reserve(hardwareConcurrency);minimaxTasks.reserve(hardwareConcurrency);tasksToRun.reserve(hardwareConcurrency);size_t const step = (hardwareConcurrency <= 4)? 2: 1;for (size_t depth = minDepth; depth < minDepth + step * hardwareConcurrency; depth += step){config.m_maxDepth = depth;auto& minimaxAlgo = minimaxAlgos.emplace_back(resolver, config);auto& minimaxTask = minimaxTasks.emplace_back(&minimaxAlgo, gameState);tasksToRun.push_back(&minimaxTask);}std::cerr << "Running ... \n";mimax::mt::CTasksRunner().RunTasksAndWait(tasksToRun, 95ms);std::cerr << "Finished ... \n";SVec2 turn = { -1, -1 };for (int i = (int)minimaxTasks.size() - 1; i >= 0; --i){auto& move = minimaxTasks[i].GetResult();if (move.has_value()){turn = move.value();
#if MIMAX_MINIMAX_DEBUG
if (debugEnabled){std::cerr << minimaxAlgos[i].GetDebugInfo();}
#endif 
break;}}if (turn[0] == -1){std::cerr << "[ERROR] Could not find a next turn using minimax!\n";game::Turns turns;game::CollectPossibleTurns(gameState, turns);turn = *mimax::common::GetRandomItem(turns.begin(), turns.end());}return turn;}}}

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

namespace mimax {namespace mt {using namespace std;void CTasksRunner::RunTasksAndWait(vector<ITask*> const& tasks, chrono::microseconds const waitingTime){m_tasks = tasks;m_futures.clear();RunTasks();Wait(waitingTime);StopTasks();WaitForTasksCompleted();}void CTasksRunner::RunTasks(){for (auto task : m_tasks){auto future = async([task](){task->RunTask();});m_futures.emplace_back(move(future));}}void CTasksRunner::Wait(chrono::microseconds const time){auto const startTime = chrono::high_resolution_clock::now();auto const endTime = startTime + time;while (chrono::high_resolution_clock::now() < endTime){this_thread::yield();}}void CTasksRunner::StopTasks(){for (auto task : m_tasks){task->StopTask();}}void CTasksRunner::WaitForTasksCompleted(){for (auto& future : m_futures){future.wait();}}} } 
namespace ut3 {namespace bot {SOutputData CBotBase::FirstUpdate(SInputData initData){if (initData.m_oppTurnX >= 0){m_myPlayer = 1;return Update(initData);}else{m_myPlayer = 0;auto const turn = FindTurn(m_gameState);game::MakeTurn(m_gameState, turn[0], turn[1]);return { turn[0], turn[1] };}}SOutputData CBotBase::Update(SInputData turnData){game::MakeTurn(m_gameState, turnData.m_oppTurnX, turnData.m_oppTurnY);if (m_isDebugEnabled){game::SGameStateView(m_gameState).Print();}auto const turn = FindTurn(m_gameState);game::MakeTurn(m_gameState, turn[0], turn[1]);return { turn[0], turn[1] };}void CBotBase::Reset(){m_gameState = game::SGameState();m_myPlayer = -1;m_isDebugEnabled = false;}}}
namespace ut3 {namespace game {char ConvertGameStateElemToChar(int const elem);int ConvertCharToGameStateElem(char const c);SGameState CreateGameState(char const map[9][10], SVec2 const oppLastTurn);} } 
namespace ut3 {namespace game {SGameStateView::SGameStateView(SGameState const& state){for (int blockX = 0; blockX < 3; ++blockX){for (int blockY = 0; blockY < 3; ++blockY){int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);m_globalBlockStatuses[blockY][blockX] = ConvertGameStateElemToChar(GAME_STATE_GET_BLOCK_STATUS(state, blockIndex));auto const block = GetBlockState(state, blockIndex);for (int x = 0; x < 3; ++x){for (int y = 0; y < 3; ++y){m_map[y + blockY * 3][x + blockX * 3] = ConvertGameStateElemToChar(GAME_BLOCK_STATE_GET_ELEMENT_BY_POS(block, x, y));}}}}for (int r = 0; r < 3; ++r){m_globalBlockStatuses[r][3] = '\0';}for (int r = 0; r < 9; ++r){m_map[r][9] = '\0';}m_player = ConvertGameStateElemToChar(GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(GAME_STATE_GET_PLAYER(state)));m_oppLastTurnX = GAME_STATE_GET_LAST_OPP_TURN_X(state);m_oppLastTurnY = GAME_STATE_GET_LAST_OPP_TURN_Y(state);m_gameWinner = ConvertGameStateElemToChar(GAME_STATE_GET_GAME_WINNER(state));auto const globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(state);m_xBlocksCnt = (unsigned char)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 0);m_oBlocksCnt = (unsigned char)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 1);m_elementsCnt = (unsigned char)GAME_STATE_ELEMENTS_COUNT(state);}void SGameStateView::Print(){for (int r = 0; r < 9; ++r){std::cerr << m_map[r] << "\n";}for (int r = 0; r < 3; ++r){std::cerr << m_globalBlockStatuses[r] << "\n";}std::cerr << "Player: " << m_player << "\n";std::cerr << "Opp last turn: " << (int)m_oppLastTurnY << ", " << (int)m_oppLastTurnX << "\n";std::cerr << "Game winner: " << m_gameWinner << "\n";std::cerr << "X blocks count: " << (int)m_xBlocksCnt << "\n";std::cerr << "O blocks count: " << (int)m_oBlocksCnt << "\n";std::cerr << "Elements count: " << (int)m_elementsCnt << "\n";}}}
namespace ut3 {namespace game {char ConvertGameStateElemToChar(int const elem){if (elem == GAME_STATE_ELEMENT_EMPTY) return '-';else if (elem == GAME_STATE_ELEMENT_X) return 'X';else if (elem == GAME_STATE_ELEMENT_O) return 'O';return 'D';}int ConvertCharToGameStateElem(char const c){if (c == '-') return GAME_STATE_ELEMENT_EMPTY;else if (c == 'X') return GAME_STATE_ELEMENT_X;else if (c == 'O') return GAME_STATE_ELEMENT_O;return GAME_STATE_ELEMENT_DRAW;}SGameState CreateGameState(char const map[9][10], SVec2 const oppLastTurn){SGameState gameState;size_t const elementsCount = GAME_STATE_ELEMENTS_COUNT(gameState);size_t const palyerIndex = (elementsCount & 1) ? 1 : 0;GAME_STATE_SET_PLAYER(gameState, palyerIndex);GAME_STATE_SET_LAST_OPP_TURN_X(gameState, oppLastTurn[0]);GAME_STATE_SET_LAST_OPP_TURN_Y(gameState, oppLastTurn[1]);for (int blockX = 0; blockX < 3; ++blockX){for (int blockY = 0; blockY < 3; ++blockY){SGameBlockState blockState;for (int x = 0; x < 3; ++x){for (int y = 0; y < 3; ++y){int const elem = ConvertCharToGameStateElem(map[y + 3 * blockY][x + 3 * blockX]);GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, x, y, elem);}}int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);SetBlockState(gameState, blockIndex, blockState);int const blockWinner = CalculateWinner(blockState);GAME_STATE_SET_BLOCK_STATUS(gameState, blockIndex, blockWinner);}}int const gameWinner = CalculateWinner(gameState);GAME_STATE_SET_GAME_WINNER(gameState, gameWinner);return gameState;}} } 
namespace{constexpr size_t WINNER_MASKS_COUNT = 8;constexpr unsigned int WINNER_MASKS[WINNER_MASKS_COUNT] = {21, 1344, 86016, 4161, 16644, 66576, 65793, 4368};
#   define GET_WINNER_MASK_FOR_PLAYER(mask, playerIndex) ((mask) << (playerIndex))
inline void AddAvailableTurns(ut3::game::Turns& turnsOut, ut3::game::SGameBlockState block, int const blockX, int const blockY){for (int x = 0; x < 3; ++x){for (int y = 0; y < 3; ++y){if (GAME_BLOCK_STATE_GET_ELEMENT_BY_POS(block, x, y) == GAME_STATE_ELEMENT_EMPTY){turnsOut.push_back({x + 3 * blockX, y + 3 * blockY});}}}}}namespace ut3 {namespace game {int CalculateWinner(SGameState const& gameState){auto const globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(gameState);int const blockWinner = CalculateWinner(globalBlock);if (blockWinner == GAME_STATE_ELEMENT_DRAW){int const xCnt = GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 0);int const oCnt = GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 1);if (xCnt == oCnt) return GAME_STATE_ELEMENT_DRAW;return (xCnt > oCnt) ? GAME_STATE_ELEMENT_X : GAME_STATE_ELEMENT_O;}return blockWinner;}int CalculateWinner(SGameBlockState const blockState){for (int i = 0; i < WINNER_MASKS_COUNT; ++i){unsigned int const mask0 = GET_WINNER_MASK_FOR_PLAYER(WINNER_MASKS[i], 0);unsigned int const mask1 = GET_WINNER_MASK_FOR_PLAYER(WINNER_MASKS[i], 1);if ((mask0 & blockState.m_data) == mask0){if (UT3_COUNT_BITS_32(mask1 & blockState.m_data) == 0)return GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(0);}else if ((mask1 & blockState.m_data) == mask1){if (UT3_COUNT_BITS_32(mask0 & blockState.m_data) == 0)return GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(1);}}for (int i = 0; i < 9; ++i){if (GAME_BLOCK_STATE_GET_ELEMENT(blockState, i) == GAME_STATE_ELEMENT_EMPTY)return GAME_STATE_ELEMENT_EMPTY;}return GAME_STATE_ELEMENT_DRAW;}void MakeTurn(SGameState& gameState, int const worldX, int const worldY){int const blockX = worldX / 3;int const blockY = worldY / 3;int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);auto block = GetBlockState(gameState, blockIndex);int const localX = worldX - 3 * blockX;int const localY = worldY - 3 * blockY;int const playerIndex = GAME_STATE_GET_PLAYER(gameState);GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(block, localX, localY, GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(playerIndex));SetBlockState(gameState, blockIndex, block);GAME_STATE_SET_PLAYER(gameState, ((playerIndex + 1) & 1));GAME_STATE_SET_LAST_OPP_TURN_X(gameState, worldX);GAME_STATE_SET_LAST_OPP_TURN_Y(gameState, worldY);int const blockWinner = CalculateWinner(block);if (blockWinner != GAME_STATE_ELEMENT_EMPTY){GAME_STATE_SET_BLOCK_STATUS(gameState, blockIndex, blockWinner);int const gameWinner = CalculateWinner(gameState);GAME_STATE_SET_GAME_WINNER(gameState, gameWinner);}}void CollectPossibleTurns(SGameState const& gameState, Turns& turnsOut){turnsOut.clear();if (GAME_STATE_GET_GAME_WINNER(gameState) != GAME_STATE_ELEMENT_EMPTY)return;bool findAllTurns = true;if (GAME_STATE_IS_VALID_LAST_OPP_TURN(gameState)){int const blockX = GAME_STATE_GET_LAST_OPP_TURN_X(gameState) % 3;int const blockY = GAME_STATE_GET_LAST_OPP_TURN_Y(gameState) % 3;int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);if (GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) == GAME_STATE_ELEMENT_EMPTY){findAllTurns = false;auto const block = GetBlockState(gameState, blockIndex);AddAvailableTurns(turnsOut, block, blockX, blockY);}}if (findAllTurns){for (int blockX = 0; blockX < 3; ++blockX){for (int blockY = 0; blockY < 3; ++blockY){int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);if (GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) == GAME_STATE_ELEMENT_EMPTY){auto const block = GetBlockState(gameState, blockIndex);AddAvailableTurns(turnsOut, block, blockX, blockY);}}}}}}}

#if MIMAX_MINIMAX_DEBUG
namespace mimax {namespace dma {std::ostream& operator<<(std::ostream& o, SMinimaxDebugInfo const& debugInfo){size_t const mxDepth = debugInfo.m_maxDepth > SMinimaxDebugInfo::MAX_DEPTH? SMinimaxDebugInfo::MAX_DEPTH: debugInfo.m_maxDepth + 1;o << "Evaluated nodes count: " << debugInfo.m_evaluatedNodesCnt << "\n";o << "Visited nodes count " << debugInfo.m_totalVisitedNodesCnt << ": \n";for (size_t depth = 1; depth <= mxDepth; ++depth){o << depth << ": " << debugInfo.m_visitedNodesCnt[depth] << "\n";}o << "Pruned nodes count " << debugInfo.m_totalPrunedNodesCnt << ": \n";for (size_t depth = 1; depth <= mxDepth; ++depth){o << depth << ": " << debugInfo.m_prunedNodesCnt[depth] << "\n";}return o;}} } 
#endif 

namespace mimax {namespace common {char const* ToString(SVec2i const vec2){static char buff[32];sprintf(buff, "{%i:%i}", vec2[0], vec2[1]);return buff;}char const* ToString(SVec2char const vec2){static char buff[32];sprintf(buff, "{%i:%i}", (int)vec2[0], (int)vec2[1]);return buff;}std::ostream& operator<<(std::ostream& out, SVec2char const& vec2){out << (int)vec2[0] << " " << (int)vec2[1];return out;}std::istream& operator>>(std::istream& in, SVec2char& vec2){int v1, v2;in >> v1 >> v2;vec2[0] = (char)v1;vec2[1] = (char)v2;return in;}}}
using namespace std;namespace{static int gNestedLevel = 0;}namespace mimax {namespace common {CTimeProfiler::CTimeProfiler(char const* const name): m_name(name), m_startTime(chrono::high_resolution_clock::now()){++gNestedLevel;}CTimeProfiler::~CTimeProfiler(){auto const end = chrono::high_resolution_clock::now();double const executionTime = (double)chrono::duration_cast<chrono::nanoseconds>(end - m_startTime).count();string const tabs(gNestedLevel, '-');cerr.precision(3);cerr << "[Profiler] " << tabs << m_name << ": " << executionTime / 1000000.0 << fixed << " ms" << endl;--gNestedLevel;}}}
#include <time.h>
namespace mimax {namespace common {unsigned int GenerateRandomSeed(){return (unsigned int)time(nullptr);}unsigned int UpdateRandomSeed(unsigned int seed){if (seed == 0)seed = GenerateRandomSeed();srand(seed);return seed;}unsigned int GetRandomUInt32(unsigned int const minVal, unsigned int const maxVal){assert(minVal < maxVal);unsigned int const range = maxVal - minVal;return rand() % range + minVal;}}}

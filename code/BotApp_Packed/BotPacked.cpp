
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC optimize("O3")
#pragma GCC optimize("inline")
#pragma GCC optimize("omit-frame-pointer")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("Ofast")
#endif 

#define _CRT_SECURE_NO_WARNINGS


#include <chrono>
#include <ctime>
#include <string>
namespace mimax {namespace common {class CTimeProfiler{public:CTimeProfiler(char const* const name);~CTimeProfiler();private:std::string m_name;decltype(std::chrono::high_resolution_clock::now()) m_startTime;};}}
#ifndef DISABLE_PROFILER
#define PROFILE_TIME(name) mimax::common::CTimeProfiler timeProfiler(name);
#else
#define PROFILE_TIME(name)
#endif 

namespace ut3{struct SInputData{int m_oppTurnY;int m_oppTurnX;};};
namespace ut3{struct SOutputData{int m_turnX;int m_turnY;};};

#include <cassert>
namespace ut3 {namespace game {
#   define UT3_FIRST_BITS_MASK(bitsCount)  ((~(1ULL)) >> (64 - bitsCount)) 

#   define UT3_SET_BIT(number, bitPos, bitVal) number = ((number & (~(1ULL << (bitPos)))) | ((unsigned long long)((bitVal) == 0 ? 0 : 1) << (bitPos)))
#   define UT3_GET_BIT(number, bitPos) (((number) >> (bitPos)) & 1)

#   define UT3_SET_BITS(number, bitStartPos, bitsCount, bitsVal) number = ((number & (~(UT3_FIRST_BITS_MASK((unsigned long long)(bitsCount)) << (bitStartPos)))) | (((unsigned long long)(bitsVal)) << (bitStartPos)))
#   define UT3_GET_BITS(number, bitStartPos, bitsCount) (((number) >> (bitStartPos)) & UT3_FIRST_BITS_MASK((unsigned long long)(bitsCount)))

#   define GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(playerIndex) (1ULL << playerIndex)

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
struct SGameState{SGameState(){m_data[0] = m_data[1] = m_data[2] = 0;GAME_STATE_INVALIDATE_LAST_OPP_TURN(*this);}unsigned long long m_data[3];};static_assert(sizeof(SGameState) == 8 * 3, "SGameState has invalid size");struct SGameBlockState{unsigned int m_data = 0;inline bool operator==(SGameBlockState other) const { return m_data == other.m_data; }};static_assert(sizeof(SGameBlockState) == 4, "SGameBlockState has invalid size");inline SGameBlockState GetBlockState(SGameState const gameState, size_t const blockIndex){assert(blockIndex < 9);if (blockIndex < 3){return SGameBlockState{ UT3_GET_BITS(gameState.m_data[0], blockIndex * 18, 18) };}if (blockIndex >= 4){if (blockIndex < 7){return SGameBlockState{ UT3_GET_BITS(gameState.m_data[1], (blockIndex - 4) * 18 + 8, 18) };}else{return SGameBlockState{ UT3_GET_BITS(gameState.m_data[2], (blockIndex - 7) * 18, 18) };}}return SGameBlockState{ (gameState.m_data[0] >> 54) | ((gameState.m_data[1] & UT3_FIRST_BITS_MASK(8)) << 10) };}inline void SetBlockState(SGameState& gameState, size_t const blockIndex, SGameBlockState const blockState){assert(blockIndex < 9);if (blockIndex < 3){UT3_SET_BITS(gameState.m_data[0], blockIndex * 18, 18, blockState.m_data);}else if (blockIndex >= 4){if (blockIndex < 7){UT3_SET_BITS(gameState.m_data[1], (blockIndex - 4) * 18 + 8, 18, blockState.m_data);}else{UT3_SET_BITS(gameState.m_data[2], (blockIndex - 7) * 18, 18, blockState.m_data);}}else{unsigned long long const firstBits = (blockState.m_data & UT3_FIRST_BITS_MASK(10));unsigned long long const secondBits = UT3_GET_BITS(blockState.m_data, 10, 8);UT3_SET_BITS(gameState.m_data[0], 54, 10, firstBits);UT3_SET_BITS(gameState.m_data[1], 0, 8, secondBits);}}}}
namespace ut3{class CMinimaxBot{public:CMinimaxBot(SInputData initData);SOutputData FirstUpdate(SInputData initData);SOutputData Update(SInputData turnData);private:int m_myPlayer;game::SGameState m_gameState;};}

#include <iostream>
void ReadData(ut3::SInputData& inputData){std::cin >> inputData.m_oppTurnX >> inputData.m_oppTurnY;int validActionCount;scanf("%i", &validActionCount);for (int i = 0; i < validActionCount; ++i){int x, y;scanf("%i%i", &x, &y);}}void WriteData(ut3::SOutputData const& outputData){std::cout << outputData.m_turnX << " " << outputData.m_turnY << "\n";}int main(){ut3::SInputData inData;ReadData(inData);ut3::CMinimaxBot bot(inData);WriteData(bot.FirstUpdate(inData));while (true){ReadData(inData);{PROFILE_TIME("Update");WriteData(bot.Update(inData));}}return 0;}

#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <array>
#include <assert.h>
#include <cstring>
#include <functional>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>


#include <limits>

#define MINIMAX_ENABLE_ALPHA_BETA_PRUNING (1)
namespace mimax {namespace minimax {template<typename TState, typename TMove, typename TMovesContainer, typename TResolver>class CMinimaxAlgorithmBase{public:CMinimaxAlgorithmBase(size_t const maxDepth, TResolver const& resolver): m_maxDepth(maxDepth), m_resolver(resolver)
#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
, m_minValue(-std::numeric_limits<float>::max()), m_maxValue(std::numeric_limits<float>::max())
#endif 
{}
#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
CMinimaxAlgorithmBase(size_t const maxDepth, TResolver const& resolver, float const minValue, float const maxValue): m_maxDepth(maxDepth), m_resolver(resolver), m_minValue(minValue), m_maxValue(maxValue){}
#endif 
inline TMove Solve(TState const& state){
#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
return VisitState(state, 0, m_minValue, m_maxValue).m_move;
#else
return VisitState(state, 0).m_move;
#endif 
}private:struct STraversalResult{float m_score = 0.0f;TMove m_move;};private:
#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
STraversalResult VisitState(TState const& state, size_t const depth, float alpha, float beta)
#else
STraversalResult VisitState(TState const& state, size_t const depth)
#endif 
{TMovesContainer moves;if(depth != m_maxDepth){m_resolver.GetPossibleMoves(moves, state);}STraversalResult result;if(moves.empty()){int const colorMultiplier = (depth & 1) == 0 ? 1 : -1;result.m_score = m_resolver.EvaluateState(state) * colorMultiplier;return result;}result.m_score = -std::numeric_limits<float>::max();for(auto const move: moves){TState childState = state;m_resolver.MakeMove(childState, move);
#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
auto const childResult = VisitState(childState, depth + 1, -beta, -alpha);
#else
auto const childResult = VisitState(childState, depth + 1);
#endif 
if(-childResult.m_score > result.m_score){result.m_move = move;result.m_score = -childResult.m_score;
#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
alpha = (result.m_score > alpha) ? result.m_score : alpha;if (alpha >= beta) break;
#endif 
}}return result;}private:TResolver m_resolver;size_t m_maxDepth;
#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
float m_minValue;float m_maxValue;
#endif 
};}}
namespace mimax {namespace common {template<typename T, size_t TSize>class CStaticVector{public:static const size_t MAX_SIZE = TSize;public:CStaticVector() : m_nxtSlotIndex(0) {}CStaticVector(size_t const size) { resize(size); }CStaticVector(std::initializer_list<T> const& items){resize(items.size());std::copy(items.begin(), items.end(), begin());m_nxtSlotIndex = items.size();}inline void push_back(T const& elem) { assert(m_nxtSlotIndex < TSize); m_data[m_nxtSlotIndex++] = elem; }template<typename ... TArgs>inline T& emplace_back(TArgs&& ... args) { m_data[m_nxtSlotIndex++] = std::move(T(std::forward<TArgs>(args)...)); return back(); }inline void pop_back() { m_nxtSlotIndex--; }inline T& back() { return m_data[m_nxtSlotIndex - 1]; }inline T& front() { return *m_data; }inline T const& back() const { return m_data[m_nxtSlotIndex - 1]; }inline T const& front() const { return *m_data; }inline size_t size() const { return m_nxtSlotIndex; }inline void resize(size_t const capacity) { m_nxtSlotIndex = capacity; }inline void reserve(size_t const capacity) { assert(capacity <= TSize); }inline size_t max_capacity() const { return TSize; }inline bool is_full() const { return m_nxtSlotIndex >= TSize; }void erase_swap(T* const iter){size_t const index = std::distance(m_data, iter);std::swap(m_data[index], m_data[m_nxtSlotIndex - 1]);pop_back();}inline bool empty() const { return m_nxtSlotIndex == 0; }inline void clear() { m_nxtSlotIndex = 0; }inline T* begin() { return m_data; }inline T* end() { return m_data + m_nxtSlotIndex; }inline T const* begin() const { return m_data; }inline T const* end() const { return m_data + m_nxtSlotIndex; }inline T& operator[](size_t const index) { assert(index < m_nxtSlotIndex); return m_data[index]; }inline T const& operator[](size_t const index) const { assert(index < m_nxtSlotIndex); return m_data[index]; }private:size_t m_nxtSlotIndex = 0;T m_data[TSize];};}}
namespace mimax {namespace common {template<typename TComponent>struct SVec2Base{static const size_t ms_vecSize = 2;using ValueType = TComponent;template<typename TOtherComponent>explicit SVec2Base(SVec2Base<TOtherComponent> const other): m_components{ static_cast<TComponent>(other[0]), static_cast<TComponent>(other[1]) }{}template<typename TOtherComponent>SVec2Base(TOtherComponent const x, TOtherComponent const y): m_components{ static_cast<TComponent>(x), static_cast<TComponent>(y) }{}template<typename TOtherComponent>SVec2Base(std::initializer_list<TOtherComponent> const& data){assert(data.size() <= ms_vecSize);std::transform(data.begin(), data.end(), m_data,[](TOtherComponent const otherComp){return static_cast<TComponent>(otherComp);});}SVec2Base() : m_components{0, 0} {}SVec2Base(TComponent const x, TComponent const y) : m_components{x, y} {}SVec2Base(std::initializer_list<TComponent> const& data){assert(data.size() <= ms_vecSize);std::copy(data.begin(), data.end(), m_data);}inline SVec2Base& operator=(SVec2Base const other) { m_data[0] = other.m_data[0]; m_data[1] = other.m_data[1]; return *this; }inline bool operator==(SVec2Base const other) const { return other[0] == m_data[0] && other[1] == m_data[1]; }inline bool operator!=(SVec2Base const other) const { return !(*this == other); }inline TComponent operator[](size_t const index) const { return m_data[index]; }inline TComponent& operator[](size_t const index) { return m_data[index]; }inline SVec2Base operator+(SVec2Base const other) const { return SVec2Base(m_data[0] + other.m_data[0], m_data[1] + other.m_data[1]); }inline SVec2Base operator-(SVec2Base const other) const { return SVec2Base(m_data[0] - other.m_data[0], m_data[1] - other.m_data[1]); }inline bool operator<(SVec2Base const other) const { if (m_data[0] == other[0]) { return m_data[1] < other[1]; } return m_data[0] < other[0]; }union{struct{TComponent m_x;TComponent m_y;} m_components;TComponent m_data[ms_vecSize];};};using SVec2i = SVec2Base<int>;using SVec2u = SVec2Base<unsigned int>;using SVec2si = SVec2Base<short>;using SVec2su = SVec2Base<unsigned short>;}}using SVec2 = mimax::common::SVec2Base<char>;
namespace ut3 {namespace game {using Turns = mimax::common::CStaticVector<SVec2, 9 * 9>;int CalculateWinner(SGameState const& gameState);int CalculateWinner(SGameBlockState const blockState);void MakeTurn(SGameState& gameState, int const worldX, int const worldY);void CollectPossibleTurns(SGameState const& gameState, Turns& turnsOut);}}

#include <bitset>
#include <thread>
using namespace std::chrono_literals;namespace ut3{namespace{constexpr unsigned int PLAYERS_GLOBAL_BLOCK_MASKS[2] = {87381,174762};class CUT3MinimaxResolver{public:CUT3MinimaxResolver(int const myPlayer): m_myPlayer(myPlayer){}float EvaluateState(game::SGameState const& state){int const gameWinner = GAME_STATE_GET_GAME_WINNER(state);if (gameWinner != GAME_STATE_ELEMENT_EMPTY){if (gameWinner == GAME_STATE_ELEMENT_DRAW) return 0.0f;if (gameWinner == GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(m_myPlayer)) return 10.0f;return -10.0f;}auto const globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(state);int const oppPlayer = (m_myPlayer & 1);size_t const myBlocksCnt = std::bitset<32>(globalBlock.m_data & PLAYERS_GLOBAL_BLOCK_MASKS[m_myPlayer]).count();size_t const oppBlocksCnt = std::bitset<32>(globalBlock.m_data & PLAYERS_GLOBAL_BLOCK_MASKS[oppPlayer]).count();return (float)(myBlocksCnt - oppBlocksCnt);}void GetPossibleMoves(game::Turns& turnsOut, game::SGameState const& state){game::CollectPossibleTurns(state, turnsOut);}void MakeMove(game::SGameState& state, SVec2 move){game::MakeTurn(state, move[0], move[1]);}private:int m_myPlayer;};}CMinimaxBot::CMinimaxBot(SInputData /*initData*/): m_myPlayer(-1){}SOutputData CMinimaxBot::FirstUpdate(SInputData initData){if (initData.m_oppTurnX >= 0){m_myPlayer = 1;return Update(initData);}else{m_myPlayer = 0;SOutputData output{ 4, 4 };game::MakeTurn(m_gameState, output.m_turnX, output.m_turnY);return output;}}SOutputData CMinimaxBot::Update(SInputData turnData){game::MakeTurn(m_gameState, turnData.m_oppTurnX, turnData.m_oppTurnY);using CUT3MinimaxAlgo = mimax::minimax::CMinimaxAlgorithmBase<game::SGameState, SVec2, game::Turns, CUT3MinimaxResolver>;CUT3MinimaxAlgo minimax(5, CUT3MinimaxResolver(m_myPlayer));SVec2 turn1 = minimax.Solve(m_gameState);game::MakeTurn(m_gameState, turn1[0], turn1[1]);return { turn1[0], turn1[1] };}}
namespace{constexpr size_t WINNER_MASKS_COUNT = 8;constexpr unsigned int WINNER_MASKS[WINNER_MASKS_COUNT] = {21, 1344, 86016, 4161, 16644, 66576, 65793, 4368};
#   define GET_WINNER_MASK_FOR_PLAYER(mask, playerIndex) ((mask) << (playerIndex))
inline void AddAvailableTurns(ut3::game::Turns& turnsOut, ut3::game::SGameBlockState block, int const blockX, int const blockY){for (int x = 0; x < 3; ++x){for (int y = 0; y < 3; ++y){if (GAME_BLOCK_STATE_GET_ELEMENT_BY_POS(block, x, y) == GAME_STATE_ELEMENT_EMPTY){turnsOut.push_back({x + 3 * blockX, y + 3 * blockY});}}}}}namespace ut3 {namespace game {int CalculateWinner(SGameState const& gameState){return CalculateWinner(GAME_STATE_GET_GLOBAL_BLOCK(gameState));}int CalculateWinner(SGameBlockState const blockState){for (int i = 0; i < WINNER_MASKS_COUNT; ++i){for (int playerIndex = 0; playerIndex < 2; ++playerIndex){unsigned int const mask = GET_WINNER_MASK_FOR_PLAYER(WINNER_MASKS[i], playerIndex);if ((mask & blockState.m_data) == mask){return GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(playerIndex);}}}for (int i = 0; i < 9; ++i){if (GAME_BLOCK_STATE_GET_ELEMENT(blockState, i) == GAME_STATE_ELEMENT_EMPTY)return GAME_STATE_ELEMENT_EMPTY;}return GAME_STATE_ELEMENT_DRAW;}void MakeTurn(SGameState& gameState, int const worldX, int const worldY){int const blockX = worldX / 3;int const blockY = worldY / 3;int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);auto block = GetBlockState(gameState, blockIndex);int const localX = worldX - 3 * blockX;int const localY = worldY - 3 * blockY;int const playerIndex = GAME_STATE_GET_PLAYER(gameState);GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(block, localX, localY, GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(playerIndex));SetBlockState(gameState, blockIndex, block);GAME_STATE_SET_PLAYER(gameState, ((playerIndex + 1) & 1));GAME_STATE_SET_LAST_OPP_TURN_X(gameState, worldX);GAME_STATE_SET_LAST_OPP_TURN_Y(gameState, worldY);int const blockWinner = CalculateWinner(block);if (blockWinner != GAME_STATE_ELEMENT_EMPTY){GAME_STATE_SET_BLOCK_STATUS(gameState, blockIndex, blockWinner);int const gameWinner = CalculateWinner(gameState);GAME_STATE_SET_GAME_WINNER(gameState, gameWinner);}}void CollectPossibleTurns(SGameState const& gameState, Turns& turnsOut){turnsOut.clear();if (GAME_STATE_GET_GAME_WINNER(gameState) != GAME_STATE_ELEMENT_EMPTY)return;bool findAllTurns = true;if (GAME_STATE_IS_VALID_LAST_OPP_TURN(gameState)){int const blockX = GAME_STATE_GET_LAST_OPP_TURN_X(gameState) % 3;int const blockY = GAME_STATE_GET_LAST_OPP_TURN_Y(gameState) % 3;int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);if (GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) == GAME_STATE_ELEMENT_EMPTY){findAllTurns = false;auto const block = GetBlockState(gameState, blockIndex);AddAvailableTurns(turnsOut, block, blockX, blockY);}}if (findAllTurns){for (int blockX = 0; blockX < 3; ++blockX){for (int blockY = 0; blockY < 3; ++blockY){int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);if (GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) == GAME_STATE_ELEMENT_EMPTY){auto const block = GetBlockState(gameState, blockIndex);AddAvailableTurns(turnsOut, block, blockX, blockY);}}}}}}}

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

using namespace std;namespace{static int gNestedLevel = 0;}namespace mimax {namespace common {CTimeProfiler::CTimeProfiler(char const* const name): m_name(name), m_startTime(chrono::high_resolution_clock::now()){++gNestedLevel;}CTimeProfiler::~CTimeProfiler(){auto const end = chrono::high_resolution_clock::now();double const executionTime = (double)chrono::duration_cast<chrono::nanoseconds>(end - m_startTime).count();string const tabs(gNestedLevel, '-');cerr.precision(3);cerr << "[Profiler] " << tabs << m_name << ": " << executionTime / 1000000.0 << fixed << " ms" << endl;--gNestedLevel;}}}

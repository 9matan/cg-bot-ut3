
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC optimize("O3")
#pragma GCC optimize("inline")
#pragma GCC optimize("omit-frame-pointer")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("Ofast")
#endif 

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

#else 

#   ifdef MIMAX_MCTS_DEBUG
#   undef MIMAX_MCTS_DEBUG
#   endif
#   define MIMAX_MCTS_DEBUG (1)

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
namespace mimax {namespace common {class CMathHelper{public:static inline float log(float const val) { return ::log(val); }static inline float sqrt(float const val) { return ::sqrt(val); }};} } 
namespace mimax {namespace common {template<typename TObject>class CObjectPool{public:inline void ResetPool(){m_objects.clear();}inline void ResetPool(size_t const poolSize){m_objects.clear();m_objects.reserve(poolSize);}inline bool IsEmpty() { return m_objects.size() == m_objects.capacity(); }inline size_t GetAllocatedSize() const { return m_objects.size(); }template<typename ...TArgs>inline TObject* AllocateObject(TArgs&& ...args){assert(m_objects.size() < m_objects.capacity());m_objects.emplace_back(std::forward<TArgs>(args)...);return &m_objects.back();}private:std::vector<TObject> m_objects;};}}

#if MIMAX_MCTS_DEBUG
namespace mimax {namespace dma {struct SMctsDebugInfo{std::vector<size_t> m_nodesCnt;std::vector<size_t> m_visitedNodesCnt;size_t m_totalNodesCnt;size_t m_totalVisitedNodesCnt;size_t m_simulationsCnt;SMctsDebugInfo(){Reset();}inline void ExpanseNode(size_t const nodeDepth, size_t const childrenCnt){m_totalNodesCnt += childrenCnt;if (nodeDepth + 1 >= m_nodesCnt.size()){m_nodesCnt.resize(nodeDepth + 2, 0);}m_nodesCnt[nodeDepth + 1] += childrenCnt;}inline void VisitNode(size_t const nodeDepth){++m_totalVisitedNodesCnt;if (nodeDepth >= m_visitedNodesCnt.size()){m_visitedNodesCnt.resize(nodeDepth + 1, 0);}++m_visitedNodesCnt[nodeDepth];}inline void Reset(){m_totalNodesCnt = 0;m_totalVisitedNodesCnt = 0;m_simulationsCnt = 0;m_nodesCnt.clear();m_nodesCnt.reserve(64);m_visitedNodesCnt.clear();m_visitedNodesCnt.reserve(64);}inline size_t GetMaxDepth() const { return m_visitedNodesCnt.size(); }};std::ostream& operator<<(std::ostream& o, SMctsDebugInfo const& debugInfo);} } 
#endif 

namespace mimax {namespace dma {template<typename TMove>struct SMctsNodeEvalResult{struct SChildInfo{TMove m_move;unsigned int m_simulationsCount;bool operator==(SChildInfo const& other) const{return m_move == other.m_move&& m_simulationsCount == other.m_simulationsCount;}};std::vector<SChildInfo> m_childrenInfo;};} } 
namespace mimax {namespace dma {template<typename TState,typename TMove,typename TMovesContainer,typename TResolver,typename TMathHelper = mimax::common::CMathHelper>class CMctsBase{public:struct SConfig{size_t m_randomSeed;size_t m_nodesPoolSize = 65000;float m_explorationParam = 1.4142f;};public:CMctsBase(TResolver const& resolver, SConfig const& config): m_resolver(resolver), m_randomEngine(config.m_randomSeed), m_config(config)
#if MIMAX_MCTS_DEBUG
, m_currentDepth(0)
#endif 
{}void Reset(TState const& state){
#if MIMAX_MCTS_DEBUG
m_debugInfo.Reset();m_currentDepth = 0;
#endif 
m_nodesPool.ResetPool(m_config.m_nodesPoolSize);m_root = AllocateNode();m_root->m_state = state;bool const successful = Expanse(m_root);assert(successful);}void Evaluate(){MakeIteration();}TMove GetCurrentResult() const{SNode* childNode = m_root->m_firstChild;SNode* bestNode = m_root->m_firstChild;while (childNode != nullptr){if (childNode->m_simulationsCount > bestNode->m_simulationsCount){bestNode = childNode;}childNode = childNode->m_nextNode;}return bestNode->m_move;}SMctsNodeEvalResult<TMove> GetRootEvalResult() const{SMctsNodeEvalResult<TMove> evalResult;SNode* childNode = m_root->m_firstChild;while(childNode != nullptr){auto& info = evalResult.m_childrenInfo.emplace_back();info.m_move = childNode->m_move;info.m_simulationsCount = childNode->m_simulationsCount;childNode = childNode->m_nextNode;}return std::move(evalResult);}
#if MIMAX_MCTS_DEBUG
inline SMctsDebugInfo const& GetDebugInfo() const { return m_debugInfo; }
#endif 
private:struct SNode{SNode(): m_simulationsCount(0), m_score(0), m_parent(nullptr), m_nextNode(nullptr), m_firstChild(nullptr), m_unvisitedChildrenCount(0){}inline bool IsVisited() const { return m_simulationsCount > 0; }inline bool HasChildren() const { return m_firstChild != nullptr; }inline bool HasUnvisitedChildren() const { return m_unvisitedChildrenCount > 0; }unsigned int m_simulationsCount;float m_score;SNode* m_parent;SNode* m_nextNode;SNode* m_firstChild;TState m_state;TMove m_move;unsigned short m_unvisitedChildrenCount;};private:std::mt19937_64 m_randomEngine;TResolver m_resolver;SNode* m_root;SConfig m_config;TMovesContainer m_movesBuffer;mimax::common::CObjectPool<SNode> m_nodesPool;
#if MIMAX_MCTS_DEBUG
SMctsDebugInfo m_debugInfo;size_t m_currentDepth;
#endif 
private:inline SNode* AllocateNode(){if (m_nodesPool.IsEmpty()) return nullptr;return m_nodesPool.AllocateObject();}bool Expanse(SNode* node){m_resolver.GetPossibleMoves(node->m_state, m_movesBuffer);std::shuffle(m_movesBuffer.begin(), m_movesBuffer.end(), m_randomEngine);SNode** curChildIter = &(node->m_firstChild);for (auto const move : m_movesBuffer){SNode* curChild = AllocateNode();if (curChild == nullptr) break;curChild->m_parent = node;curChild->m_move = move;*curChildIter = curChild;curChildIter = &(curChild->m_nextNode);++node->m_unvisitedChildrenCount;}
#if MIMAX_MCTS_DEBUG
m_debugInfo.ExpanseNode(m_currentDepth, node->m_unvisitedChildrenCount);
#endif 
return node->m_unvisitedChildrenCount > 0;}void MakeIteration(){SNode* curNode = SelectChildNode(m_root);
#if MIMAX_MCTS_DEBUG
m_currentDepth = 1;
#endif 
while (curNode->IsVisited()){if (!curNode->HasChildren() && !Expanse(curNode)){break;}curNode = SelectChildNode(curNode);
#if MIMAX_MCTS_DEBUG
++m_currentDepth;
#endif 
}float score = curNode->IsVisited()? curNode->m_score / (float)curNode->m_simulationsCount: VisitNode(curNode);while (curNode != nullptr){UpdateStatistics(curNode, score);score = 1.0f - score;curNode = curNode->m_parent;
#if MIMAX_MCTS_DEBUG
--m_currentDepth;
#endif 
}
#if MIMAX_MCTS_DEBUG
++m_debugInfo.m_simulationsCnt;
#endif 
}inline SNode* SelectChildNode(SNode* node){return node->HasUnvisitedChildren()? GetFirstUnvisitedChild(node): GetBestNodeByUCT(node);}inline float VisitNode(SNode* node){auto parent = node->m_parent;--parent->m_unvisitedChildrenCount;node->m_state = parent->m_state;m_resolver.MakeMove(node->m_state, node->m_move);
#if MIMAX_MCTS_DEBUG
m_debugInfo.VisitNode(m_currentDepth);
#endif 
return m_resolver.Playout(node->m_state);}inline void UpdateStatistics(SNode* node, float const score){++node->m_simulationsCount;node->m_score += score;}inline SNode* GetFirstUnvisitedChild(SNode* node){auto childNode = node->m_firstChild;while (childNode != nullptr){if (!childNode->IsVisited()){return childNode;}childNode = childNode->m_nextNode;}assert(false);return nullptr;}inline SNode* GetBestNodeByUCT(SNode* node){assert(node->HasChildren());SNode* childNode = node->m_firstChild;SNode* bestNode = node->m_firstChild;float bestUCTScore = 0.0f;while (childNode != nullptr){float const childUCTScore = CalculateUCTScore(childNode);if (childUCTScore > bestUCTScore){bestNode = childNode;bestUCTScore = childUCTScore;}childNode = childNode->m_nextNode;}return bestNode;}inline float CalculateUCTScore(SNode const* node){auto const parentSimCount = node->m_parent->m_simulationsCount;return  1.0f - (float)node->m_score / (float)node->m_simulationsCount+ m_config.m_explorationParam * TMathHelper::sqrt(TMathHelper::log(parentSimCount) / node->m_simulationsCount);}};} } 
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
namespace ut3 {namespace bot {class IBot{public:virtual ~IBot() {}virtual SOutputData FirstUpdate(SInputData initData) = 0;virtual SOutputData Update(SInputData turnData) = 0;virtual void SetDebugIsEnabled(bool const enableDebug) = 0;virtual char const* GetBotName() const = 0;virtual void Reset() = 0;virtual void OnMatchEnded() {}};}}
namespace ut3 {namespace bot {class CBotBase : public IBot{public:SOutputData FirstUpdate(SInputData initData) override;SOutputData Update(SInputData turnData) override;void SetDebugIsEnabled(bool const enableDebug) override { m_isDebugEnabled = enableDebug; }void Reset() override;protected:virtual SVec2 FindTurn(game::SGameState const& gameState) = 0;protected:game::SGameState m_gameState;int m_myPlayer = -1;bool m_isDebugEnabled = false;};}}
namespace mimax {namespace common {template<typename T, size_t TSize>class CStaticVector{public:static const size_t MAX_SIZE = TSize;public:CStaticVector() : m_nxtSlotIndex(0) {}CStaticVector(size_t const size) { resize(size); }CStaticVector(std::initializer_list<T> const& items){resize(items.size());std::copy(items.begin(), items.end(), begin());m_nxtSlotIndex = items.size();}inline void push_back(T const& elem) { assert(m_nxtSlotIndex < TSize); m_data[m_nxtSlotIndex++] = elem; }template<typename ... TArgs>inline T& emplace_back(TArgs&& ... args) { m_data[m_nxtSlotIndex++] = std::move(T(std::forward<TArgs>(args)...)); return back(); }inline void pop_back() { m_nxtSlotIndex--; }inline T& back() { return m_data[m_nxtSlotIndex - 1]; }inline T& front() { return *m_data; }inline T const& back() const { return m_data[m_nxtSlotIndex - 1]; }inline T const& front() const { return *m_data; }inline size_t size() const { return m_nxtSlotIndex; }inline void resize(size_t const capacity) { m_nxtSlotIndex = capacity; }inline void reserve(size_t const capacity) { assert(capacity <= TSize); }inline size_t max_capacity() const { return TSize; }inline bool is_full() const { return m_nxtSlotIndex >= TSize; }void erase_swap(T* const iter){size_t const index = std::distance(m_data, iter);std::swap(m_data[index], m_data[m_nxtSlotIndex - 1]);pop_back();}inline bool empty() const { return m_nxtSlotIndex == 0; }inline void clear() { m_nxtSlotIndex = 0; }inline T* begin() { return m_data; }inline T* end() { return m_data + m_nxtSlotIndex; }inline T const* begin() const { return m_data; }inline T const* end() const { return m_data + m_nxtSlotIndex; }inline T& operator[](size_t const index) { assert(index < m_nxtSlotIndex); return m_data[index]; }inline T const& operator[](size_t const index) const { assert(index < m_nxtSlotIndex); return m_data[index]; }private:size_t m_nxtSlotIndex = 0;T m_data[TSize];};}}
namespace ut3 {namespace game {using Turns = mimax::common::CStaticVector<SVec2, 9 * 9>;int CalculateWinner(SGameState const& gameState);int CalculateWinner(SGameBlockState const blockState);void MakeTurn(SGameState& gameState, int const worldX, int const worldY);void CollectPossibleTurns(SGameState const& gameState, Turns& turnsOut);}}
#if MIMAX_MCTS_DEBUG
#endif 
namespace ut3 {namespace bot {template<size_t TCacheSize>class CUT3MctsCachedMathHelper{public:static void Initialize();static inline float sqrt(float const val){int const intVal = (int)(val * 100.0f + 0.00001f);if (intVal >= ms_sqrtCache.size()) return ::sqrt(val);return ms_sqrtCache[intVal];}static inline float log(int const val){if (val >= ms_logCache.size()) return (float)::log(val);return ms_logCache[val];}private:static std::array<float, TCacheSize> ms_sqrtCache;static std::array<float, TCacheSize> ms_logCache;};class CUT3MctsResolver{public:CUT3MctsResolver(size_t const maxDepth);inline void GetPossibleMoves(game::SGameState const& state, game::Turns& movesOut){game::CollectPossibleTurns(state, movesOut);}inline void MakeMove(game::SGameState& state, SVec2 const move){game::MakeTurn(state, move[0], move[1]);}float Playout(game::SGameState const& state);private:size_t m_maxDepth;};using CUT3MctsMathHelper = CUT3MctsCachedMathHelper<30000>;using CUT3Mcts = mimax::dma::CMctsBase<game::SGameState, SVec2, game::Turns, CUT3MctsResolver, CUT3MctsMathHelper>;class CMCTSBot_v1 : public CBotBase{public:CMCTSBot_v1();CMCTSBot_v1(float explorationParam, size_t const maxDepth);char const* GetBotName() const override { return "CMCTSBot_v1"; }void Reset() override;void OnMatchEnded() override;protected:SVec2 FindTurn(game::SGameState const& gameState) override;
#if MIMAX_MCTS_DEBUG
public:struct SDebugStats{float m_totalNodesCnt = 0.0f;float m_totalVisitedNodesCnt = 0.0f;float m_simulationsCnt = 0.0f;};private:std::vector<SDebugStats> m_debugStatsHistory;
#endif 
private:std::vector<CUT3Mcts> m_mctsAlgos;};}}
void ReadData(ut3::SInputData& inputData){std::cin >> inputData.m_oppTurnY >> inputData.m_oppTurnX;int validActionCount;scanf("%i", &validActionCount);for (int i = 0; i < validActionCount; ++i){int x, y;scanf("%i%i", &x, &y);}}void WriteData(ut3::SOutputData const& outputData){std::cout << outputData.m_turnY << " " << outputData.m_turnX << "\n";}int main(){int const randomSeed = mimax::common::UpdateRandomSeed();std::cerr << "Random seed: " << randomSeed << "\n";std::cerr << "Hardware concurrency: " << std::thread::hardware_concurrency() << "\n";ut3::SInputData inData;ReadData(inData);ut3::bot::CMCTSBot_v1 bot;
#if RELEASE_BOT
bot.SetDebugIsEnabled(false);
#else
bot.SetDebugIsEnabled(true);
#endif
WriteData(bot.FirstUpdate(inData));while (true){ReadData(inData);{PROFILE_TIME("Update");WriteData(bot.Update(inData));}}return 0;}

#define _CRT_SECURE_NO_WARNINGS
namespace mimax {namespace dma {namespace mcts_utility {template<typename TMove>void CombineEvalResults(SMctsNodeEvalResult<TMove>& result, SMctsNodeEvalResult<TMove> const& resultToCombine){auto& resultChildrenInfo = result.m_childrenInfo;for (auto const& childInfo : resultToCombine.m_childrenInfo){auto iter = find_if(resultChildrenInfo.begin(), resultChildrenInfo.end(),[move = childInfo.m_move](auto const& info){return move == info.m_move;});if (iter != resultChildrenInfo.end()){iter->m_simulationsCount += childInfo.m_simulationsCount;}else{resultChildrenInfo.push_back(childInfo);}}}template<typename TMove>TMove GetBestMove(SMctsNodeEvalResult<TMove> const& evalResult){assert(!evalResult.m_childrenInfo.empty());return std::max_element(evalResult.m_childrenInfo.begin(), evalResult.m_childrenInfo.end(),[](auto const& lhs, auto const& rhs){return lhs.m_simulationsCount < rhs.m_simulationsCount;})->m_move;}} } } 
namespace mimax {namespace mt {class ITask{public:virtual ~ITask() = default;virtual void RunTask() = 0;virtual void StopTask() = 0;};} } 
namespace mimax {namespace dma {template<typename TMcts>class CMctsTask : public mimax::mt::ITask{public:CMctsTask(TMcts* mcts): m_mcts(mcts), m_isStopRequested(false){}void RunTask() override{while (!m_isStopRequested){m_mcts->Evaluate();}}void StopTask() override{m_isStopRequested = true;}private:bool m_isStopRequested;TMcts* m_mcts;};} } 
namespace mimax {namespace mt {class ITask;class CTasksRunner{public:void RunTasksAndWait(std::vector<ITask*> const& tasks, std::chrono::microseconds const waitingTime);private:std::vector<ITask*> m_tasks;std::vector<std::future<void>> m_futures;private:void RunTasks();void Wait(std::chrono::microseconds const time);void StopTasks();void WaitForTasksCompleted();};} } 
namespace ut3 {namespace game {struct SGameStateView{SGameStateView(SGameState const& state);void Print();char m_map[9][10];char m_globalBlockStatuses[3][4];char m_player;char m_oppLastTurnX;char m_oppLastTurnY;char m_gameWinner;unsigned char m_xBlocksCnt;unsigned char m_oBlocksCnt;unsigned char m_elementsCnt;};}}
namespace ut3 {namespace bot {using CUT3MctsTask = mimax::dma::CMctsTask<CUT3Mcts>;namespace {inline static SVec2 CalculateBestMove(std::vector<CUT3Mcts> const& mctsAlgos){mimax::dma::SMctsNodeEvalResult<SVec2> combinedResult;for (auto const& algo : mctsAlgos){mimax::dma::mcts_utility::CombineEvalResults(combinedResult, algo.GetRootEvalResult());}return mimax::dma::mcts_utility::GetBestMove(combinedResult);}inline static float CalculateScore(game::SGameState const& state, int myPlayer){int const gameWinner = GAME_STATE_GET_GAME_WINNER(state);if (gameWinner != GAME_STATE_ELEMENT_EMPTY){if (gameWinner == GAME_STATE_ELEMENT_DRAW) return 0.5f;else if (gameWinner == GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(myPlayer)) return 1.0f;else return 0.0f;}auto const globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(state);int const oppPlayer = ((myPlayer + 1) & 1);int const myBlocksCnt = (int)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, myPlayer);int const oppBlocksCnt = (int)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, oppPlayer);return (myBlocksCnt == oppBlocksCnt? 0.5f: (myBlocksCnt > oppBlocksCnt ? 1.0f : 0.0f));}
#if MIMAX_MCTS_DEBUG
inline static CMCTSBot_v1::SDebugStats CalculateDebugStats(std::vector<CUT3Mcts> const& mctsAlgos){CMCTSBot_v1::SDebugStats debugStats;for (auto const& algo : mctsAlgos){auto const& debugInfo = algo.GetDebugInfo();debugStats.m_simulationsCnt += debugInfo.m_simulationsCnt;debugStats.m_totalNodesCnt += debugInfo.m_totalNodesCnt;debugStats.m_totalVisitedNodesCnt += debugInfo.m_totalVisitedNodesCnt;}debugStats.m_simulationsCnt /= (float)mctsAlgos.size();debugStats.m_totalNodesCnt /= (float)mctsAlgos.size();debugStats.m_totalVisitedNodesCnt /= (float)mctsAlgos.size();return debugStats;}inline static CMCTSBot_v1::SDebugStats GetAverageDebugStats(std::vector<CMCTSBot_v1::SDebugStats> const& debugStatsHistory){CMCTSBot_v1::SDebugStats averageDebugStats;for (auto const& debugStats : debugStatsHistory){averageDebugStats.m_simulationsCnt += debugStats.m_simulationsCnt;averageDebugStats.m_totalNodesCnt += debugStats.m_totalNodesCnt;averageDebugStats.m_totalVisitedNodesCnt += debugStats.m_totalVisitedNodesCnt;}averageDebugStats.m_simulationsCnt /= (float)debugStatsHistory.size();averageDebugStats.m_totalNodesCnt /= (float)debugStatsHistory.size();averageDebugStats.m_totalVisitedNodesCnt /= (float)debugStatsHistory.size();return averageDebugStats;}
#endif 
} template<size_t TCacheSize>void CUT3MctsCachedMathHelper<TCacheSize>::Initialize(){static bool isInitialized = false;if (isInitialized) return;isInitialized = true;ms_sqrtCache[0] = 0.0f;for (int i = 1; i < TCacheSize; ++i){ms_sqrtCache[i] = ::sqrt((float)i / 100.0f);ms_logCache[i] = (float)::log(i);}}template<size_t TCacheSize>std::array<float, TCacheSize> CUT3MctsCachedMathHelper<TCacheSize>::ms_sqrtCache;template<size_t TCacheSize>std::array<float, TCacheSize> CUT3MctsCachedMathHelper<TCacheSize>::ms_logCache;CUT3MctsResolver::CUT3MctsResolver(size_t const maxDepth): m_maxDepth(maxDepth){}float CUT3MctsResolver::Playout(game::SGameState const& state){int myPlayer = GAME_STATE_GET_PLAYER(state);game::SGameState curState = state;game::Turns moves;for (size_t depth = 0; depth <= m_maxDepth; ++depth){game::CollectPossibleTurns(curState, moves);if (moves.empty()) break;auto const move = *mimax::common::GetRandomItem(moves.begin(), moves.end());game::MakeTurn(curState, move[0], move[1]);}return CalculateScore(curState, myPlayer);}CMCTSBot_v1::CMCTSBot_v1(): CMCTSBot_v1(1.41f, 7){}CMCTSBot_v1::CMCTSBot_v1(float explorationParam, size_t const maxDepth){CUT3MctsMathHelper::Initialize();auto const hardwareConcurrency = std::thread::hardware_concurrency();m_mctsAlgos.reserve(hardwareConcurrency);std::mt19937_64 randEngine(time(NULL));CUT3MctsResolver resolver(maxDepth);CUT3Mcts::SConfig config;config.m_nodesPoolSize = 200000;config.m_explorationParam = explorationParam;for (size_t i = 0; i < hardwareConcurrency; ++i){config.m_randomSeed = randEngine();m_mctsAlgos.emplace_back(resolver, config);}}void CMCTSBot_v1::Reset(){
#if MIMAX_MCTS_DEBUG
m_debugStatsHistory.clear();
#endif 
CBotBase::Reset();}void CMCTSBot_v1::OnMatchEnded(){
#if MIMAX_MCTS_DEBUG
auto const debugStats = GetAverageDebugStats(m_debugStatsHistory);std::cerr << "\n";std::cerr << "Simulations count: " << debugStats.m_simulationsCnt << "\n";std::cerr << "Nodes count: " << debugStats.m_totalNodesCnt << "\n";std::cerr << "Visited nodes count: " << debugStats.m_totalVisitedNodesCnt << "\n";
#endif 
}SVec2 CMCTSBot_v1::FindTurn(game::SGameState const& gameState){if (GAME_STATE_ELEMENTS_COUNT(gameState) == 0){return { 4, 4 };}std::vector<CUT3MctsTask> mctsTasks;std::vector<mimax::mt::ITask*> tasksToRun;mctsTasks.reserve(m_mctsAlgos.size());tasksToRun.reserve(m_mctsAlgos.size());for (size_t i = 0; i < m_mctsAlgos.size(); ++i){m_mctsAlgos[i].Reset(gameState);auto& mctsTask = mctsTasks.emplace_back(&(m_mctsAlgos[i]));tasksToRun.push_back(&mctsTask);}mimax::mt::CTasksRunner().RunTasksAndWait(tasksToRun, std::chrono::milliseconds(90));if (m_isDebugEnabled){
#if MIMAX_MCTS_DEBUG
std::cerr << m_mctsAlgos.front().GetDebugInfo();
#endif 
}
#if MIMAX_MCTS_DEBUG
m_debugStatsHistory.push_back(CalculateDebugStats(m_mctsAlgos));
#endif 
return CalculateBestMove(m_mctsAlgos);}}}
namespace{constexpr size_t WINNER_MASKS_COUNT = 8;constexpr unsigned int WINNER_MASKS[WINNER_MASKS_COUNT] = {21, 1344, 86016, 4161, 16644, 66576, 65793, 4368};
#   define GET_WINNER_MASK_FOR_PLAYER(mask, playerIndex) ((mask) << (playerIndex))
inline void AddAvailableTurns(ut3::game::Turns& turnsOut, ut3::game::SGameBlockState block, int const blockX, int const blockY){for (int x = 0; x < 3; ++x){for (int y = 0; y < 3; ++y){if (GAME_BLOCK_STATE_GET_ELEMENT_BY_POS(block, x, y) == GAME_STATE_ELEMENT_EMPTY){turnsOut.push_back({x + 3 * blockX, y + 3 * blockY});}}}}}namespace ut3 {namespace game {int CalculateWinner(SGameState const& gameState){auto const globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(gameState);int const blockWinner = CalculateWinner(globalBlock);if (blockWinner == GAME_STATE_ELEMENT_DRAW){int const xCnt = GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 0);int const oCnt = GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 1);if (xCnt == oCnt) return GAME_STATE_ELEMENT_DRAW;return (xCnt > oCnt) ? GAME_STATE_ELEMENT_X : GAME_STATE_ELEMENT_O;}return blockWinner;}int CalculateWinner(SGameBlockState const blockState){for (int i = 0; i < WINNER_MASKS_COUNT; ++i){unsigned int const mask0 = GET_WINNER_MASK_FOR_PLAYER(WINNER_MASKS[i], 0);unsigned int const mask1 = GET_WINNER_MASK_FOR_PLAYER(WINNER_MASKS[i], 1);if ((mask0 & blockState.m_data) == mask0){if (UT3_COUNT_BITS_32(mask1 & blockState.m_data) == 0)return GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(0);}else if ((mask1 & blockState.m_data) == mask1){if (UT3_COUNT_BITS_32(mask0 & blockState.m_data) == 0)return GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(1);}}for (int i = 0; i < 9; ++i){if (GAME_BLOCK_STATE_GET_ELEMENT(blockState, i) == GAME_STATE_ELEMENT_EMPTY)return GAME_STATE_ELEMENT_EMPTY;}return GAME_STATE_ELEMENT_DRAW;}void MakeTurn(SGameState& gameState, int const worldX, int const worldY){int const blockX = worldX / 3;int const blockY = worldY / 3;int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);auto block = GetBlockState(gameState, blockIndex);int const localX = worldX - 3 * blockX;int const localY = worldY - 3 * blockY;int const playerIndex = GAME_STATE_GET_PLAYER(gameState);GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(block, localX, localY, GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(playerIndex));SetBlockState(gameState, blockIndex, block);GAME_STATE_SET_PLAYER(gameState, ((playerIndex + 1) & 1));GAME_STATE_SET_LAST_OPP_TURN_X(gameState, worldX);GAME_STATE_SET_LAST_OPP_TURN_Y(gameState, worldY);int const blockWinner = CalculateWinner(block);if (blockWinner != GAME_STATE_ELEMENT_EMPTY){GAME_STATE_SET_BLOCK_STATUS(gameState, blockIndex, blockWinner);int const gameWinner = CalculateWinner(gameState);GAME_STATE_SET_GAME_WINNER(gameState, gameWinner);}}void CollectPossibleTurns(SGameState const& gameState, Turns& turnsOut){turnsOut.clear();if (GAME_STATE_GET_GAME_WINNER(gameState) != GAME_STATE_ELEMENT_EMPTY)return;bool findAllTurns = true;if (GAME_STATE_IS_VALID_LAST_OPP_TURN(gameState)){int const blockX = GAME_STATE_GET_LAST_OPP_TURN_X(gameState) % 3;int const blockY = GAME_STATE_GET_LAST_OPP_TURN_Y(gameState) % 3;int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);if (GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) == GAME_STATE_ELEMENT_EMPTY){findAllTurns = false;auto const block = GetBlockState(gameState, blockIndex);AddAvailableTurns(turnsOut, block, blockX, blockY);}}if (findAllTurns){for (int blockX = 0; blockX < 3; ++blockX){for (int blockY = 0; blockY < 3; ++blockY){int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);if (GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) == GAME_STATE_ELEMENT_EMPTY){auto const block = GetBlockState(gameState, blockIndex);AddAvailableTurns(turnsOut, block, blockX, blockY);}}}}}}}
namespace ut3 {namespace bot {SOutputData CBotBase::FirstUpdate(SInputData initData){if (initData.m_oppTurnX >= 0){m_myPlayer = 1;return Update(initData);}else{m_myPlayer = 0;auto const turn = FindTurn(m_gameState);game::MakeTurn(m_gameState, turn[0], turn[1]);return { turn[0], turn[1] };}}SOutputData CBotBase::Update(SInputData turnData){game::MakeTurn(m_gameState, turnData.m_oppTurnX, turnData.m_oppTurnY);if (m_isDebugEnabled){game::SGameStateView(m_gameState).Print();}auto const turn = FindTurn(m_gameState);game::MakeTurn(m_gameState, turn[0], turn[1]);return { turn[0], turn[1] };}void CBotBase::Reset(){m_gameState = game::SGameState();m_myPlayer = -1;m_isDebugEnabled = false;}}}
namespace ut3 {namespace game {char ConvertGameStateElemToChar(int const elem);int ConvertCharToGameStateElem(char const c);SGameState CreateGameState(char const map[9][10], SVec2 const oppLastTurn);} } 
namespace ut3 {namespace game {SGameStateView::SGameStateView(SGameState const& state){for (int blockX = 0; blockX < 3; ++blockX){for (int blockY = 0; blockY < 3; ++blockY){int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);m_globalBlockStatuses[blockY][blockX] = ConvertGameStateElemToChar(GAME_STATE_GET_BLOCK_STATUS(state, blockIndex));auto const block = GetBlockState(state, blockIndex);for (int x = 0; x < 3; ++x){for (int y = 0; y < 3; ++y){m_map[y + blockY * 3][x + blockX * 3] = ConvertGameStateElemToChar(GAME_BLOCK_STATE_GET_ELEMENT_BY_POS(block, x, y));}}}}for (int r = 0; r < 3; ++r){m_globalBlockStatuses[r][3] = '\0';}for (int r = 0; r < 9; ++r){m_map[r][9] = '\0';}m_player = ConvertGameStateElemToChar(GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(GAME_STATE_GET_PLAYER(state)));m_oppLastTurnX = GAME_STATE_GET_LAST_OPP_TURN_X(state);m_oppLastTurnY = GAME_STATE_GET_LAST_OPP_TURN_Y(state);m_gameWinner = ConvertGameStateElemToChar(GAME_STATE_GET_GAME_WINNER(state));auto const globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(state);m_xBlocksCnt = (unsigned char)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 0);m_oBlocksCnt = (unsigned char)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 1);m_elementsCnt = (unsigned char)GAME_STATE_ELEMENTS_COUNT(state);}void SGameStateView::Print(){for (int r = 0; r < 9; ++r){for (int c = 0; c < 9; ++c){std::cerr << m_map[r][c];if (c == 2 || c == 5) std::cerr << " ";}std::cerr << "\n";if (r == 2 || r == 5) std::cerr << "\n";}for (int r = 0; r < 3; ++r){std::cerr << m_globalBlockStatuses[r] << "\n";}std::cerr << "Player: " << m_player << "\n";std::cerr << "Opp last turn: " << (int)m_oppLastTurnY << ", " << (int)m_oppLastTurnX << "\n";std::cerr << "Game winner: " << m_gameWinner << "\n";std::cerr << "X blocks count: " << (int)m_xBlocksCnt << "\n";std::cerr << "O blocks count: " << (int)m_oBlocksCnt << "\n";std::cerr << "Elements count: " << (int)m_elementsCnt << "\n";}}}
namespace ut3 {namespace game {char ConvertGameStateElemToChar(int const elem){if (elem == GAME_STATE_ELEMENT_EMPTY) return '-';else if (elem == GAME_STATE_ELEMENT_X) return 'X';else if (elem == GAME_STATE_ELEMENT_O) return 'O';return 'D';}int ConvertCharToGameStateElem(char const c){if (c == '-') return GAME_STATE_ELEMENT_EMPTY;else if (c == 'X') return GAME_STATE_ELEMENT_X;else if (c == 'O') return GAME_STATE_ELEMENT_O;return GAME_STATE_ELEMENT_DRAW;}SGameState CreateGameState(char const map[9][10], SVec2 const oppLastTurn){SGameState gameState;size_t const elementsCount = GAME_STATE_ELEMENTS_COUNT(gameState);size_t const palyerIndex = (elementsCount & 1) ? 1 : 0;GAME_STATE_SET_PLAYER(gameState, palyerIndex);GAME_STATE_SET_LAST_OPP_TURN_X(gameState, oppLastTurn[0]);GAME_STATE_SET_LAST_OPP_TURN_Y(gameState, oppLastTurn[1]);for (int blockX = 0; blockX < 3; ++blockX){for (int blockY = 0; blockY < 3; ++blockY){SGameBlockState blockState;for (int x = 0; x < 3; ++x){for (int y = 0; y < 3; ++y){int const elem = ConvertCharToGameStateElem(map[y + 3 * blockY][x + 3 * blockX]);GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, x, y, elem);}}int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);SetBlockState(gameState, blockIndex, blockState);int const blockWinner = CalculateWinner(blockState);GAME_STATE_SET_BLOCK_STATUS(gameState, blockIndex, blockWinner);}}int const gameWinner = CalculateWinner(gameState);GAME_STATE_SET_GAME_WINNER(gameState, gameWinner);return gameState;}} } 

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS


#if MIMAX_MCTS_DEBUG
namespace mimax {namespace dma {std::ostream& operator<<(std::ostream& o, SMctsDebugInfo const& debugInfo){o << "Max depth: " << debugInfo.GetMaxDepth() << "\n";o << "Simulations count: " << debugInfo.m_simulationsCnt << "\n";o << "Nodes count " << debugInfo.m_totalNodesCnt << ": \n";for (size_t depth = 1; depth <= debugInfo.m_nodesCnt.size(); ++depth){o << depth << ": " << debugInfo.m_nodesCnt[depth] << "\n";}o << "Visited nodes count " << debugInfo.m_totalVisitedNodesCnt << ": \n";for (size_t depth = 1; depth <= debugInfo.m_visitedNodesCnt.size(); ++depth){o << depth << ": " << debugInfo.m_visitedNodesCnt[depth] << "\n";}return o;}} } 
#endif 

namespace mimax {namespace mt {using namespace std;void CTasksRunner::RunTasksAndWait(vector<ITask*> const& tasks, chrono::microseconds const waitingTime){m_tasks = tasks;m_futures.clear();RunTasks();Wait(waitingTime);StopTasks();WaitForTasksCompleted();}void CTasksRunner::RunTasks(){for (auto task : m_tasks){auto future = async([task](){task->RunTask();});m_futures.emplace_back(move(future));}}void CTasksRunner::Wait(chrono::microseconds const time){auto const startTime = chrono::high_resolution_clock::now();auto const endTime = startTime + time;while (chrono::high_resolution_clock::now() < endTime){this_thread::yield();}}void CTasksRunner::StopTasks(){for (auto task : m_tasks){task->StopTask();}}void CTasksRunner::WaitForTasksCompleted(){for (auto& future : m_futures){future.wait();}}} } 
using namespace std;namespace{static int gNestedLevel = 0;}namespace mimax {namespace common {CTimeProfiler::CTimeProfiler(char const* const name): m_name(name), m_startTime(chrono::high_resolution_clock::now()){++gNestedLevel;}CTimeProfiler::~CTimeProfiler(){auto const end = chrono::high_resolution_clock::now();double const executionTime = (double)chrono::duration_cast<chrono::nanoseconds>(end - m_startTime).count();string const tabs(gNestedLevel, '-');cerr.precision(3);cerr << "[Profiler] " << tabs << m_name << ": " << executionTime / 1000000.0 << fixed << " ms" << endl;--gNestedLevel;}}}
#include <time.h>
namespace mimax {namespace common {unsigned int GenerateRandomSeed(){return (unsigned int)time(nullptr);}unsigned int UpdateRandomSeed(unsigned int seed){if (seed == 0)seed = GenerateRandomSeed();srand(seed);return seed;}unsigned int GetRandomUInt32(unsigned int const minVal, unsigned int const maxVal){assert(minVal < maxVal);unsigned int const range = maxVal - minVal;return rand() % range + minVal;}}}

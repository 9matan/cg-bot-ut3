#include "BotHeader.h"

#include "mimax/common/Random.h"
#include "mimax/common/Profiler.h"
#include "bot-core/bot/MCTSBot_v1.h"

void ReadData(ut3::SInputData& inputData)
{
	std::cin >> inputData.m_oppTurnY >> inputData.m_oppTurnX;

	int validActionCount;
	scanf("%i", &validActionCount);
	for (int i = 0; i < validActionCount; ++i)
	{
		int x, y;
		scanf("%i%i", &x, &y);
	}
}

void WriteData(ut3::SOutputData const& outputData)
{
	std::cout << outputData.m_turnY << " " << outputData.m_turnX << "\n";
}

int main()
{
	int const randomSeed = mimax::common::UpdateRandomSeed();
	std::cerr << "Random seed: " << randomSeed << "\n";
	std::cerr << "Hardware concurrency: " << std::thread::hardware_concurrency() << "\n";
#ifdef _ITERATOR_DEBUG_LEVEL
	std::cerr << "_ITERATOR_DEBUG_LEVEL: " << _ITERATOR_DEBUG_LEVEL << "\n";
#endif
#ifdef NDEBUG
	std::cerr << "NDEBUG" << "\n";
#endif
#ifdef _DEBUG
	std::cerr << "_DEBUG" << "\n";
#endif
#ifdef _GLIBCXX_DEBUG
	std::cerr << "_GLIBCXX_DEBUG" << "\n";
#endif
#ifdef _GLIBCXX_INLINE_VERSION
	std::cerr << "_GLIBCXX_INLINE_VERSION: " << _GLIBCXX_INLINE_VERSION << "\n";
#endif

	ut3::SInputData inData;

	ReadData(inData);
	ut3::bot::CMCTSBot_v1 bot(0.35f, 60);
#if RELEASE_BOT
	bot.SetDebugIsEnabled(false);
#else
	bot.SetDebugIsEnabled(true);
#endif
	WriteData(bot.FirstUpdate(inData));

	while (true)
	{
		ReadData(inData);
		{
			PROFILE_TIME("Update");
			WriteData(bot.Update(inData));
		}
	}

	return 0;
}
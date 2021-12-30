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

	ut3::SInputData inData;

	ReadData(inData);
	ut3::bot::CMCTSBot_v1 bot;
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
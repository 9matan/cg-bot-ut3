#include "Optimizations.h"

#include <iostream>

#include "mimax/common/Profiler.h"
#include "bot-core/bot/MinimaxBot.h"

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
	ut3::SInputData inData;

	ReadData(inData);
	ut3::CMinimaxBot bot(inData);
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
#pragma once
#include <iostream>
#include <vector>
#include <math.h>

#include <chrono>
#include <windows.h>
#include <psapi.h>
using namespace std;

class TimeMemory
{
private:
	PROCESS_MEMORY_COUNTERS pmc;
	chrono::steady_clock::time_point start;
	chrono::steady_clock::time_point end;
	chrono::duration<double> duration;
public:

	void PrintMemoryUsage()
	{
		GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
		std::cout << "Using memory: " << pmc.WorkingSetSize / (1024 * 1024) << " MiB\n";
	}
	void StartTime()
	{
		start = chrono::high_resolution_clock::now();
	}
	void EndTime()
	{
		end = chrono::high_resolution_clock::now();
		duration = end - start;
		cout << endl << "Время выполнения: " << duration.count() << " секунд" << endl;
	}
};


float Area(float a, float b, float c);

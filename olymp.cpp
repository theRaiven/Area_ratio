#include <iostream>
#include <vector>
#include <math.h>

#include <chrono>
#include <windows.h>
#include <psapi.h>
using namespace std;

float CountSquare(vector<pair<int, int>> coordinates, int n, float x_second, float x_min, float x_max, int P, int Q);
int CheckRatio(float Square_P, float Square_Q, int P, int Q);
float GetNewSecondary(float max, float min);

void PrintMemoryUsage()
 {
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	std::cout << "Using memory: " << pmc.WorkingSetSize / (1024 * 1024) << " MiB\n";
}

int main()
{
	PrintMemoryUsage();
	setlocale(LC_ALL, "rus");

	int number_vertices, P, Q;
	float x_min, x_max; float x_second;
	float result;

	cin >> number_vertices >> P >> Q;

	vector<pair<int, int>> coordinates_vertices(number_vertices+1, pair<int, int>(0, 0));

	cin >> coordinates_vertices[0].first >> coordinates_vertices[0].second;

	coordinates_vertices[number_vertices].first = coordinates_vertices[0].first;
	coordinates_vertices[number_vertices].second = coordinates_vertices[0].second;

	x_min = coordinates_vertices[0].first;
	x_max = x_min;

	for (int i = 1; i < number_vertices; i++)
	{
		cin >> coordinates_vertices[i].first >> coordinates_vertices[i].second;
		if (coordinates_vertices[i].first > x_max)
		{
			x_max = coordinates_vertices[i].first;
		}
		if (coordinates_vertices[i].first < x_min)
		{
			x_min = coordinates_vertices[i].first;
		}
	}
	auto start{ chrono::high_resolution_clock::now() };
	if (P == 0)
	{
		cout << x_min;
		return 0;
	}
	if (Q == 0)
	{
		cout << x_max;
		return 0;
	}
	x_second = GetNewSecondary(x_max, x_min);

	result = (CountSquare(coordinates_vertices, number_vertices, x_second, x_min, x_max, P, Q));
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(3);
	cout << result;

	auto end{ chrono::high_resolution_clock::now() };
	chrono::duration<double> duration = end - start;
	cout << endl << "Время выполнения: " << duration.count() << " секунд" << endl;
	PrintMemoryUsage();
	return 0;
}

float CountSquare(vector<pair<int, int>> coordinates, int n, float x_second, float x_min, float x_max, int P, int Q)
{
	float sum_P{ 0 }, sum_Q{ 0 };
	char ratio{ 0 };
	const float eps{ 1e-4 };

	float y_lower{ 0 };
	float y_upper{ 0 };
	float direct_section;
	while (true)
	{
		sum_P = 0;
		sum_Q = 0;

		for (int i = 0; i < n; i++)
		{
			int x1{ coordinates[i].first }, y1{ coordinates[i].second };
			int x2{ coordinates[i + 1].first }, y2{ coordinates[i + 1].second };
			
			if ((x1 < x_second && x2 > x_second) || (x1 > x_second && x2 < x_second))
			{

				if (x1 < x_second)
				{
					y_lower = y1 + (y2 - y1) * (x_second - x1) / (float)(x2 - x1);

					sum_P += x1 * y_lower - x_second * y1;
					sum_Q += x_second * y2 - x2 * y_lower;
				}
				else
				{
					y_upper = y1 + (y2 - y1) * (x_second - x1) / (float)(x2 - x1);
					sum_Q += x1 * y_upper - x_second * y1;
					sum_P += x_second * y2 - x2 * y_upper;
				}
			}
			else if (fabs(x1 - x_second) < eps || fabs(x2 - x_second) < eps)
			{
				if (fabs(x1 - x_second) < eps && fabs(x2 - x_second) < eps)
				{
					continue;
				}
				else if (x1 == x_second)
				{
					if (x2 > x_second)
					{
						y_upper = y1 + (y2 - y1) * (x_second - x1) / (float)(x2 - x1);
						sum_Q += x_second * y2 - x2 * y1;
					}
					else
					{
						y_lower = y1 + (y2 - y1) * (x_second - x1) / (float)(x2 - x1);
						sum_P += x1 * y2 - x2 * y1;
					}
				}
				else if (x2 == x_second)
				{
					if (x1 > x_second)
					{
						y_upper = y1 + (y2 - y1) * (x_second - x1) / (float)(x2 - x1);
						sum_Q += x1 * y2 - x_second * y1;
					}
					else
					{
						y_lower = y1 + (y2 - y1) * (x_second - x1) / (float)(x2 - x1);
						sum_P += x1 * y2 - x_second * y1;
					}
				}
			}
			else
			{
				if (x1 < x_second)
				{
					sum_P += x1 * y2 - x2 * y1;
				}
				else
				{
					sum_Q += x1 * y2 - x2 * y1;
				}
			}
			
		}
		direct_section = x_second * y_upper - x_second * y_lower;
		sum_P += direct_section;
		sum_Q -= direct_section;

		sum_P = fabs(sum_P)/2;
		sum_Q = fabs(sum_Q)/2;
		ratio = CheckRatio(sum_P, sum_Q, P, Q);
		if (ratio == -1)
		{
			x_min = x_second;
			x_second = GetNewSecondary(x_second, x_max);
		}
		if (ratio == 1)
		{
			x_max = x_second;
			x_second = GetNewSecondary(x_min, x_second);
		}
		if (ratio == 0)
		{
			return x_second;
		}
		if (x_max - x_min < eps)
		{
			return x_second; 
		}
	}

}
int CheckRatio(float Square_P, float Square_Q, int P, int Q)
{
	return (Square_P * Q == Square_Q * P) ? 0 : (Square_P * Q > Square_Q * P) ? 1 : -1;
}
float GetNewSecondary(float max, float min)
{
	return (max + min) / 2.0;
}
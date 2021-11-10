#include "OstreamLogger.h"
#include <thread>
#include <chrono>
#include <random>

std::ofstream out("log.txt", std::ios_base::app);

void LogTestConsole(int i)
{	
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(1.0, 10.0);
	OStreamLogger aux(std::cout);
	LogMessage b;
	///std::this_thread::sleep_for(std::chrono::seconds((int)dist(mt)));
	b << "done " <<i;
	aux.Log(b, LogSeverity::Crititical);
}

void LogTestFile(int i)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(1.0, 10.0);
	OStreamLogger aux(out);
	LogMessage b;
	///std::this_thread::sleep_for(std::chrono::seconds((int)dist(mt)));
	b << "done " << i;
	aux.Log(b, LogSeverity::Crititical);

}

int main()
{
	std::vector<std::thread>threads1;
	threads1.resize(10);
	for (int i = 0; i < 10; ++i)
	{
		threads1[i] = std::thread(LogTestConsole, i);
	}

	for (int i = 0; i < 10; ++i)
	{
		threads1[i].join();
	}

	std::cout << "\n\n\n";

	std::vector<std::thread>threads2;
	threads2.resize(10);
	for (int i = 0; i < 10; ++i)
	{
		threads2[i] = std::thread(LogTestFile,i);
	}

	for (int i = 0; i < 10; ++i)
	{
		threads2[i].join();
	}


	std::cout << "\n\n\n";


	int a = 5;
	const std::string name = "John";
	std::ofstream out("log.txt", std::ios_base::app);
	OStreamLogger test(out);
	LogMessage b;
	b << a<< " " <<name;
	test.SetPriority(LogSeverity::Trace);

	test.Log(b, LogSeverity::Trace);
	test.Log(b, LogSeverity::Debug);
	test.Log(b, LogSeverity::Info);
	test.Log(b, LogSeverity::Warn);
	test.Log(b, LogSeverity::Error);
	test.Log(b, LogSeverity::Crititical);

	return 0;
}
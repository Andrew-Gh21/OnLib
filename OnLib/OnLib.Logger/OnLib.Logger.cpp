#include "OstreamLogger.h"
#include <thread>
#include <chrono>
#include <random>

void LogTest(int i)
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

int main()
{
	std::vector<std::thread>threads;
	threads.resize(10);
	for (int i = 0; i < 10; ++i)
	{
		threads[i] = std::thread(LogTest, i);
	}

	for (int i = 0; i < 10; ++i)
	{
		threads[i].join();
	}

	std::cout << "\n\n\n";

	int a = 5;
	const std::string name = "John";
	std::ofstream out("log.txt", std::ios_base::app);
	OStreamLogger test(out);
	LogMessage b;
	b << a<< name;
	test.setPriority(LogSeverity::Trace);

	test.Log(b, LogSeverity::Trace);
	test.Log(b, LogSeverity::Debug);
	test.Log(b, LogSeverity::Info);
	test.Log(b, LogSeverity::Warn);
	test.Log(b, LogSeverity::Error);
	test.Log(b, LogSeverity::Crititical);

	out.close();

	return 0;
}
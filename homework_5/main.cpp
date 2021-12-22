#include "parallel_scheduler.h"

void f1(void* arg)
{
	std::cout << "f1 called." << std::endl;
}

void f2(void* arg)
{
	std::cout << "f2 called." << std::endl;
}

void f3(void* arg)
{
	std::cout << "f3 called." << std::endl;
}

void f4(void* arg)
{
	std::cout << "f4 called." << std::endl;
}

int main()
{
	int n = 4;
	ParallelScheduler p_s(n);
	p_s.run(f1, nullptr);
	p_s.run(f2, nullptr);
	p_s.run(f3, nullptr);
	p_s.run(f4, nullptr);

	return 0;
}

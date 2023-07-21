#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <iomanip>
#include <stdlib.h>
#include <exception>

using namespace std::chrono_literals;

void foo(const int th_num) {
	auto start = std::chrono::steady_clock::now();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

#pragma warning(push)
#pragma warning(disable:4244)
	try {
		std::srand(time(NULL));
		int rand = std::rand();
		if ((rand % 2) == 0) throw std::exception();
	}
	catch (...) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	}
#pragma warning(pop)

	std::cout << "Номер потока: " << th_num <<
		"\nИдентификатор потока: " << std::this_thread::get_id() <<
		'\n' << std::setfill('#') << std::setw(th_num) << '#' << std::endl;

	auto end = std::chrono::steady_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << delta.count() << "μs" << std::endl;

	std::this_thread::sleep_for(800ms);
	std::system("cls");
}

int main() {
	const int length = 10;

	for (int i = 0; i < length; ++i)
	{
		std::thread th(foo, i + 1);
		th.join();
	}

	return 0;
}
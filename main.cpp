#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <iomanip>
#include <vector>
#include <random>
#include <mutex>

std::mutex m;

using namespace std::chrono_literals;

void foo(const int th_num) {
	std::lock_guard<std::mutex> lg(m);
	auto start = std::chrono::steady_clock::now();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 5);

	for(int i = 0; i < 55; i += dist(gen)){
		std::cout << '\r' << th_num << '\t' << std::this_thread::get_id() << "\t[";
		if (i > 50) i = 50;

		const size_t size = 1'000'000;
		std::vector<int> v1(size), v2(size);
		std::generate(v1.begin(), v1.end(), [&]() {
			return dist(gen);
			});
		std::generate(v2.begin(), v2.end(), [&]() {
			return dist(gen);
			});

		std::vector<int> temp = v1;
		v1 = v2;
		v2 = temp;
		
		std::cout << std::setfill('#') << std::setw(i) << '#' << std::setfill(' ') << std::setw(52 - i) << "] " << i * 2 << '%';
		std::cout.flush();
		std::this_thread::sleep_for(200ms);
		if (i == 50) break;
	}

	auto end = std::chrono::steady_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << '\t' << delta.count() << "ms" << std::endl;
}

int main() {
	const int qty = 12;
	std::vector<std::thread> thds;

	std::cout << '#' << "\tid" << "\t\t" << "\tProgress Bar\t\t\t\t\t" << "Time" << std::endl;

	for (int i = 0; i < qty; ++i) {
		thds.push_back(std::thread(foo, i));
	}

	for (auto& th : thds) {
		th.join();
	}

	return 0;
}
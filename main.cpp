#include <iostream>
#include <thread>
#include <iomanip>
#include <vector>
#include <random>
#include <mutex>

#include "Timer.h"

using namespace std::chrono_literals;

std::mutex m;

void foo(const int th_num) {
	auto start = std::chrono::steady_clock::now();
	consol_parameter con;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(1, 5);

	const int length = 50;
	m.lock();
	con.SetPosition(0, th_num + 1);
	std::cout << th_num << '\t' << std::this_thread::get_id() << "\t[" <<
		std::setfill(' ') << std::setw(length + 3) << "] " << std::flush;
	m.unlock();

	const int pos(17);
	int shift(0);

	for(int i = 0; i < (length + 5); i += dist(gen)){
		if (i > length) i = length;

		const size_t size = 3'000'000;
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
		
		m.lock();
		try {
			if (dist(gen) == 3) throw std::exception();
		}
		catch (...) {
			con.SetColor(4, 0);
		}

		con.SetPosition(pos + shift, th_num + 1);
		std::cout << std::setfill('=') << std::setw(i - shift + 1) << '>' << std::flush;
		shift = i;
		con.SetPosition(length + 20, th_num + 1);
		con.SetColor(7, 0);
		std::cout <<  i * 2 << '%' << std::flush;
		m.unlock();

		if (i == length) break;
	}

	auto end = std::chrono::steady_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::lock_guard<std::mutex> lk(m);
	std::cout << '\t' << delta.count() << "ms" << std::flush;
}

int main() {
	const int qty = 8;
	std::vector<std::thread> thds;

	std::cout << '#' << "\tid" << "\t\t" << "\tProgress Bar\t\t\t\t\t" << "Time" << std::endl;

	for (int i = 0; i < qty; ++i) {
		thds.push_back(std::thread(foo, i));
	}

	for (auto& th : thds) {
		th.join();
	}

	consol_parameter con;
	con.SetColor(7, 0);
	con.SetPosition(0, qty + 1);

	return 0;
}
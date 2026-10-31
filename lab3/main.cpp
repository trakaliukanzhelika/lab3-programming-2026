// Trakaliuk Anzhelika K-18
// Variant 9

#include <iostream>
#include <syncstream>
#include <thread>
#include <latch>
#include <chrono>
#include <windows.h>

// Simulates one action and reports
void f(char x, int i) {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::osyncstream(std::cout) << "З набору " << x << " виконано дію " << i << ".\n";
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	std::latch a_done(4), b_done(6), c_done(4), d_done(5), e_done(8),
		g_done(5), h_done(7), i_done(4), j_done(5);

	auto run = [](char x, int i, std::latch& done) {f(x, i); done.count_down();};

	std::osyncstream(std::cout) << "Обчислення розпочато.\n";
	auto start = std::chrono::steady_clock::now();
	{
		// Thread 1
		std::jthread t1([&] {
			run('a', 1, a_done);
			run('a', 4, a_done);
			run('b', 3, b_done);
			run('b', 6, b_done);
			a_done.wait();
			run('c', 3, c_done);
			run('d', 2, d_done);
			run('d', 5, d_done);
			b_done.wait();
			run('e', 3, e_done);
			run('e', 6, e_done);
			run('g', 1, g_done);
			run('g', 4, g_done);
			d_done.wait();
			e_done.wait();
			run('h', 2, h_done);
			run('h', 5, h_done);
			g_done.wait();
			run('i', 1, i_done);
			run('i', 4, i_done);
			c_done.wait();
			run('j', 3, j_done);
			h_done.wait();
			i_done.wait();
			j_done.wait();
			f('k', 1);
			f('k', 4);
			f('k', 7);
			});
		// Thread 2
		std::jthread t2([&] {
			run('a', 2, a_done);
			run('b', 1, b_done);
			run('b', 4, b_done);
			a_done.wait();
			run('c', 1, c_done);
			run('c', 4, c_done);
			run('d', 3, d_done);
			b_done.wait();
			run('e', 1, e_done);
			run('e', 4, e_done);
			run('e', 7, e_done);
			run('g', 2, g_done);
			run('g', 5, g_done);
			d_done.wait();
			e_done.wait();
			run('h', 3, h_done);
			run('h', 6, h_done);
			g_done.wait();
			run('i', 2, i_done);
			c_done.wait();
			run('j', 1, j_done);
			run('j', 4, j_done);
			h_done.wait();
			i_done.wait();
			j_done.wait();
			f('k', 2);
			f('k', 5);
			f('k', 8);
			});
		// Thread 3
		std::jthread t3([&] {
			run('a', 3, a_done);
			run('b', 2, b_done);
			run('b', 5, b_done);
			a_done.wait();
			run('c', 2, c_done);
			run('d', 1, d_done);
			run('d', 4, d_done);
			b_done.wait();
			run('e', 2, e_done);
			run('e', 5, e_done);
			run('e', 8, e_done);
			run('g', 3, g_done);
			d_done.wait();
			e_done.wait();
			run('h', 1, h_done);
			run('h', 4, h_done);
			run('h', 7, h_done);
			g_done.wait();
			run('i', 3, i_done);
			c_done.wait();
			run('j', 2, j_done);
			run('j', 5, j_done);
			h_done.wait();
			i_done.wait();
			j_done.wait();
			f('k', 3);
			f('k', 6);
			});
	}
	double sec = std::chrono::duration<double>(
		std::chrono::steady_clock::now() - start).count();
	std::osyncstream(std::cout) << "Обчислення завершено. Тривалість обчислення "
		<< sec << " секунд.\n";

	return 0;
}
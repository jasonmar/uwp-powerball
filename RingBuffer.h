#pragma once

#include <vector>
#include <stdint.h>

using namespace std;

namespace PowerBall
{

class RingBuffer
{
private:
	vector<uint64_t> data;
	size_t limit = 0;
	size_t i = 0;
	size_t j = 0;
public:
	RingBuffer();
	RingBuffer(const size_t n);
	void push(const uint64_t x);
	size_t pop();
	void reset();
};

}
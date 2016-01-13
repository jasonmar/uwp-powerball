#pragma once
#include "RingBuffer.h"

#include <stdint.h>
#include <vector>

using namespace std;
using namespace PowerBall;

namespace PowerBall
{

class BitSet
{
private:
	vector<uint64_t> data;
	RingBuffer rb;
	size_t len; // bit count
	size_t k; // container size
public:
	BitSet(const uint64_t n);
	void set(const uint64_t val);
	void unset(const uint64_t val);
	size_t available;
	size_t pop();
	void reset();
};

}
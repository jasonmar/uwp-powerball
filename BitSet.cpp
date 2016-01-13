#include "pch.h"
#include "BitSet.h"

#include <vector>
#include <stdint.h>

using namespace std;
using namespace PowerBall;

namespace PowerBall
{

BitSet::BitSet(const uint64_t n)
{
	len = n;
	k = ((n - 1) >> 6) + 1;
	available = n;
	data = vector<uint64_t>(k);
	rb = RingBuffer(1024);
}

void BitSet::set(const uint64_t val)
{
	size_t j = val >> 6;
	uint64_t x = data[j];
	uint64_t bit = 0x0000000000000001ui64 << (val & 0x3F); // shift 1 by val modulo 64 (implemented as & 63) to get single bit mask
	if ((x & bit) != bit)
	{
		x |= bit; // set the bit
		rb.push(val); // log to ring buffer
		--available;
	}
	// if (x == 0xffffffffffffffffui64) {} // bytes filled
	data[j] = x; //leave bounds checking to vector
}

void BitSet::unset(const uint64_t val)
{
	size_t j = val >> 6;
	uint64_t x = data[j];
	uint64_t mask = 0x0000000000000001ui64 << (val & 0x3F); // bit mask
	uint64_t umask = mask ^ mask; // XOR for unset mask
	x &= umask; // unset the bit
	data[j] = x; // leave bounds checking to vector
}

size_t BitSet::pop()
{
	return rb.pop();
}

void BitSet::reset()
{
	rb.reset();
}

}
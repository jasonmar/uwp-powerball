#include "pch.h"
#include "RingBuffer.h"

#include <vector>
#include <stdint.h>

using namespace std;

namespace PowerBall
{

RingBuffer::RingBuffer() {}

RingBuffer::RingBuffer(const size_t n)
{
	data = vector<uint64_t>(n);
	limit = n - 1;
}

void RingBuffer::push(const uint64_t x)
{
	data[i] = x;
	if (i == limit)
	{
		i = 0;
	}
	else
	{
		++i;
	}
	j = i;
}

size_t RingBuffer::pop()
{
	if (j == 0)
	{
		j = limit;
	}
	else
	{
		--j;
	}
	return data[j];
}

void RingBuffer::reset()
{
	j = i;
}

}
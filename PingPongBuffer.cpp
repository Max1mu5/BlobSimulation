#include "PingPongBuffer.h"

void PingPongBuffer::allocate(std::vector<float> data) {
	bufferObject[0].allocate(data, GL_DYNAMIC_DRAW);
	bufferObject[1].allocate(data, GL_DYNAMIC_DRAW);

	src = &bufferObject[0];
	dst = &bufferObject[1];
}

void PingPongBuffer::update(std::vector<float> data) {
	bufferObject[0].updateData(data);
	bufferObject[1].updateData(data);
}

void PingPongBuffer::swap() {
	std::swap(src, dst);
}

ofBufferObject& PingPongBuffer::operator[](int n) {
	return bufferObject[n];
}

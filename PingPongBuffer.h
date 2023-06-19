#pragma once
#include <ofShader.h>
#include <ofBufferObject.h>
#include <vector>

class PingPongBuffer {
private:
	ofBufferObject bufferObject[2];

public:
	ofBufferObject *src;
	ofBufferObject *dst;

	void allocate(std::vector<float> data);
	void update(std::vector<float> data);
	void swap();

	ofBufferObject& operator[](int n);
};


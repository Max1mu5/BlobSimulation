#version 440

uniform int width;
uniform int height;
uniform float decayRate;
uniform float diffuseRate;

layout(binding = 1) buffer oldTrail {
	float oldTrailMap[];
};

layout(binding = 2) buffer newTrail {
	float newTrailMap[];
};

float lerp(float to, float from, float weight) {
	return to + weight * (from - to);
}

float lerpVec4(float to, float from, float weight) {
	return max(0, lerp(to, from, weight) - decayRate);
}

layout(local_size_x = 1000, local_size_y = 1, local_size_z = 1) in;
void main() {
	int y = int(gl_GlobalInvocationID.x) / width;
	int x = int(gl_GlobalInvocationID.x) - y * width;

	float sum = 0;
	for (int modX = -1; modX <= 1; modX++) {
		int targetX = x + modX;
		if (targetX < 0 || targetX > width - 1) {
			continue;
		}
		for (int modY = -1; modY <= 1; modY++) {
			int targetY = y + modY;
			if (targetY < 0 || targetY > height - 1) {
				continue;
			}
			int index = targetX + (targetY * width);
			sum += oldTrailMap[index];
		}
	}

	float to = sum / float(9);
	int index = x + (y * width);
	float from = oldTrailMap[index];
	newTrailMap[gl_GlobalInvocationID.x] = lerpVec4(to, from, diffuseRate);
}

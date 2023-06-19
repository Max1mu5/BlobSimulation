#version 440

layout(binding = 0) buffer agent {
	vec4 agents[];
};


layout(binding = 1) buffer oldTrail {
	float oldTrailMap[];
};

layout(binding = 2) buffer newTrail {
	float newTrailMap[];
};

float hash(uint state) {
	state ^= 2747636419u;
	state *= 2654435769u;
	state ^= state >> 16;
	state *= 2654435769u;
	state ^= state >> 16;
	state *= 2654435769u;
	return state / 4294967295.0;
}

uniform float speed;
uniform int width;
uniform int height;
uniform float maxTrailDensity;
uniform float senseDistance;
uniform int sensorSize;
uniform float senseAngle;
uniform float steerStrength;
uniform float rotateAngle;
uniform float pheromoneStrength;


float sense(vec4 agent, float angleOffset) {
	float angle = mod(agent.z + angleOffset, 360.0);
	vec2 direction = vec2(cos(angle), sin(angle));
	vec2 senseLocation = senseDistance * direction + agent.xy;
	float sum = 0;
	for (int modX = -sensorSize; modX <= sensorSize; modX++) {
		int targetX = int(senseLocation.x) + modX;
		if (targetX < 0 || targetX > width - 1) {
			continue;
		}
		for (int modY = -sensorSize; modY <= sensorSize; modY++) {
			int targetY = int(senseLocation.y) + modY;
			if (targetY < 0 || targetY > height - 1) {
				continue;
			}
			int index = targetX + (targetY * width);
			sum += oldTrailMap[index];	
		}
	}
	int sensorArea = sensorSize == 0 ? 1 : (sensorSize + 2) * (sensorSize + 2);
	return sum / (maxTrailDensity);
}


layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;
void main() {
	vec4 agent = agents[gl_GlobalInvocationID.x];
	float left = sense(agent, -senseAngle);
	float right = sense(agent, senseAngle);
	float straight = sense(agent, 0.0);

	if (left > straight && left > right) {
		agent.z = mod(agent.z - (steerStrength), 360.0);
	}
	else if (right > straight && right > left) {
		agent.z = mod(agent.z + (steerStrength), 360.0);
	}

	vec2 direction = vec2(cos(agent.z + rotateAngle), sin(agent.z + rotateAngle));
	vec2 newPos = agent.xy + direction * speed;
	if (newPos.x < 0 || newPos.x > width - 1 || newPos.y < 0 || newPos.y > height - 1) {
		newPos.x = min(width - 1, max(0, agent.x));
		newPos.y = min(height - 1, max(0, agent.y));
		agent.z = mod(hash(uint(agent.x + agent.y * width)), 360.0f);
	}

	agents[gl_GlobalInvocationID.x] = vec4(newPos.x, newPos.y, agent.z, 0);

	int index = int(newPos.x) + int(newPos.y) * width;
	float val = oldTrailMap[index];
	oldTrailMap[index] = min(val + pheromoneStrength, maxTrailDensity);
}

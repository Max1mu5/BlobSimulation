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

layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;
void main() {
	vec4 agent = agents[gl_GlobalInvocationID.x];

	vec2 direction = vec2(cos(agent.z), sin(agent.z));
	vec2 newPos = agent.xy + direction * speed;

	if (newPos.x < 0 || newPos.x > width - 1 || newPos.y < 0 || newPos.y > height - 1) {
		newPos.x = min(width - 1, max(0, agent.x));
		newPos.y = min(height - 1, max(0, agent.y));
		agent.z = mod(agent.z + 60, 360.0f);
	}

	agents[gl_GlobalInvocationID.x] = vec4(newPos.x, newPos.y, agent.z, 0);

	int index = int(newPos.x) + int(newPos.y) * width;
	float val = oldTrailMap[index];
	oldTrailMap[index] = min(val + 3, maxTrailDensity);
}

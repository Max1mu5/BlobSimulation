#version 440

out vec4 outputColor;
uniform int width;
uniform int height;
uniform float decay;
uniform float maxTrailDensity;
uniform float xRatio;
uniform float yRatio;
uniform vec4 baseColor;

layout(binding = 2) buffer newTrail {
	float newTrailMap[];
};

void main()
{
    float x = gl_FragCoord.x / xRatio; 
    float y = gl_FragCoord.y / yRatio; 
    int index = int(x) + int(int(y) * width);
    
    float val = newTrailMap[index] / maxTrailDensity;

    vec4 chosenColor = baseColor;
    float weight = max(0, val);

    outputColor = mix(vec4(1.0, 1.0, 1.0, 1.0), vec4(0.2, 0.2, 0.2, 1.0), weight + decay);

}
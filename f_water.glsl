#version 330


uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform float moveFactor;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Zmienne interpolowane
in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;

const float waveStrength = 0.01;

void main(void) {
	vec2 ndc = ((clipSpace.xy/clipSpace.w)/2) + 0.5;

	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

	vec2 distortedTexCoords = texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg*0.1;
	distortedTexCoords = textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y+moveFactor);
	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;

	refractTexCoords += totalDistortion;
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

	reflectTexCoords += totalDistortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

	vec4 reflectColor = texture(tex, reflectTexCoords);
	vec4 refractColor = texture(tex2, refractTexCoords);

	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, vec3(0.0,1.0,0.0));
	refractiveFactor = pow(refractiveFactor, 0.4);

	vec4 normalMapColor = texture(normalMap, distortedTexCoords);
	vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
	normal = normalize(normal);

	pixelColor = mix(reflectColor, refractColor, refractiveFactor);
	pixelColor = mix(pixelColor, vec4(0.0,0.3,0.5,1.0),0.1);
}

#version 330


uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform float moveFactor;
uniform vec3 lightColor;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Zmienne interpolowane
in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 fromLightVector;

const float waveStrength = 0.02;
const float shineDamper = 10.0;
const float reflectivity = 0.3;

void main(void) {
	// Zmienienie wspolrzednych na [0;1]
	vec2 ndc = ((clipSpace.xy/clipSpace.w)/2) + 0.5;

	// Obliczenie wpolrzednych odbicia i zalamania
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

	// Zaklocenie przez wode
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

	// Obliczenie refractionFactor korzystaj?c z wektora do kamery i wektora normalnego wody
	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, vec3(0.0,1.0,0.0));
	refractiveFactor = pow(refractiveFactor, 0.3);

	// Stworzenie wektora normalnego z u?yciem tekstury 
	vec4 normalMapColor = texture(normalMap, distortedTexCoords);
	vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
	normal = normalize(normal);

	// Obliczenie ?wiat?a odbitego od wody 
	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = lightColor * specular * reflectivity;

	// Obliczenie kolory wynikowego4
	pixelColor = mix(reflectColor, refractColor, refractiveFactor);
	pixelColor = mix(pixelColor, vec4(0.0,0.3,0.5,1.0),0.1) + vec4(specularHighlights, 0.0);
}

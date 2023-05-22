#version 330

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela


uniform vec4 color=vec4(0,0,1,1);
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

in vec4 clipSpace;

void main(void) {

	vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;

	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoors = vec2(ndc.x, -ndc.y);

	vec4 reflectColor = texture(reflectionTexture, reflectTexCoors);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);

	pixelColor = mix(reflectColor, refractColor, 0.5);
}

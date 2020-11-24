#version 450

in vec2 uv;
in vec3 Normal;
in vec3 lpos;

out vec3 color;

uniform sampler2D myTexture;

void main() {
    

    float dott = dot(Normal, lpos);

    float diff= max(dott,0.0);



	color = texture(myTexture, uv).rgb * diff;

}
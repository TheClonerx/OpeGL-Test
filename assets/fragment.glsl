#version 330 core

in vec4 color;
in vec2 textureCoord;

out vec4 outColor;

uniform sampler2D texture1;

void main() {
    vec4 fragmentColor = texture(texture1, textureCoord) * /* texture(texture2, textureCoord) * */ color;
    if (fragmentColor.a <= 0.05)
        discard;
    outColor = fragmentColor;
}

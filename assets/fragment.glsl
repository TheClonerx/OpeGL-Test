#version 330 core

in vec4 color;
in vec2 textureCoord;

out vec4 fragmentColor;

uniform sampler2D texture1;

void main() {
    fragmentColor = texture(texture1, textureCoord) * /* texture(texture2, textureCoord) * */ color;
}

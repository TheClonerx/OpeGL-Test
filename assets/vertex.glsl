#version 330 core

layout (location = 0) in vec3 vertexCoord;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec2 vertexUV;

out vec4 color;
out vec2 textureCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexCoord, 1.0);
    color = vertexColor;
    textureCoord = vertexUV;
}
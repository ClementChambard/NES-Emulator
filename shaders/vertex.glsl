#version 130
precision mediump float;

in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexTexCoords;

out vec2 fragmentTexCoords;

void main()
{
    gl_Position = vec4(vertexPosition, 1.0);
    fragmentTexCoords = vertexTexCoords;
}

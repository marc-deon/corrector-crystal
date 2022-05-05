#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
// in vec4 fragColor;

// "Greyscale" version of sprite
uniform sampler2D texture0;

// 256x1 texture containing sprite's palette
uniform sampler2D palette;

// Output fragment color
out vec4 finalColor;

void main()
{
    float x = texture(texture0, fragTexCoord).x;
    finalColor = texture(palette, vec2(x, 0));
}


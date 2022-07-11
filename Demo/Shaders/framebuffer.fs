#version 330

#define POST_NONE    0
#define POST_INVERT  1
#define POST_GRAY    2
#define POST_BLUR    3
#define POST_SHARPEN 4

in vec2 texCoord;
out vec4 fragColorOut;

uniform sampler2D screenTexture;
uniform int selected = POST_SHARPEN;

vec3 noPostProcessing()
{
    return texture(screenTexture, texCoord).rgb;
}

vec3 inversion()
{
    vec3 colour = texture(screenTexture, texCoord).rgb;
    return vec3(1.0) - colour;
}

vec3 grayScale()
{
    vec3 colour = texture(screenTexture, texCoord).rgb;
    float average = 0.2126 * colour.r + 0.7152 * colour.g + 0.0722 * colour.b;
    return vec3(average, average, average);
}

vec3 kernelEffect(float kernel[9]);

vec3 blur()
{
    float kernel[9] = float[]
    (
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    );

    return kernelEffect(kernel);
}

vec3 sharpen()
{
    float kernel[9] = float[]
    (
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    return kernelEffect(kernel);
}

void main()
{
    if(selected == POST_NONE)
    {
        fragColorOut = vec4(noPostProcessing(), 1.0);
    } else if(selected == POST_INVERT)
    {
        fragColorOut = vec4(inversion(), 1.0);
    } else if(selected == POST_GRAY)
    {
        fragColorOut = vec4(grayScale(), 1.0);
    } else if(selected == POST_BLUR)
    {
        fragColorOut = vec4(blur(), 1.0);
    } else
    {
        fragColorOut = vec4(sharpen(), 1.0);
    }
}

vec3 kernelEffect(float kernel[9])
{
    const float OFFSET = 1.0 / 300.0;

    vec2 offsets[9] = vec2[]
    (
        vec2(-OFFSET,  OFFSET), // top-left
        vec2( 0.0f,    OFFSET), // top-center
        vec2( OFFSET,  OFFSET), // top-right
        vec2(-OFFSET,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( OFFSET,  0.0f),   // center-right
        vec2(-OFFSET, -OFFSET), // bottom-left
        vec2( 0.0f,   -OFFSET), // bottom-center
        vec2( OFFSET, -OFFSET)  // bottom-right
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; ++i)
    {
        sampleTex[i] = vec3(texture(screenTexture, texCoord.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; ++i)
    {
        col += sampleTex[i] * kernel[i];
    }

    return col;
}


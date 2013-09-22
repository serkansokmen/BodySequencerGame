#version 150

// these are our textures
uniform sampler2DRect gridTexture;
uniform sampler2DRect gridAlpha;

uniform int currentStep;

// this comes from the vertex shader
in vec2 texCoordVarying;
in vec4 position;

// this is the output of the fragment shader
out vec4 outputColor;

void main()
{
    // get rgb from tex0
    vec3 src = texture(gridTexture, texCoordVarying).rgb;
    
    // get alpha from mask
    float mask = texture(gridAlpha, texCoordVarying).a;
    
    //mix the rgb from tex0 with the alpha of the mask
    outputColor = vec4(src , mask);
}
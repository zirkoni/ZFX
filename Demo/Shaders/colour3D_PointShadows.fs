#version 330 core
out vec4 f_out_colour;

in V_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} f_in;

uniform sampler2D u_diffuseTexture;
uniform samplerCube u_depthMap;

uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

uniform float u_farPlane;


// Array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float shadowCalculation(vec3 position)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = position - u_lightPos;

    // Get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

	// PCF
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(u_viewPos - position);
    float diskRadius = (1.0 + (viewDistance / u_farPlane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(u_depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= u_farPlane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
        {
            shadow += 1.0;
        }
    }

    shadow /= float(samples);
    return shadow;
}

void main()
{
    vec3 color = texture(u_diffuseTexture, f_in.texCoord).rgb;
    vec3 normal = normalize(f_in.normal);
    vec3 lightColor = vec3(0.3);

    // ambient
    vec3 ambient = 0.6 * lightColor;

    // diffuse
    vec3 lightDir = normalize(u_lightPos - f_in.position);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(u_viewPos - f_in.position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    // calculate shadow
    float shadow = shadowCalculation(f_in.position);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    f_out_colour = vec4(lighting, 1.0);
}


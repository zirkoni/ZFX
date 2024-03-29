#version 330 core

out vec4 f_out_colour;

in V_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    vec4 fragPosLightSpace;
} f_in;

uniform sampler2D u_diffuseTexture;
uniform sampler2D u_shadowMap;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

float shadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_shadowMap, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(f_in.normal);
    vec3 lightDir = normalize(u_lightPos - f_in.position);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }

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
    float shadow = shadowCalculation(f_in.fragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    f_out_colour = vec4(lighting, 1.0);
}

#version 330 core

in vec3 oNormal;
in vec2 oTexCoord;
in vec3 oFragPos;
in vec4 oLightSpaceFragPos;

uniform sampler2D diffuseTexture;
uniform sampler2D depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

// 在ps中进行坐标变换不可取，效率低；优化项
//float calculateShadow(vec3 fragPos)
//{
//    vec4 lightSpacePos = (oLightSpaceMatrix * vec4(fragPos, 1.0));
//    vec3 projCoord = lightSpacePos.xyz / lightSpacePos.w;

//    // transform to [0,1]
//    projCoord = projCoord * 0.5 + 0.5;
//    float closeDepth = texture(depthMap, projCoord.xy).r;
//    float curDepth = projCoord.z;

//    float bias = 0.005;
//    vec3 lightDir = normalize(lightPos - oFragPos);
//    //bias = max(0.05*(1 - dot(oNormal, lightDir)), bias);
//    float shadow = curDepth - bias > closeDepth ? 1.0 : 0.0;

//    return shadow;
//}

float calculateShadow(vec4 lightSpaceFragPos)
{
    // 先要从投影空间转化的NDC空间
    vec3 projCoord = lightSpaceFragPos.xyz / lightSpaceFragPos.w;

    // transform to [0,1]
    projCoord = projCoord * 0.5 + 0.5;

    float closeDepth = texture(depthMap, projCoord.xy).r;
    float curDepth = projCoord.z;

    float bias = 0.005;
    vec3 lightDir = normalize(lightPos - oFragPos);
    //bias = max(0.05*(1 - dot(oNormal, lightDir)), bias);
    float shadow = curDepth - bias > closeDepth ? 1.0 : 0.0;

    return shadow;
}
void main()
{
    vec3 color = texture(diffuseTexture, oTexCoord).rgb;
    vec3 normal = normalize(oNormal);
    vec3 lightColor = vec3(1.0);

    vec3 ambient = 0.3 * lightColor;

    vec3 lightDir = normalize(lightPos - oFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - oFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(halfDir, normal), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    float shadow = calculateShadow(oLightSpaceFragPos);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0);

}
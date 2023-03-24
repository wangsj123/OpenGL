#version 330 core

struct DirLight
{
	vec3 dir;
	vec3 color;
};

struct PointLight
{
	vec3 pos;
	vec3 color;
};

struct SpotLight
{
	vec3 pos;
	float outerCutOff;
	vec3 dir;
	float cutOff;
	vec3 color;
};

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 eyePos);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 eyePos);

uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform vec3 eyePos;
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D emissionMap;

in vec3 normal;
in vec2 texcoord;
in vec3 fragPos;

out vec4 FragColor;

void main()
{
	/* // texture
	vec3 diffColor = vec3(texture(diffuseMap, texcoord));
	vec4 specColor = vec4(texture(specularMap, texcoord));
	vec3 emissColor = vec3(texture(emissionMap, texcoord));

	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(lightColor);

	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 norm = normalize(normal);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor.xyz * diffColor.xyz;

	// 反射光线与相机方向的点乘
	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 eyeDir = normalize(eyePos - fragPos);
	//float spec = max(dot(reflectDir, eyeDir), 0);
	//vec3 specular = pow(spec, 64) * lightColor.xyz * specularStrength;

	// lightDir 和 eyeDir 中心向量与法向量点乘
	vec3 midDir = normalize(lightDir + eyeDir);
	float spec = pow(max(dot(midDir, norm), 0), 64);
	vec3 specular = spec * lightColor.xyz * specularStrength * specColor.rgb;

	vec3 result = (ambient + diffuse + specular + emissColor);*/

	vec3 viewDir = eyePos - fragPos;
	vec3 result = calcDirLight(dirLight, normal, viewDir);
	result += calcPointLight(pointLight, normal, fragPos, eyePos);
	result += calcSpotLight(spotLight, normal, fragPos, eyePos);
	FragColor =  vec4(result, 1.0f);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	float ambt = 0.2f;

	vec3 dirLight = normalize(-light.dir);
	vec3 norm = normalize(normal);
	float diff = max(dot(dirLight, norm), 0);

	/* phong 光照模型
	vec3 reflectDir = normalize(reflect(-dirLight, norm));
	float spec = pow(max(dot(reflectDir, viewDir), 0), 32);*/

	// blinn phong
	vec3 halfDir = normalize(dirLight + viewDir);
	float spec = pow(max(dot(halfDir, norm), 0), 128);

	vec3 ambient = light.color * vec3(texture(diffuseMap, texcoord)) * ambt;
	vec3 diffuse = light.color * vec3(texture(diffuseMap, texcoord)) * diff;
	vec3 specular = light.color * vec3(texture(specularMap, texcoord)) * spec;

	return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 eyePos)
{
	vec3 lightDir = normalize(light.pos - fragPos);
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(eyePos - fragPos);
	vec3 halfDir = normalize(lightDir + viewDir);

	float ambt = 0.1;
	float diff = max(dot(lightDir, norm), 0);
	float spec = pow(max(dot(halfDir, norm), 0), 32);

	// attenuation 二次方衰减
	float dis = length(light.pos - fragPos);
	float constant = 1.0;
	float linear = 0.09;
	float quadratic = 0.032;
	float attenuation = 1 / (constant + linear * dis + quadratic * dis * dis);

	vec3 ambient = light.color * vec3(texture(diffuseMap, texcoord)) * ambt;
	vec3 diffuse = light.color * vec3(texture(diffuseMap, texcoord)) * diff;
	vec3 specular = light.color * vec3(texture(specularMap, texcoord)) * spec;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 eyePos)
{
	vec3 lightDir = normalize(light.pos - fragPos);
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(eyePos - fragPos);
	vec3 halfDir = normalize(lightDir + viewDir);

	float ambt = 0.2;
	float diff = max(dot(lightDir, norm), 0);
	float spec = pow(max(dot(halfDir, norm), 0), 32);

	// attenuation 二次方衰减
	float dis = length(light.pos - fragPos);
	float constant = 1.0;
	float linear = 0.09;
	float quadratic = 0.032;
	float attenuation = 1.0 / (constant + linear * dis + quadratic * dis * dis);


	// spotlight intensity
	float theta = dot(lightDir, normalize(-light.dir));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);


	{
		vec3 ambient = light.color * vec3(texture(diffuseMap, texcoord)) * ambt;
		vec3 diffuse = light.color * vec3(texture(diffuseMap, texcoord)) * diff;
		vec3 specular = light.color * vec3(texture(specularMap, texcoord)) * spec;

		// ambient *= (attenuation * intensity);
		diffuse *= (attenuation * intensity);
		specular *= (attenuation * intensity);

		return (ambient + diffuse + specular);
	}
}
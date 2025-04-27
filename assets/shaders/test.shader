#shader vertex
#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 aTexCords;
layout(location = 3) in vec4 instanceModelRow1;
layout(location = 4) in vec4 instanceModelRow2;
layout(location = 5) in vec4 instanceModelRow3;
layout(location = 6) in vec4 instanceModelRow4;

layout(std140, binding = 0) uniform CameraData {
    mat4 projection;
    vec3 cameraPos;
};

out vec3 fragNormal;
out vec3 fragPos;
out vec2 texCords;

void main() {
    mat4 instanceModel = mat4(instanceModelRow1,
                              instanceModelRow2,
                              instanceModelRow3,
                              instanceModelRow4);

    fragPos = vec3(instanceModel * vec4(position, 1.0));

    gl_Position = projection * instanceModel * vec4(position, 1.0);

    fragNormal = mat3(transpose(inverse(instanceModel))) * normal;

    texCords = aTexCords;
}

#shader fragment
#version 430 core

struct Material {
    vec3 albedo;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {
    vec3 color;
    float constant;
    vec3 position;
    float linear;
    vec3 padding;
    float quadratic;
};

struct DirectionalLight {
    vec3 color;
    float padding1;
    vec3 direction;
    float padding2;
};

layout(std140, binding = 0) uniform CameraData {
    mat4 projection;
    vec3 cameraPos;
};

layout(std140, binding = 1) uniform LightData {
    int isLightingEnabled;
    int padding;
    int numberOfPointLights;
    int numberOfDirectionalLights;
    PointLight pointLights[20];
    DirectionalLight directionalLights[5];
};


in vec3 fragNormal;
in vec3 fragPos;
in vec2 texCords;

uniform Material material;

uniform sampler2D tex;
uniform int isTexture;

out vec4 color;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // Ambient
    vec3 ambient = light.color * material.ambient;;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * light.color;

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * material.specular * light.color;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));

    // Combine results
    return (ambient + diffuse + specular) * attenuation * material.albedo;
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // Ambient
    vec3 ambient = light.color * material.ambient;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * light.color;

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * material.specular * light.color;

    // Combine results
    return ambient + diffuse + specular;
}

void main() {
    // Normalize all vectors
    vec3 normalizedNormal = normalize(fragNormal);
    vec3 viewDir = normalize(cameraPos - fragPos);

    vec3 result = vec3(0.0);

    if (isLightingEnabled == 1) {
        for(int i = 0; i < numberOfPointLights; i++) {
            result += calcPointLight(pointLights[i], normalizedNormal, viewDir);
        }

        for(int i = 0; i < numberOfDirectionalLights; i++) {
            result += calcDirectionalLight(directionalLights[i], normalizedNormal, viewDir);
        }
    } else {
        result = vec3(1.0);   
    }

    // Apply texture
    vec4 baseColor = (isTexture == 1) ? texture(tex, texCords) : vec4(material.albedo, 1.0);
    
    color = vec4(result, 1.0) * baseColor;
    //color = vec4(1.0, 0.0, 0.0, 1.0); // Red color for debugging

}


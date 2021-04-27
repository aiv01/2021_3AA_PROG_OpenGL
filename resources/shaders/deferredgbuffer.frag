#version 450 core
out vec4 fragColor;

in vec2 vert_uv;
in vec3 worldNorm;
in vec3 worldPos;

layout (binding = 1) uniform sampler2D boxText;

uniform vec3 pointLightPos;
uniform vec3 cameraPos;

void main() {
    vec3 meshColor = texture(boxText, vert_uv).rgb;
    
    vec3 lightColor = vec3(1.f);
    vec3 lightDir = normalize(pointLightPos - worldPos);
    vec3 norm = normalize(worldNorm);
    float specularShineness = 40.f;
    vec3 specularColor = vec3(1.f, 0.f, 0.f);

    //Ambient
    float ambientFactor = 0.2f;
    vec3 ambient = lightColor * meshColor * ambientFactor;

    //Diffuse
    vec3 diffuse = vec3(0.f);
    float lambert = max(dot(norm, lightDir), 0);
    diffuse = lightColor * meshColor * lambert;

    //Specular
    vec3 specular = vec3(0.f);

    vec3 dirToEye = normalize(cameraPos - worldPos);
    vec3 refletDir = reflect(-lightDir, norm);
    float specularStrength = max(dot(dirToEye, refletDir), 0);
    float specularFactor = pow(specularStrength, specularShineness);
    specular = specularColor * specularFactor;



    vec3 phong = ambient + diffuse + specular;

    float distance = length(pointLightPos - worldPos);
    float k_constant = 1.f;
    float k_linear = 0.07f;
    float k_quadratic = 0.017f;
    //float attenuation = 1.f / distance;
    float attenuation = 1.f / (k_constant  + 
                               k_linear * distance +
                               k_quadratic * (distance * distance)
    );

    phong *= attenuation;

    fragColor = vec4(phong, 1.f);
}
#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec4 vertexColor;

out vec4 FragColor;

uniform vec3 lightDir; // direction TO light
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform bool lightOn;
uniform vec3 objectColor;

void main()
{
    vec3 baseColor = objectColor * vertexColor.rgb;
    if (!lightOn) {
        FragColor = vec4(baseColor, 1.0);
        return;
    }

    // Ambient (kept low so object hue shows)
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightDir);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.2;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * baseColor;
    FragColor = vec4(result, 1.0);
}

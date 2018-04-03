#version 330 core

in vec3 frag_color;
in vec3 frag_pos;
in vec3 frag_normal;

uniform vec3 light_pos;
uniform vec3 view_pos;
uniform vec3 light_color;

out vec4 FragColor;

void main(){
    vec3 light_dir = normalize(light_pos - frag_pos);
    vec3 view_dir = normalize(view_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, frag_normal);

	// ambient light
	float ambient_strength = 0.1f;
	vec3 ambient = ambient_strength * light_color;
    // diffuse light
	float diff = max(dot(frag_normal, light_dir), 0.0f);
	vec3 diffuse = diff * light_color;
    // specular light
	float specular_strength = 0.5f;
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 16);
    vec3 specular = specular_strength * spec * light_color;

    vec3 result = (ambient + diffuse + specular) * frag_color;
    FragColor = vec4(result, 1.0f);
}

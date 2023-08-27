#version 450

in vec2 fragPos;

out vec4 FragColor;

uniform vec4 light_color;
void main() {
    // Calculate the distance from the fragment position to the origin
    float dis = sqrt(dot(fragPos, fragPos));

    if(dis >= 1.0) discard;
    // Calculate the blend factor based on distance (closer to the center means higher blend factor)
    float blendFactor = smoothstep(1.0, -1.0, dis);

    // Blend between light_color.xyz and white color using the blend factor
    vec3 blendedColor = mix(light_color.xyz * light_color.w, vec3(1.0), blendFactor);

    // Set the fragment color
    FragColor = vec4(blendedColor * light_color.w, 1.0);
}
#version 410 core
uniform bool uHasTexture;
uniform sampler2D uTexture;
uniform vec4 uColor;

in vec2 vUV;
out vec4 fragColor;

void main() {
    vec4 baseColor;
    if (uHasTexture) {
        baseColor = texture(uTexture, vUV);
    } else {
        baseColor = uColor;
    }
    fragColor = baseColor;
}

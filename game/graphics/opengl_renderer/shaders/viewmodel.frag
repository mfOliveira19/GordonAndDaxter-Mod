#version 410 core
uniform bool uHasTexture;
uniform bool uUseNormalUV;
uniform sampler2D uTexture;
uniform vec4 uColor;

uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform vec3 uAmbient;

uniform float uChromeSpecPower;
uniform float uChromeIntensity;
uniform float uChromeDiffuseStrength;
uniform vec2 uChromeUVScale;
uniform vec2 uChromeUVOffset;

in vec2 vUV;
in vec3 vNormalView;

out vec4 fragColor;

void main() {
    vec3 n = normalize(vNormalView);

    vec2 chromeUV = n.xy * 0.5 + 0.5;
    chromeUV = chromeUV * uChromeUVScale + uChromeUVOffset;

    vec2 uv = uUseNormalUV ? chromeUV : vUV;
    vec4 baseColor = uHasTexture ? texture(uTexture, uv) : uColor;

    vec3 lighting;

    if (uUseNormalUV) {
        float diff = max(dot(n, normalize(uLightDir)), 0.0);
        lighting = uAmbient + uLightColor * diff;

        float spec = pow(diff, uChromeSpecPower);
        lighting += vec3(uChromeIntensity) * spec;

        vec3 litColor = baseColor.rgb * (lighting * uChromeDiffuseStrength + 0.9);
        fragColor = vec4(litColor, baseColor.a);

    } else {
        float diff = 0.5 * (dot(n, normalize(uLightDir)) + 1.0);

        lighting = uAmbient + uLightColor * diff;

        vec3 litColor = baseColor.rgb * (lighting * 1.2 + 1.0);

        fragColor = vec4(litColor, baseColor.a);
    }
}

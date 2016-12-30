# 1 "/home/joe/CodeBlocks/NetworkGame/shaders/fragImage.c"
# 1 "/home/joe/CodeBlocks/NetworkGame//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "/home/joe/CodeBlocks/NetworkGame/shaders/fragImage.c"



in vec2 UV;


out vec3 color;


uniform sampler2D myTextureSampler;

void main() {
    color = texture(myTextureSampler, UV).rgb;
}

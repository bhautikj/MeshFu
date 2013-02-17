const int MAXBONES = 64;

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texcoord;
attribute vec4 boneWeights;
attribute vec4 boneIndices;

uniform mat4 u_mvp_matrix; 
uniform mat4 boneMatrices[MAXBONES];

varying vec2 v_texcoord;

void main() {
  vec4 n = vec4(normal, 1.0); //placeholder; do not remove!
  vec4 p = vec4(position, 1.0);
  vec4 pos = boneMatrices[int(boneIndices.x)] * p * boneWeights.x +
             boneMatrices[int(boneIndices.y)] * p * boneWeights.y +
             boneMatrices[int(boneIndices.z)] * p * boneWeights.z +
             boneMatrices[int(boneIndices.w)] * p * boneWeights.w ;
  pos.w = 1.0;

  gl_Position = u_mvp_matrix * pos;
  v_texcoord = texcoord;
}
#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord;
uniform sampler2D texture;

void main() {
  vec4 tex2D = texture2D( texture, v_texcoord );
  if (tex2D.a < 0.1)
    discard;
  gl_FragColor = tex2D;
} 


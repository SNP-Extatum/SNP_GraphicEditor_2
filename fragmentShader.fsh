uniform sampler2D u_texture;
varying highp vec2 v_textcoord;
varying highp vec4 v_position;
varying highp vec3 v_normal;
void main(void)
{
  //gl_FragColor = vec4(1.0,0.0,0.0,1.0);

  gl_FragColor = texture2D(u_texture, v_textcoord);
}

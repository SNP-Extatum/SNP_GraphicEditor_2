uniform vec4 u_simpleColor;
uniform bool u_isTextureUse;
uniform sampler2D u_texture;
varying vec2 v_textcoord;


void main(void)
{
  if (u_isTextureUse) {
	gl_FragColor = texture2D(u_texture, v_textcoord);
  } else { gl_FragColor = u_simpleColor; }
}

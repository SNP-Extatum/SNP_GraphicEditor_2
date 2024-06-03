uniform vec4 u_simpleColor;
uniform bool u_isTextureUse;
uniform sampler2D u_texture;
uniform highp vec4 u_lightPosition;
uniform highp float u_lightPower;
varying highp vec2 v_textcoord;
varying highp vec4 v_position;
varying highp vec3 v_normal;

void main(void)
{
  vec4 resulColor = vec4(0.0,0.0,0.0,0.0);
  vec4 eyePosition = vec4(0.0,0.0,0.0,1.0);
  vec4 diffMatColor;
  //if (u_isTextureUse){
    diffMatColor = texture2D(u_texture, v_textcoord);
  //} else { diffMatColor = u_simpleColor;}
  vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz);
  vec3 lightVect = normalize(v_position.xyz - u_lightPosition.xyz);
  vec3 reflectLight = normalize(reflect(lightVect, v_normal));
  float len = length(v_position.xyz - eyePosition.xyz);
  float specularFactor = 75.0;
  float ambientFactor = 0.1;

  vec4 diffColor = diffMatColor * u_lightPower * max(0.0, (dot(v_normal, -lightVect))) ;// / (1.0 + 0.25 * pow(len, 2));
  resulColor += diffColor;
//20
  vec4 ambientColor = ambientFactor * diffColor;
  resulColor += ambientColor;

  vec4 specularColor = vec4(1.0,1.0,1.0,1.0) * u_lightPower * pow(max(0.0, (dot(reflectLight, -eyeVect))), specularFactor);// / (1.0 + 0.25 * pow(len, 2));
resulColor += specularColor;

  gl_FragColor = resulColor;
}

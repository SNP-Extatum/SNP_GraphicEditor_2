attribute vec4 a_position;
attribute vec2 a_textcoord;
attribute vec3 a_normal;
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;
varying vec2 v_textcoord;

void main(void)
{
  mat4 viewMatrix = u_viewMatrix;
  viewMatrix[3][0] = 0;
  viewMatrix[3][1] = 0;
  viewMatrix[3][2] = 0;
  mat4 mv_matrix = viewMatrix * u_modelMatrix;
  gl_Position =  u_projectionMatrix * mv_matrix * a_position;
  v_textcoord = a_textcoord;
}

#include "camera.hpp"

Camera::Camera() {
  scale = 1.0f;
  globalTransform.setToIdentity();
}
void Camera::rotate(const QQuaternion& rot) {
  rotation = rot * rotation;

  updateViewMatrix();
}

void Camera::rotateX(const QQuaternion& rot) {
  rotationX = rot * rotationX;
  rotation = rotationX * rotationY;
  updateViewMatrix();
}

void Camera::rotateY(const QQuaternion& rot) {
  rotationY = rot * rotationY;
  rotation = rotationX * rotationY;
  updateViewMatrix();
}

void Camera::translate(const QVector3D& trans) {
  translation += trans;

  updateViewMatrix();
}

void Camera::scaleIt(const float& scaling) {
  scale *= scaling;
  updateViewMatrix();
}

void Camera::setGlobalTransform(const QMatrix4x4& mat) { globalTransform = mat; }

void Camera::draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions) {
  if (functions != 0) return;
  updateViewMatrix();
  shaderProgram->setUniformValue("u_viewMatrix", viewMatrix);
}

const QMatrix4x4& Camera::getViewMatrix() const { return viewMatrix; }

void Camera::updateViewMatrix() {
  viewMatrix.setToIdentity();
  viewMatrix.translate(translation);
  viewMatrix.rotate(rotation);
  viewMatrix.scale(scale);
  viewMatrix = viewMatrix * globalTransform.inverted();
}

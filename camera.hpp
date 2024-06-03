#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "transformational.hpp"

class Camera : public Transformational {
 public:
  Camera();

  void rotate(const QQuaternion& rot) override;
  void rotateX(const QQuaternion& rot);
  void rotateY(const QQuaternion& rot);
  void translate(const QVector3D& trans) override;
  void scaleIt(const float& scaling) override;
  void setGlobalTransform(const QMatrix4x4& mat) override;
  void draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions = 0) override;

  const QMatrix4x4& getViewMatrix() const;

  void updateViewMatrix();

 private:
  QQuaternion rotation;
  QQuaternion rotationX;
  QQuaternion rotationY;
  QVector3D translation;
  float scale;
  QMatrix4x4 globalTransform;
  QMatrix4x4 viewMatrix;
};

#endif  // CAMERA_HPP

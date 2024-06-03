#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <QImage>

#include "cubic.hpp"
#include "transformational.hpp"

class SkyBox : public Transformational {
 public:
  SkyBox(float width, const QImage& texture);
  ~SkyBox();
  void draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions) override;
  void rotate(const QQuaternion& rot) override;
  void translate(const QVector3D& trans) override;
  void scaleIt(const float& scaling) override;
  void setGlobalTransform(const QMatrix4x4& mat) override;

 private:
  Cubic* cube;
};

#endif  // SKYBOX_HPP

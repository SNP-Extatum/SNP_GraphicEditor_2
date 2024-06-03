#ifndef GROUP3D_HPP
#define GROUP3D_HPP

#include "transformational.hpp"

class Group3d : public Transformational {
 public:
  Group3d();

  // Transformational interface
 public:
  void rotate(const QQuaternion& rot) override;
  void translate(const QVector3D& trans) override;
  void scaleIt(const float& scaling) override;
  void setGlobalTransform(const QMatrix4x4& mat) override;
  void draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions) override;

  void addObject(Transformational* obj);

  void delObject(Transformational* obj);
  void delObject(const int& index);

  void updateLocalMatrix();

 private:
  QQuaternion rotation;
  QVector3D translation;
  float scale;
  QMatrix4x4 globalTransform;
  QMatrix4x4 localMatrix;

  QVector<Transformational*> objects;
};

#endif  // GROUP3D_HPP

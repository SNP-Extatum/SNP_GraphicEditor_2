#include "group3d.hpp"

Group3d::Group3d() { scale = 1.0f; }

void Group3d::rotate(const QQuaternion& rot) { rotation = rot * rotation; }

void Group3d::translate(const QVector3D& trans) { translation += trans; }

void Group3d::scaleIt(const float& scaling) { scale *= scaling; }

void Group3d::setGlobalTransform(const QMatrix4x4& mat) { globalTransform = mat; }

void Group3d::draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions) {
  QMatrix4x4 localMatrix;
  localMatrix.setToIdentity();
  localMatrix.setToIdentity();
  localMatrix.translate(translation);
  localMatrix.rotate(rotation);
  localMatrix.scale(scale);
  localMatrix = globalTransform * localMatrix;

  for (int i = 0; i < objects.size(); ++i) {
	objects[i]->setGlobalTransform(localMatrix);
	objects[i]->draw(shaderProgram, functions);
  }
}

void Group3d::addObject(Transformational* obj) { objects.append(obj); }

#include "group3d.hpp"

Group3d::Group3d() { scale = 1.0f; }

void Group3d::rotate(const QQuaternion& rot) {
  rotation = rot * rotation;

  updateLocalMatrix();
}

void Group3d::translate(const QVector3D& trans) {
  translation += trans;
  updateLocalMatrix();
}

void Group3d::scaleIt(const float& scaling) {
  scale *= scaling;
  updateLocalMatrix();
}

void Group3d::setGlobalTransform(const QMatrix4x4& mat) {
  globalTransform = mat;

  updateLocalMatrix();
}

void Group3d::draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions) {
  for (int i = 0; i < objects.size(); ++i) {
	objects[i]->draw(shaderProgram, functions);
  }
}

void Group3d::addObject(Transformational* obj) {
  objects.append(obj);
  updateLocalMatrix();

  // objects[objects.size() - 1]->setGlobalTransform(localMatrix);
}

void Group3d::delObject(Transformational* obj) { objects.removeAll(obj); }

void Group3d::delObject(const int& index) { objects.remove(index); }

void Group3d::updateLocalMatrix() {
  localMatrix.setToIdentity();
  localMatrix.translate(translation);
  localMatrix.rotate(rotation);
  localMatrix.scale(scale);
  localMatrix = globalTransform * localMatrix;

  for (int i = 0; i < objects.size(); ++i) {
	objects[i]->setGlobalTransform(localMatrix);
  }
}

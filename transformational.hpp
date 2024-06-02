#ifndef TRANSFORMATIONAL_HPP
#define TRANSFORMATIONAL_HPP

#include <QDebug>
#include <QList>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QTimer>
#include <QtMath>

class Transformational {
 public:
  virtual void rotate(const QQuaternion& rot) = 0;
  virtual void translate(const QVector3D& trans) = 0;
  virtual void scaleIt(const float& scaling) = 0;
  virtual void setGlobalTransform(const QMatrix4x4& mat) = 0;
  virtual void draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions) = 0;
};

#endif  // TRANSFORMATIONAL_HPP

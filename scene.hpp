#ifndef SCENE_HPP
#define SCENE_HPP

#include <math.h>

#include <QDebug>
#include <QList>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>

struct VertexData {
  VertexData() {}
  VertexData(QVector3D pos, QVector2D textCoord, QVector3D norm) : position(pos), textureCoord(textCoord), normal(norm) {}
  QVector3D position;
  QVector2D textureCoord;
  QVector3D normal;
};

class Scene : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
 public:
  Scene(QWidget* parent = 0);

  // QOpenGLWidget interface
 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  // QWidget interface
 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;

 protected:
  void initShaders();
  void initCube(float _width);

 private:
  QMatrix4x4 projectionMatrix;         // матрица проекции
  QOpenGLShaderProgram shaderProgram;  // шейдеры
  QOpenGLTexture* texture;
  QOpenGLBuffer arrayBuffer;
  QOpenGLBuffer indexBuffer;
  QVector2D mousePos;
  float mouseSpeed = 2.0f;
  QQuaternion rotation;
  //
};

#endif  // SCENE_HPP

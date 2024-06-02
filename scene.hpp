#ifndef SCENE_HPP
#define SCENE_HPP

#include "group3d.hpp"
#include "simpleobject.hpp"

class Scene : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
 public:
  Scene(QWidget* parent = 0);

  // QObject interface
 protected:
  void timerEvent(QTimerEvent* event) override;

  // QOpenGLWidget interface
 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  // QWidget interface
 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

 protected:
  void initShaders();
  void initCube(float _width);

 private:
  QMatrix4x4 projectionMatrix;         // матрица проекции
  QOpenGLShaderProgram shaderProgram;  // шейдеры
  QVector2D mousePos;
  float mouseSpeed = 2.0f;
  QQuaternion rotation;

  QVector<SimpleObject*> objects;
  QVector<Transformational*> transformObjects;
  QVector<Group3d*> groups;

  void showOrts();

  QTimer timer;
  float angleObject;
  float angleGroup1;
  float angleGroup2;
  float angleMain;

  float dz = -5;
 public slots:
  void simpleAnimation();

  //
};

#endif  // SCENE_HPP

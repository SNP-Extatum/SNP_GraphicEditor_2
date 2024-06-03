#ifndef SCENE_HPP
#define SCENE_HPP

 #include <QKeyEvent>
 #include <QMouseEvent>

#include "camera.hpp"
#include "cubic.hpp"
#include "group3d.hpp"
#include "skybox.hpp"

struct Controllers {
  bool forward = false;
  bool back = false;
  bool right = false;
  bool left = false;
  bool up = false;
  bool down = false;
};

class Scene : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
 public:
  Scene(QWidget* parent = 0);
  ~Scene();
  void createStarHouse(int countStars);

  // QObject interface
 protected:
  void timerEvent(QTimerEvent* event) override;

  // QOpenGLWidget interface
 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  // QWidget interface
 public:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

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
  QOpenGLShaderProgram shaderSkyBox;

  QVector2D mousePos;
  float mouseSpeed = 2.0f;

  QVector<Cubic*> objects;
  QVector<Transformational*> transformObjects;
  QVector<Group3d*> groups;

  void showOrts();

  QTimer timer;
  float angleObject;
  float angleGroup1;
  float angleGroup2;
  float angleMain;

  Camera* camera;
  SkyBox* skybox;
  void selectNextGroupForCamera();
  Controllers controllers;

 public slots:
  void simpleAnimation();

  //
};

#endif  // SCENE_HPP

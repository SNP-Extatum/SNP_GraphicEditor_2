#include "scene.hpp"

Scene::Scene(QWidget* parent) : QOpenGLWidget(parent), QOpenGLFunctions() {
  camera = new Camera();
  camera->translate(QVector3D(0, 0, -5));
}

Scene::~Scene() {
  delete camera;

  for (int i = 0; i < objects.size(); ++i) {
	delete objects[i];
  }
  for (int i = 0; i < groups.size(); ++i) {
	delete groups[i];
  }
  for (int i = 0; i < transformObjects.size(); ++i) {
	delete transformObjects[i];
  }
}

void Scene::createStarHouse(int countStars) {
  for (int i = 0; i < objects.size(); ++i) {
	delete objects[i];
  }
  objects.clear();
  initCube(5);
  update();
}

void Scene::timerEvent(QTimerEvent* event) { Q_UNUSED(event) }

void Scene::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  initShaders();

  float step = 2.0f;

  groups.append(new Group3d);

  for (float x = -step; x <= step; x += step) {
	for (float y = -step; y <= step; y += step) {
	  for (float z = -step; z <= step; z += step) {
		initCube(1.0f);
		objects[objects.size() - 1]->translate(QVector3D(x - 4, y, z));
		groups[groups.size() - 1]->addObject(objects[objects.size() - 1]);
	  }
	}
  }
  groups[0]->translate(QVector3D(-4, 0, 0));

  groups.append(new Group3d);
  for (float x = -step; x <= step; x += step) {
	for (float y = -step; y <= step; y += step) {
	  for (float z = -step; z <= step; z += step) {
		initCube(1.0f);
		objects[objects.size() - 1]->translate(QVector3D(x + 4, y, z));
		groups[groups.size() - 1]->addObject(objects[objects.size() - 1]);
	  }
	}
  }
  groups[1]->translate(QVector3D(4, 0, 0));

  groups.append(new Group3d);
  groups[2]->addObject(groups[0]);
  groups[2]->addObject(groups[1]);

  transformObjects.append(groups[2]);
  // transformObjects.append(camera);
  // groups[0]->addObject(camera);
  skybox = new SkyBox(100, QImage(":/sprites/skybox_day3.png"));

  connect(&timer, SIGNAL(timeout()), this, SLOT(simpleAnimation()));
  timer.start(30);
}

void Scene::resizeGL(int w, int h) {
  float aspect = w / (float)h;
  projectionMatrix.setToIdentity();
  projectionMatrix.perspective(45, aspect, 0.01f, 1000.0f);
}

void Scene::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // showOrts();

  shaderSkyBox.bind();
  shaderSkyBox.setUniformValue("u_projectionMatrix", projectionMatrix);
  shaderSkyBox.setUniformValue(
	  "u_simpleColor", QVector4D(skybox->simpleColor.red(), skybox->simpleColor.green(), skybox->simpleColor.blue(), skybox->simpleColor.alphaF()));
  shaderSkyBox.setUniformValue("u_isTextureUse", skybox->is_textureUse);
  camera->draw(&shaderSkyBox);
  skybox->draw(&shaderSkyBox, context()->functions());
  shaderProgram.release();

  shaderProgram.bind();
  shaderProgram.setUniformValue("u_projectionMatrix", projectionMatrix);
  shaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
  shaderProgram.setUniformValue("u_lightPower", 1.0f);

  camera->draw(&shaderProgram);

  // for (int i = 0; i < objects.size(); ++i) {
  //  objects[i]->draw(&shaderProgram, context()->functions());
  //}

  for (int i = 0; i < transformObjects.size(); ++i) {
	transformObjects[i]->draw(&shaderProgram, context()->functions());
  }

  shaderProgram.release();
}

void Scene::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
	case Qt::Key_Tab:
	  selectNextGroupForCamera();
	  break;
	case Qt::Key_W:
	  controllers.forward = true;
	  break;
	case Qt::Key_A:
	  controllers.left = true;
	  break;
	case Qt::Key_S:
	  controllers.back = true;
	  break;
	case Qt::Key_D:
	  controllers.right = true;
	  break;
	case Qt::Key_Shift:
	  controllers.up = true;
	  break;
	case Qt::Key_Space:
	  controllers.down = true;
	  break;
  }
  update();
}

void Scene::keyReleaseEvent(QKeyEvent* event) {
  switch (event->key()) {
	case Qt::Key_W:
	  controllers.forward = false;
	  break;
	case Qt::Key_A:
	  controllers.left = false;
	  break;
	case Qt::Key_S:
	  controllers.back = false;
	  break;
	case Qt::Key_D:
	  controllers.right = false;
	  break;
	case Qt::Key_Shift:
	  controllers.up = false;
	  break;
	case Qt::Key_Space:
	  controllers.down = false;
	  break;
  }
  update();
}

void Scene::mousePressEvent(QMouseEvent* event) {
  switch (event->buttons()) {
	case Qt::RightButton:
	  mousePos = QVector2D(event->localPos());
	  break;
  }
  update();
}

void Scene::mouseMoveEvent(QMouseEvent* event) {
  switch (event->buttons()) {
	case Qt::RightButton:
	  QVector2D diff = QVector2D(event->localPos()) - mousePos;
	  mousePos = QVector2D(event->localPos());
	  float angleX = diff.y() / mouseSpeed;
	  float angleY = diff.x() / mouseSpeed;
	  camera->rotateX(QQuaternion::fromAxisAndAngle(1.0, 0.0, 0.0, angleX));
	  camera->rotateY(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, angleY));
	  update();
	  break;
  }
}

void Scene::wheelEvent(QWheelEvent* event) {
  if (event->delta() > 0) camera->translate(QVector3D(0, 0, 0.25));
  if (event->delta() < 0) camera->translate(QVector3D(0, 0, -0.25));
  ;
  update();
}

void Scene::initShaders() {
  if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertexShader.vsh")) {
	qDebug() << "Ошибка компиляции вершинного шейдера";
	close();
  }
  if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragmentShader.fsh")) {
	qDebug() << "Ошибка компиляции фрагментного шейдера";
	close();
  }
  if (!shaderProgram.link()) {
	qDebug() << "Ошибка линковки";
	close();
  }

  if (!shaderSkyBox.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/skybox.vsh")) {
	qDebug() << "Ошибка компиляции вершинного шейдера";
	close();
  }
  if (!shaderSkyBox.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/skybox.fsh")) {
	qDebug() << "Ошибка компиляции фрагментного шейдера";
	close();
  }
  if (!shaderSkyBox.link()) {
	qDebug() << "Ошибка линковки";
	close();
  }
}

void Scene::initCube(float _width) {
  float w = _width / 2.0f;
  QVector<VertexData> vertexes;

  vertexes.append(VertexData(QVector3D(-w, w, w), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
  vertexes.append(VertexData(QVector3D(-w, -w, w), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
  vertexes.append(VertexData(QVector3D(w, w, w), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
  vertexes.append(VertexData(QVector3D(w, -w, w), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));

  vertexes.append(VertexData(QVector3D(w, w, w), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(w, -w, w), QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(w, w, -w), QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(w, -w, -w), QVector2D(1.0, 0.0), QVector3D(1.0, 0.0, 0.0)));

  vertexes.append(VertexData(QVector3D(w, w, w), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(w, w, -w), QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-w, w, w), QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-w, w, -w), QVector2D(1.0, 0.0), QVector3D(0.0, 1.0, 0.0)));

  vertexes.append(VertexData(QVector3D(w, w, -w), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
  vertexes.append(VertexData(QVector3D(w, -w, -w), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
  vertexes.append(VertexData(QVector3D(-w, w, -w), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
  vertexes.append(VertexData(QVector3D(-w, -w, -w), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, -1.0)));

  vertexes.append(VertexData(QVector3D(-w, w, w), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-w, w, -w), QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-w, -w, w), QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-w, -w, -w), QVector2D(1.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));

  vertexes.append(VertexData(QVector3D(-w, -w, w), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-w, -w, -w), QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(w, -w, w), QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(w, -w, -w), QVector2D(1.0, 0.0), QVector3D(0.0, -1.0, 0.0)));

  // // верхняя грань
  // vertexes.append(VertexData(QVector3D(-w, -w, w), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
  // vertexes.append(VertexData(QVector3D(-w, w, w), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
  // vertexes.append(VertexData(QVector3D(w, w, w), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
  // vertexes.append(VertexData(QVector3D(w, -w, w), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
  // // нижняя грань
  // vertexes.append(VertexData(QVector3D(-w, -w, -w), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
  // vertexes.append(VertexData(QVector3D(-w, w, -w), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
  // vertexes.append(VertexData(QVector3D(w, w, -w), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
  // vertexes.append(VertexData(QVector3D(w, -w, -w), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
  // // передняя грань
  // vertexes.append(VertexData(QVector3D(-w, -w, -w), QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
  // vertexes.append(VertexData(QVector3D(-w, -w, w), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
  // vertexes.append(VertexData(QVector3D(w, -w, w), QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
  // vertexes.append(VertexData(QVector3D(w, -w, -w), QVector2D(1.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
  // // задняя грань
  // vertexes.append(VertexData(QVector3D(-w, w, -w), QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
  // vertexes.append(VertexData(QVector3D(-w, w, w), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
  // vertexes.append(VertexData(QVector3D(w, w, w), QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
  // vertexes.append(VertexData(QVector3D(w, w, -w), QVector2D(1.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
  // // левая грань
  // vertexes.append(VertexData(QVector3D(-w, w, -w), QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
  // vertexes.append(VertexData(QVector3D(-w, w, w), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
  // vertexes.append(VertexData(QVector3D(-w, -w, w), QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
  // vertexes.append(VertexData(QVector3D(-w, -w, -w), QVector2D(1.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
  // // правая грань
  // vertexes.append(VertexData(QVector3D(w, -w, -w), QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
  // vertexes.append(VertexData(QVector3D(w, -w, w), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
  // vertexes.append(VertexData(QVector3D(w, w, w), QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
  // vertexes.append(VertexData(QVector3D(w, w, -w), QVector2D(1.0, 0.0), QVector3D(1.0, 0.0, 0.0)));

  QVector<GLuint> indexes;

  // for (int i = 0; i < 24; i += 4) {
  // indexes.append(i + 0);
  // indexes.append(i + 1);
  // indexes.append(i + 2);
  // indexes.append(i + 0);
  // indexes.append(i + 2);
  // indexes.append(i + 3);
  // }

  for (int i = 0; i < 24; i += 4) {
	indexes.append(i + 0);
	indexes.append(i + 1);
	indexes.append(i + 2);
	indexes.append(i + 2);
	indexes.append(i + 1);
	indexes.append(i + 3);
  }

  objects.append(new Cubic(vertexes, indexes, QImage(":/sprites/TEST_QUAD.png")));
}

void Scene::showOrts() {
  glLineWidth(5);
  glBegin(GL_LINES);
  glColor4f(1.0, 0.0, 0.0, 0.3);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(10.0, 0.0, 0.0);

  glColor4f(0.0, 1.0, 0.0, 0.3);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 10.0, 0.0);

  glColor4f(0.0, 0.0, 1.0, 0.3);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 10.0);

  glEnd();
}

void Scene::selectNextGroupForCamera() {}

void Scene::simpleAnimation() {
  for (int i = 0; i < objects.size(); ++i) {
	if (i % 2 == 0) {
	  objects[i]->rotate(QQuaternion::fromAxisAndAngle(1.0, 0.0, 0.0, qSin(angleObject)));
	  objects[i]->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, qCos(angleObject)));
	} else {
	  objects[i]->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, qSin(angleObject)));
	  objects[i]->rotate(QQuaternion::fromAxisAndAngle(1.0, 0.0, 0.0, qCos(angleObject)));
	}
  }
  groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0, 0.0, 1.0, qCos(angleGroup1)));
  groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, -qSin(angleGroup1)));

  groups[1]->rotate(QQuaternion::fromAxisAndAngle(1.0, 0.0, 0.0, qCos(angleGroup1)));
  groups[1]->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, -qCos(angleGroup1)));

  groups[2]->rotate(QQuaternion::fromAxisAndAngle(1.0, 0.0, 0.0, qSin(angleMain)));
  groups[2]->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, qCos(angleMain)));

  angleObject += M_PI / 180.0f;
  angleGroup1 += M_PI / 180.0f / 2.0f;
  angleGroup2 += M_PI / 180.0f / 2.0f;
  angleMain += M_PI / 180.0f / 2.0f;

  update();
}

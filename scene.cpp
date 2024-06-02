#include "scene.hpp"

Scene::Scene(QWidget* parent) : QOpenGLWidget(parent), QOpenGLFunctions() {}

void Scene::timerEvent(QTimerEvent* event) {}

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
		objects[objects.size() - 1]->translate(QVector3D(x, y, z));
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
		objects[objects.size() - 1]->translate(QVector3D(x, y, z));
		groups[groups.size() - 1]->addObject(objects[objects.size() - 1]);
	  }
	}
  }
  groups[1]->translate(QVector3D(4, 0, 0));

  groups.append(new Group3d);
  groups[2]->addObject(groups[0]);
  groups[2]->addObject(groups[1]);

  transformObjects.append(groups[2]);
  /// connect(frameTimer, SIGNAL(timeout()), this, SLOT(update()));

  connect(&timer, SIGNAL(timeout()), this, SLOT(simpleAnimation()));
  timer.start(30);
}

void Scene::resizeGL(int w, int h) {
  float aspect = w / (float)h;
  projectionMatrix.setToIdentity();
  projectionMatrix.perspective(45, aspect, 0.01f, 100.0f);
}

void Scene::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // showOrts();

  QMatrix4x4 viewMatrix;
  viewMatrix.setToIdentity();
  viewMatrix.translate(0.0, 0.0, dz);
  viewMatrix.rotate(rotation);

  shaderProgram.bind();
  shaderProgram.setUniformValue("u_projectionMatrix", projectionMatrix);
  shaderProgram.setUniformValue("u_viewMatrix", viewMatrix);
  shaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
  shaderProgram.setUniformValue("u_lightPower", 1.0f);

  for (int i = 0; i < transformObjects.size(); ++i) {
	transformObjects[i]->draw(&shaderProgram, context()->functions());
  }
}

void Scene::mousePressEvent(QMouseEvent* event) {
  switch (event->buttons()) {
	case Qt::RightButton:
	  mousePos = QVector2D(event->localPos());
	  break;
  }
}

void Scene::mouseMoveEvent(QMouseEvent* event) {
  switch (event->buttons()) {
	case Qt::RightButton:
	  QVector2D diff = QVector2D(event->localPos()) - mousePos;
	  mousePos = QVector2D(event->localPos());
	  float angle = diff.length() / mouseSpeed;
	  QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);
	  rotation = QQuaternion::fromAxisAndAngle(axis, angle) * rotation;
	  update();
	  break;
  }
}

void Scene::wheelEvent(QWheelEvent* event) {
  if (event->delta() > 0) dz += 0.25;
  if (event->delta() < 0) dz -= 0.25;
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

  objects.append(new SimpleObject(vertexes, indexes, QImage(":/sprites/TEST_QUAD.png")));
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

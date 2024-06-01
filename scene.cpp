#include "scene.hpp"

Scene::Scene(QWidget* parent) : QOpenGLWidget(parent), QOpenGLFunctions(), texture(0), indexBuffer(QOpenGLBuffer::IndexBuffer) {}

void Scene::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  initShaders();
  initCube(1.0f);
}

void Scene::resizeGL(int w, int h) {
  float aspect = w / (float)h;
  projectionMatrix.setToIdentity();
  projectionMatrix.perspective(45, aspect, 0.1f, 10.0f);
}

void Scene::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 viewMatrix;
  viewMatrix.setToIdentity();
  viewMatrix.translate(0.0, 0.0, -5.0);
  viewMatrix.rotate(rotation);
  QMatrix4x4 modelMatrix;
  modelMatrix.setToIdentity();
  texture->bind(0);

  shaderProgram.bind();
  shaderProgram.setUniformValue("u_projectionMatrix", projectionMatrix);
  shaderProgram.setUniformValue("u_viewMatrix", viewMatrix);
  shaderProgram.setUniformValue("u_modelMatrix", modelMatrix);
  shaderProgram.setUniformValue("u_texture", 0);

  arrayBuffer.bind();

  int offset = 0;

  int vertLoc = shaderProgram.attributeLocation("a_position");
  shaderProgram.enableAttributeArray(vertLoc);
  shaderProgram.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

  offset += sizeof(QVector3D);
  int texLoc = shaderProgram.attributeLocation("a_textcoord");
  shaderProgram.enableAttributeArray(texLoc);
  shaderProgram.setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

  indexBuffer.bind();

  glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);
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

  arrayBuffer.create();
  arrayBuffer.bind();
  arrayBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData));
  arrayBuffer.release();

  indexBuffer.create();
  indexBuffer.bind();
  indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
  indexBuffer.release();

  texture = new QOpenGLTexture(QImage(":/sprites/TEST_QUAD.png").mirrored());
  texture->setMinificationFilter(QOpenGLTexture::Nearest);
  // texture->setMinificationFilter(QOpenGLTexture::);
  texture->setMagnificationFilter(QOpenGLTexture::Nearest);
  texture->setWrapMode(QOpenGLTexture::Repeat);
}

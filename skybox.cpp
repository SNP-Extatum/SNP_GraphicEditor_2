#include "skybox.hpp"

SkyBox::SkyBox(float width, const QImage& texture) {
  simpleColor = QColor(255, 150, 150);
  float w = width / 2.0f;
  QVector<VertexData> vertexes;
#define x03 0.0
#define x04 0.0
#define x1 1.0
#define x13 1.0 / 3.0
#define x23 2.0 / 3.0
#define x33 1.0
#define x14 1.0 / 4.0
#define x24 2.0 / 4.0
#define x34 3.0 / 4.0
#define x44 1.0

  // позади нас
  vertexes.append(VertexData(QVector3D(-w, w, w), QVector2D(x1, x23), QVector3D(0.0, 0.0, -1.0)));
  vertexes.append(VertexData(QVector3D(-w, -w, w), QVector2D(x1, x13), QVector3D(0.0, 0.0, -1.0)));
  vertexes.append(VertexData(QVector3D(w, w, w), QVector2D(x34, x23), QVector3D(0.0, 0.0, -1.0)));
  vertexes.append(VertexData(QVector3D(w, -w, w), QVector2D(x34, x13), QVector3D(0.0, 0.0, -1.0)));
  // справа от нас
  vertexes.append(VertexData(QVector3D(w, w, w), QVector2D(x34, x23), QVector3D(-1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(w, -w, w), QVector2D(x34, x13), QVector3D(-1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(w, w, -w), QVector2D(x24, x23), QVector3D(-1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(w, -w, -w), QVector2D(x24, x13), QVector3D(-1.0, 0.0, 0.0)));
  // над нами
  vertexes.append(VertexData(QVector3D(w, w, w), QVector2D(x24, x33), QVector3D(0.0, -1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(w, w, -w), QVector2D(x24, x23), QVector3D(0.0, -1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-w, w, w), QVector2D(x14, x33), QVector3D(0.0, -1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-w, w, -w), QVector2D(x14, x23), QVector3D(0.0, -1.0, 0.0)));
  // перед нами
  vertexes.append(VertexData(QVector3D(w, w, -w), QVector2D(x24, x23), QVector3D(0.0, 0.0, 1.0)));
  vertexes.append(VertexData(QVector3D(w, -w, -w), QVector2D(x24, x13), QVector3D(0.0, 0.0, 1.0)));
  vertexes.append(VertexData(QVector3D(-w, w, -w), QVector2D(x14, x23), QVector3D(0.0, 0.0, 1.0)));
  vertexes.append(VertexData(QVector3D(-w, -w, -w), QVector2D(x14, x13), QVector3D(0.0, 0.0, 1.0)));
  // слева от нас
  vertexes.append(VertexData(QVector3D(-w, w, w), QVector2D(x04, x23), QVector3D(1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-w, w, -w), QVector2D(x14, x23), QVector3D(1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-w, -w, w), QVector2D(x04, x13), QVector3D(1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-w, -w, -w), QVector2D(x14, x13), QVector3D(1.0, 0.0, 0.0)));
  // под нами
  vertexes.append(VertexData(QVector3D(-w, -w, w), QVector2D(x14, x03), QVector3D(0.0, 1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-w, -w, -w), QVector2D(x14, x13), QVector3D(0.0, 1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(w, -w, w), QVector2D(x24, x03), QVector3D(0.0, 1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(w, -w, -w), QVector2D(x24, x13), QVector3D(0.0, 1.0, 0.0)));

  QVector<GLuint> indexes;

  for (int i = 0; i < 24; i += 4) {
	indexes.append(i + 0);
	indexes.append(i + 2);
	indexes.append(i + 1);

	indexes.append(i + 2);
	indexes.append(i + 3);
	indexes.append(i + 1);
  }

  cube = new Cubic(vertexes, indexes, texture);
}

SkyBox::~SkyBox() { delete cube; }

void SkyBox::draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions) { cube->draw(shaderProgram, functions); }

void SkyBox::rotate(const QQuaternion& rot) { (void)rot; }

void SkyBox::translate(const QVector3D& trans) { (void)trans; }

void SkyBox::scaleIt(const float& scaling) { (void)scaling; }

void SkyBox::setGlobalTransform(const QMatrix4x4& mat) { (void)mat; }

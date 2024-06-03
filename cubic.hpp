#ifndef CUBIC_H
#define CUBIC_H

#include "transformational.hpp"

struct VertexData {
  VertexData() {}
  VertexData(QVector3D pos, QVector2D textCoord, QVector3D norm) : position(pos), textureCoord(textCoord), normal(norm) {}
  QVector3D position;
  QVector2D textureCoord;
  QVector3D normal;
};

class Cubic : public Transformational {
 public:
  Cubic();
  Cubic(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &_texture);
  ~Cubic();
  void init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &_texture);

  void draw(QOpenGLShaderProgram *shaderProgram, QOpenGLFunctions *functions) override;
  void rotate(const QQuaternion &rot) override;
  void translate(const QVector3D &trans) override;
  void scaleIt(const float &scaling) override;
  void setGlobalTransform(const QMatrix4x4 &mat) override;

 private:
  QOpenGLBuffer vertexBuffer;
  QOpenGLBuffer indexBuffer;
  QOpenGLTexture *texture;

  QQuaternion rotation;
  QVector3D translation;
  float scale;
  QMatrix4x4 globalTransform;
};

#endif  // CUBIC_H

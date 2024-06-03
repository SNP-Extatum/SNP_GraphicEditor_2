#include "cubic.hpp"

Cubic::Cubic() : indexBuffer(QOpenGLBuffer::IndexBuffer), texture(0) { scale = 1.0f; }

Cubic::Cubic(const QVector<VertexData>& vertData, const QVector<GLuint>& indexes, const QImage& _texture)
	: indexBuffer(QOpenGLBuffer::IndexBuffer), texture(0) {
  scale = 1.0f;

  init(vertData, indexes, _texture);
}

Cubic::~Cubic() {
  if (vertexBuffer.isCreated()) vertexBuffer.destroy();
  if (indexBuffer.isCreated()) indexBuffer.destroy();
  if (texture != 0) {
	if (texture->isCreated()) {
	  texture->destroy();
	}
  }
}

void Cubic::init(const QVector<VertexData>& vertData, const QVector<GLuint>& indexes, const QImage& _texture) {
  if (vertexBuffer.isCreated()) vertexBuffer.destroy();
  if (indexBuffer.isCreated()) indexBuffer.destroy();
  if (texture != 0) {
	if (texture->isCreated()) {
	  delete texture;
	  texture = 0;
	}
	simpleColor = QColor(100, 50, 120);
  }

  vertexBuffer.create();
  vertexBuffer.bind();
  vertexBuffer.allocate(vertData.constData(), vertData.size() * sizeof(VertexData));
  vertexBuffer.release();

  indexBuffer.create();
  indexBuffer.bind();
  indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
  indexBuffer.release();

  texture = new QOpenGLTexture(_texture.mirrored());
  texture->setMinificationFilter(QOpenGLTexture::Nearest);
  // texture->setMinificationFilter(QOpenGLTexture::);
  texture->setMagnificationFilter(QOpenGLTexture::Nearest);
  texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Cubic::draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions) {
  if (!vertexBuffer.isCreated() || !indexBuffer.isCreated()) return;

  if (is_textureUse) {
	texture->bind(0);
	shaderProgram->setUniformValue("u_texture", 0);
  } else {
	shaderProgram->setUniformValue(
		"u_simpleColor", QVector4D((float)simpleColor.red(), (float)simpleColor.green(), (float)simpleColor.blue(), (float)simpleColor.alphaF()));
	shaderProgram->setUniformValue("u_isTextureUse", is_textureUse);
  }
  QMatrix4x4 modelMatrix;
  modelMatrix.setToIdentity();
  modelMatrix.translate(translation);
  modelMatrix.rotate(rotation);
  modelMatrix.scale(scale);
  modelMatrix = globalTransform * modelMatrix;

  shaderProgram->setUniformValue("u_modelMatrix", modelMatrix);

  vertexBuffer.bind();

  int offset = 0;

  int vertLoc = shaderProgram->attributeLocation("a_position");
  shaderProgram->enableAttributeArray(vertLoc);
  shaderProgram->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

  offset += sizeof(QVector3D);
  int texLoc = shaderProgram->attributeLocation("a_textcoord");
  shaderProgram->enableAttributeArray(texLoc);
  shaderProgram->setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

  offset += sizeof(QVector2D);
  int normLoc = shaderProgram->attributeLocation("a_normal");
  shaderProgram->enableAttributeArray(normLoc);
  shaderProgram->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

  indexBuffer.bind();

  functions->glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);

  vertexBuffer.release();
  indexBuffer.release();
  texture->release();
}

void Cubic::rotate(const QQuaternion& rot) { rotation = rot * rotation; }

void Cubic::translate(const QVector3D& trans) { translation += trans; }

void Cubic::scaleIt(const float& scaling) { scale *= scaling; }

void Cubic::setGlobalTransform(const QMatrix4x4& mat) { globalTransform = mat; }

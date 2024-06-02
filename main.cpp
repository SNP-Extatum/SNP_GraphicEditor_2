#include <QApplication>
#include <QSurfaceFormat>

#include "window.hpp"

int main(int argc, char *argv[]) {
  QSurfaceFormat format;
  format.setSamples(16);
  format.setDepthBufferSize(24);  // пока что не уверен, что оно надо
  QSurfaceFormat::setDefaultFormat(format);

  QApplication a(argc, argv);
  Window w;
  w.show();
  return a.exec();
}

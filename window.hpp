#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Window;
}
QT_END_NAMESPACE

class Window : public QMainWindow {
  Q_OBJECT

 public:
  Window(QWidget* parent = nullptr);
  ~Window();

 protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

 private slots:
  void on_create_starHouse_triggered();

 private:
  Ui::Window* ui;
};
#endif  // WINDOW_HPP

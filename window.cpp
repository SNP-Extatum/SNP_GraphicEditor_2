#include "window.hpp"

#include "ui_window.h"

Window::Window(QWidget* parent) : QMainWindow(parent), ui(new Ui::Window) { ui->setupUi(this); }

Window::~Window() { delete ui; }

void Window::keyPressEvent(QKeyEvent* event) { ui->OpenGL_Scene->keyPressEvent(event); }

void Window::keyReleaseEvent(QKeyEvent* event) { ui->OpenGL_Scene->keyReleaseEvent(event); }

void Window::on_create_starHouse_triggered() { ui->OpenGL_Scene->createStarHouse(10); }

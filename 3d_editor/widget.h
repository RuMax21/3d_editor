#ifndef WIDGET_H
#define WIDGET_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QMatrix4x4>
#include <QtOpenGL>

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QMatrix4x4 projection_matrix;
protected:
    void initializedGL();
    void resizeGL(int w, int h);
    void paintGL();

};
#endif // WIDGET_H

#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <simpleobject3d.h>
#include <QBasicTimer>
#include <QFile>
#include <QWheelEvent>

class SimpleObject3D;
class Transformational;
class Group3D;
class Camera3D;
class SkyBox;

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void timerEvent(QTimerEvent *event);

    void initShaders();
    void initCube(float);
    void loadObj(const QString &path);


private:
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_program;
    QOpenGLShaderProgram m_programSkyBox;

    QVector2D m_mousePosition;

    QVector<SimpleObject3D *> m_objects;
    QVector<Transformational *> m_TransformObjects;
    QVector<Group3D *> m_groups;

    QBasicTimer m_timer;
    float angleObject;
    float angleGroup1;
    float angleGroup2;
    float angleMain;

    Camera3D *m_camera;
    SkyBox *m_skybox;
};

#endif // WIDGET_H

#include "widget.h"

#include <QMouseEvent>
#include <QOpenGLContext>
#include <simpleobject3d.h>
#include <QtMath>
#include <camera3d.h>
#include <QKeyEvent>
#include <skybox.h>


#pragma comment (lib, "opengl32.lib")

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setWindowTitle("3d_editor");
    m_camera = new Camera3D;
    m_camera->translate(QVector3D(0.0f, 0.0f, -5.0f));
}

Widget::~Widget()
{
    delete m_camera;

    for (int i = 0; i < m_objects.size(); ++i)
        delete m_objects[i];
}

void Widget::initializeGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();

    loadObj("C:/Users/chere/OneDrive/Рабочий стол/3d/piranha3.obj");
    m_TransformObjects.append((m_objects[m_objects.size() - 1]));

    m_skybox = new SkyBox(100,  QImage("C:/Users/chere/OneDrive/Рабочий стол/3d/skybox.jpg"));

    m_timer.start(30, this);
}


void Widget::resizeGL(int w, int h){
    float aspect = w / (float)h;

    m_projectionMatrix.setToIdentity();

    m_projectionMatrix.perspective(45, aspect, 0.01f, 1000.0f);
}

void Widget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_programSkyBox.bind();

    m_programSkyBox.setUniformValue("u_projectionMatrix", m_projectionMatrix);

    m_camera->draw(&m_programSkyBox);
    m_skybox->draw(&m_programSkyBox, context()->functions());
    m_programSkyBox.release();

    m_program.bind();

    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_program.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    m_program.setUniformValue("u_lightPower", 1.0f);

    m_camera->draw(&m_program);

    for (int i=0;i < m_TransformObjects.size(); i++){
        m_TransformObjects[i]->draw(&m_program, context()->functions());
    }
    m_program.release();
}

void Widget::mousePressEvent(QMouseEvent *event)
{

    if(event->buttons() == Qt::LeftButton){
         m_mousePosition = QVector2D(event->localPos());
    }else if(event->button() == Qt::RightButton){
        QImage tmp("C:/Users/chere/OneDrive/Рабочий стол/3d/cube.jpg");
        initCube(2.0f);
        update();
    }

    event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
     if(event->buttons() != Qt::LeftButton) return;

     QVector2D diff = QVector2D(event->localPos()) - m_mousePosition;
     m_mousePosition = QVector2D(event->localPos());

     float angle = diff.length() / 2.0;

     QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);

     m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));

     update();


}

void Widget::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0){
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.25f));
    } else if (event->angleDelta().y() < 0){
         m_camera->translate(QVector3D(0.0f, 0.0f, -0.25f));
    }
    update();
}

void Widget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    update();
}

void Widget::initShaders(){
   if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:/Users/chere/OneDrive/Рабочий стол/3d/vshader.vsh"))
           close();

   if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:/Users/chere/OneDrive/Рабочий стол/3d/fshader.fsh"))
           close();

   if(!m_program.link())
       close();


   if (!m_programSkyBox.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:/Users/chere/OneDrive/Рабочий стол/3d/skybox.vsh"))
           close();

   if (!m_programSkyBox.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:/Users/chere/OneDrive/Рабочий стол/3d/skybox.fsh"))
           close();

   if(!m_programSkyBox.link())
       close();
}

void Widget::initCube(float width){

    float width_div_2 = width / 2.0f;

    QVector<VertexData> vertexes;
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(1.0, 0.0, 0.0)));

     vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 1.0, 0.0)));

     vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
     vertexes.append(VertexData(QVector3D(width_div_2,-width_div_2, -width_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2,- width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, -1.0)));

     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));

     vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, -1.0, 0.0)));

     QVector<GLuint> indexes;

     for(int i=0; i<24; i+=4){
         indexes.append(i + 0);
         indexes.append(i + 1);
         indexes.append(i + 2);

         indexes.append(i + 2);
         indexes.append(i + 1);
         indexes.append(i + 3);
     }

     m_objects.append(new SimpleObject3D(vertexes, indexes, QImage("C:/Users/chere/OneDrive/Рабочий стол/3d/cube.jpg")));
}

void Widget::loadObj(const QString &path)
{
    QFile objFile(path);
    if (!objFile.exists()){
        qDebug() << "File not found";
        return;
    }

    objFile.open(QIODevice::ReadOnly);
    QTextStream input(&objFile);

    QVector<QVector3D> coords;
    QVector<QVector2D> texcoords;
    QVector<QVector3D> normals;

    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;

    while (!input.atEnd()){
        QString str = input.readLine();
        QStringList list = str.split(" ");

        if (list[0] == "#"){
            qDebug() << "This is comment:" << str;
            continue;
        } else if (list[0] == "mtllib"){
            qDebug() << "File with materials:" << str;
            continue;
        } else if (list[0] == "v"){
            coords.append(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
            continue;
        } else if (list[0] == "vt"){
             qDebug() << "vt " << str;
            texcoords.append(QVector2D(list[1].toFloat(), list[2].toFloat()));
            continue;
        } else if (list[0] == "vn"){
             qDebug() << "vn " << str;
            normals.append(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
            continue;
        } else if (list[0] == "f"){
             qDebug() << "f " << str;
            for (int i = 1; i <= 3; ++i){
                QStringList vert = list[i].split("/");
                vertexes.append(VertexData(coords[vert[0].toLong() - 1], texcoords[vert[1].toLong() - 1], normals[vert[2].toLong() - 1]));
                indexes.append(indexes.size());
            }
            continue;
        }
    }

     m_objects.append(new SimpleObject3D(vertexes, indexes, QImage("C:/Users/chere/OneDrive/Рабочий стол/3d/cube.jpg")));

    objFile.close();
}

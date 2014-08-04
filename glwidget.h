#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLShaderProgram>
#include <QGLWidget>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector>
#include <QVector3D>

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

public slots:
    void setRotation(const QQuaternion &rotation);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private:
    QMatrix4x4 projectionMatrix;
    QGLShaderProgram program;
    QVector<QVector3D> vertices;
    QVector<QVector3D> colors;
    QQuaternion quaternion;
};

#endif // GLWIDGET_H

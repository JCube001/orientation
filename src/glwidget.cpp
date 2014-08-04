#include "glwidget.h"

#include <QColor>

#define RED     QVector3D(1.0f, 0.0f, 0.0f)
#define GREEN   QVector3D(0.0f, 1.0f, 0.0f)
#define BLUE    QVector3D(0.0f, 0.0f, 1.0f)
#define YELLOW  QVector3D(1.0f, 1.0f, 0.0f)
#define MAGENTA QVector3D(1.0f, 0.0f, 1.0f)
#define CYAN    QVector3D(0.0f, 1.0f, 1.0f)

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
}

GLWidget::~GLWidget()
{
}

void GLWidget::setRotation(const QQuaternion &rotation)
{
    quaternion = rotation;
    updateGL();
}

void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    qglClearColor(QColor(Qt::black));

    program.addShaderFromSourceFile(QGLShader::Vertex, QStringLiteral(":/vbox.glsl"));
    program.addShaderFromSourceFile(QGLShader::Fragment, QStringLiteral(":/fbox.glsl"));
    program.link();

    // Make the cube
    vertices << QVector3D(-0.5f, -0.5f,  0.5f) << QVector3D( 0.5f, -0.5f,  0.5f) << QVector3D( 0.5f,  0.5f,  0.5f) // Front
             << QVector3D( 0.5f,  0.5f,  0.5f) << QVector3D(-0.5f,  0.5f,  0.5f) << QVector3D(-0.5f, -0.5f,  0.5f)
             << QVector3D( 0.5f, -0.5f, -0.5f) << QVector3D(-0.5f, -0.5f, -0.5f) << QVector3D(-0.5f,  0.5f, -0.5f) // Back
             << QVector3D(-0.5f,  0.5f, -0.5f) << QVector3D( 0.5f,  0.5f, -0.5f) << QVector3D( 0.5f, -0.5f, -0.5f)
             << QVector3D(-0.5f, -0.5f, -0.5f) << QVector3D(-0.5f, -0.5f,  0.5f) << QVector3D(-0.5f,  0.5f,  0.5f) // Left
             << QVector3D(-0.5f,  0.5f,  0.5f) << QVector3D(-0.5f,  0.5f, -0.5f) << QVector3D(-0.5f, -0.5f, -0.5f)
             << QVector3D( 0.5f, -0.5f,  0.5f) << QVector3D( 0.5f, -0.5f, -0.5f) << QVector3D( 0.5f,  0.5f, -0.5f) // Right
             << QVector3D( 0.5f,  0.5f, -0.5f) << QVector3D( 0.5f,  0.5f,  0.5f) << QVector3D( 0.5f, -0.5f,  0.5f)
             << QVector3D(-0.5f,  0.5f,  0.5f) << QVector3D( 0.5f,  0.5f,  0.5f) << QVector3D( 0.5f,  0.5f, -0.5f) // Top
             << QVector3D( 0.5f,  0.5f, -0.5f) << QVector3D(-0.5f,  0.5f, -0.5f) << QVector3D(-0.5f,  0.5f,  0.5f)
             << QVector3D(-0.5f, -0.5f, -0.5f) << QVector3D( 0.5f, -0.5f, -0.5f) << QVector3D( 0.5f, -0.5f,  0.5f) // Bottom
             << QVector3D( 0.5f, -0.5f,  0.5f) << QVector3D(-0.5f, -0.5f,  0.5f) << QVector3D(-0.5f, -0.5f, -0.5f);

    colors << RED     << RED     << RED     // Front
           << RED     << RED     << RED
           << GREEN   << GREEN   << GREEN   // Back
           << GREEN   << GREEN   << GREEN
           << BLUE    << BLUE    << BLUE    // Left
           << BLUE    << BLUE    << BLUE
           << YELLOW  << YELLOW  << YELLOW  // Right
           << YELLOW  << YELLOW  << YELLOW
           << MAGENTA << MAGENTA << MAGENTA // Top
           << MAGENTA << MAGENTA << MAGENTA
           << CYAN    << CYAN    << CYAN    // Bottom
           << CYAN    << CYAN    << CYAN;
}

void GLWidget::resizeGL(int width, int height)
{
    const float aspect = static_cast<float>(width) / static_cast<float>(height ? height : 1);
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(60.0f, aspect, 0.001f, 1000.0f);
    glViewport(0, 0, width, height);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;

    QMatrix4x4 cameraTransformation;
    cameraTransformation.rotate(quaternion);

    QVector3D cameraPosition = cameraTransformation * QVector3D(0.0f, 0.0f, 2.5f);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0.0f, 1.0f, 0.0f);

    viewMatrix.lookAt(cameraPosition, QVector3D(0.0f, 0.0f, 0.0f), cameraUpDirection);

    program.bind();
    program.setUniformValue("mvpMatrix", projectionMatrix * viewMatrix * modelMatrix);

    // Draw the cube
    program.setAttributeArray("vertex", vertices.constData());
    program.enableAttributeArray("vertex");

    program.setAttributeArray("color", colors.constData());
    program.enableAttributeArray("color");

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    program.disableAttributeArray("vertex");
    program.disableAttributeArray("color");
    program.release();
}

#undef RED
#undef GREEN
#undef BLUE
#undef YELLOW
#undef MAGENTA
#undef CYAN

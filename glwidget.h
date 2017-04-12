#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include "gl/glu.h"
#include "gl/glaux.h"

class GLWidget : public QGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);

private:
    void initializeGL();
    void paintGL();
    void resizeGL(int, int);

    int n;
    int k;
    QVector <QPointF> points;
    QVector <int> u;
    bool press = false;
    int cur_vert;
    int point_radius;
    QOpenGLShaderProgram *m_program;

    void initPoints();
    void initU();
    void drawPoints();
    void drawNurbs();
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    GLuint m_sizeAttr;
    GLuint m_kSize;
    //cwc::SmartPtr<cwc::TextureFactory::CreateTextureFromFile("texture.jpg");
    float N(int k, int i, float& t);
    QPointF C(float& t);

private slots:
    void mousePressEvent( QMouseEvent* event );
    void mouseReleaseEvent( QMouseEvent* event );
    void mouseMoveEvent( QMouseEvent* event );
};

#endif // GLWIDGET_H

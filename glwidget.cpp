#include "glwidget.h"
#include "QtDebug"

GLWidget::GLWidget(QWidget *parent): QGLWidget(parent){
    n = 5;
    k = 4;
    point_radius = 8;
    initPoints();
    initU();
}

void GLWidget::initializeGL()
{
    qglClearColor(Qt::black);
    //glRotated(-20,0,1,0);
    initializeOpenGLFunctions();
    m_program = new QOpenGLShaderProgram( this );
    m_program->addShaderFromSourceFile( QOpenGLShader::Vertex, ":/Lib/vShader.glsl" );
    m_program->addShaderFromSourceFile( QOpenGLShader::Fragment, ":/Lib/fShader.glsl" );
    if ( !m_program->link() )
    {
        qWarning( "Error: unable to link a shader program" );
        return;
    }
    m_posAttr = m_program->attributeLocation( "posAttr" );
    m_colAttr = m_program->uniformLocation( "colAttr" );
    m_matrixUniform = m_program->uniformLocation( "matrix" );
    m_sizeAttr = m_program->uniformLocation( "sizeAttr" );
    m_kSize = m_program->uniformLocation( "kSize");

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!m_program->bind()){
        return;
    }
    drawNurbs();
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLint)width, (GLint)height);
    //glOrtho(-100, 100, 100, -100, -100, 100);
}

void GLWidget::initPoints(){
    points.append(QPointF(-80,10));
    points.append(QPointF(-60,-40));
    points.append(QPointF(-20,-70));
    points.append(QPointF(10,0));
    points.append(QPointF(40,80));
    points.append(QPointF(80,40));
}

void GLWidget::initU()
{
    for (int i = 0; i <= (n+k+1); i++){
        if (i <= k){
            u.append(0);
        }
        else{
            if (i <= n){
                u.append(i-k+1);
            }
            else{
                u.append(n-k+2);
            }
        }
    }
}



void GLWidget::drawNurbs()
{
    QMatrix4x4 matrix;
    matrix.ortho(-100.0f,100.0f,100.0f,-100.0f,-100.0f,100.0f);
    matrix.rotate(15,1.0f,1.0f,1.0f);
    m_program->setUniformValue(m_colAttr, 1.0f,1.0f,1.0f);
    m_program->setUniformValue( m_matrixUniform, matrix );
    GLfloat p[6];
    int i=0;
    float r = 0.0f;
    float g = 0.3f;
    float b = 0.5f;
    for(float t = u.at(0); t < u.at(u.size()-1); t += 0.01,i++){
        m_program->setUniformValue(m_colAttr, r,g,b);
        r += 0.005f;
        g += 0.003f;
        b -= 0.002f;
        float x=C(t).x(),y=C(t).y();
        p[0]=x;
        p[1]=y;
        p[2]=-50.0f;
        p[3]=x;
        p[4]=y;
        p[5]=50.0f;
        glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, p );
        glEnableVertexAttribArray( m_posAttr );
        glDrawArrays(GL_LINES, 0, 2);
        glDisableVertexAttribArray(  m_posAttr );
    }

    m_program->setUniformValue(m_colAttr, 1.0f,1.0f,1.0f);
    m_program->setUniformValue( m_sizeAttr, 10.0f);
    m_program->setUniformValue(m_kSize, 2);
    GLfloat vert[18];
    for (int i = 0; i < points.size(); i++){
        vert[3*i+0]=points.at(i).x();
        vert[3*i+1]=points.at(i).y();
        vert[3*i+2]=0.0f;
    }
    glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vert );
    glEnableVertexAttribArray( m_posAttr );
    glDrawArrays( GL_POINTS, 0, 6);
    glDisableVertexAttribArray(  m_posAttr );
    m_program->release();
}

float GLWidget::N(int k, int i, float& t)
{
    if (k == 0){
        if (i == 0){
            if ((u.at(i) <= t ) && ( t <= u.at(i+1))) return 1.0f;
        }
        return (t > u.at(i) && t <= u.at(i+1)) ? 1.0f : 0;
    }
    else{
        float c1, c2;
        if (u.at(i+k) == u.at(i)){
            (t == u.at(i)) ? c1=1.0f : c1 = 0;
        }
        else{
            c1 = (t - u.at(i)) / (u.at(i + k) - u.at(i));
        }
        if (u.at(i+k+1) == u.at(i+1)){
            (t == u.at(i+k+1)) ? c2=1.0f : c2 = 0;
        }
        else{
            c2 = (u.at(i+k+1) - t) / (u.at(i+k+1)-u.at(i+1));
        }
        return c1 * N(k-1,i,t) + c2 * N(k-1,i+1, t);
    }
}

QPointF GLWidget::C(float& t){
    QPointF res(0,0);
    for (int i = 0; i <= n; i++){
        res += N(k, i, t)*points.at(i);
    }
    return res;
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    float ex = (event->pos().x() - 250) / 2.5;
    float ey = (event->pos().y() - 250) / 2.5;
    ex=ex/cos(M_PI/9);
    float r = 15 / 2.5;
    for(int i = 0; i < points.size(); i++){
        if((ex >= (points.at(i).x() - r)) &&
                (ex <= (points.at(i).x() + r)) &&
                (ey >= (points.at(i).y() - r)) &&
                (ey <= (points.at(i).y() + r))){
            press = true;
            cur_vert = i;
        }
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    press = false;
    updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(press){
        float x = (event->pos().x() - 250)/2.5;
        float y = (event->pos().y() - 250)/2.5;
        points.replace(cur_vert, QPointF(x,y));
    }
    updateGL();
}

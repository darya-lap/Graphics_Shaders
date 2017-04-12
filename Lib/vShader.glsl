attribute highp vec4 posAttr;
uniform highp mat4 matrix;
uniform lowp vec3 colAttr;
uniform lowp float sizeAttr;
varying lowp vec4 col;
varying vec2 vTexCoord;

void main()
{

    col = vec4(colAttr,1.0f);
    gl_Position = matrix * posAttr;
    gl_PointSize=sizeAttr;
    vTexCoord = matrix * posAttr;
}

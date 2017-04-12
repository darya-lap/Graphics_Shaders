varying lowp vec4 col;

//void main()
//{
//    gl_FragColor = col;
//}

const int MAX_KOEFF_SIZE = 32; //максимальный размер ядра (массива коэффициентов)

uniform sampler2D texture; //размываемая текстура
uniform int kSize; //размер ядра
uniform float koeff[MAX_KOEFF_SIZE]; //коэффициенты
uniform vec2 direction; //направление размытия с учетом радиуса размытия и aspect ratio, например (0.003, 0.0) - горизонтальное и (0.0, 0.002) - вертикальное

varying vec2 vTexCoord; //текстурные координаты текущего фрагмента

void main() {
    koeff[0] = 0.01f;
    for (int i = 1; i < MAX_KOEFF_SIZE; i++){
        koeff[i] = koeff[i-1]+0.02f;
    }
    direction = vec2 (0.003,0.0);
        vec4 sum = vec4(0.0); //результирующий цвет

        vec2 startDir = -0.5*direction*float(kSize-1); //вычисляем начальную точку размытия
        for (int i=0; i<kSize; i++) //проходимся по всем коэффициентам
                sum += texture2D(texture, vTexCoord + startDir + direction*float(i)) * koeff[i]; //суммируем выборки

        gl_FragColor = sum;
}

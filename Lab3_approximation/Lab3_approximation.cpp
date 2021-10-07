#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

double fi(double a, double k);
double F(double a, double kt, double k);
vector <double> coord(double a, double b, double kt, double k);
double sum(double a, double b, vector <double> vect, int n, double kt);
double sum2(double a, double b, double kt, double k);
double sum3(double a, double b, vector <double> vect, double kt, double k);
int N(double a, double b, double k);
void DROW(double a, double b, int n, double a1, double a0, double kt, double k);

int main()
{
    double a, b, sumx, sumy, sumx2, sumxy, n, a0, a1;
    double k = 0.01, kt = 0.00001;
    vector <double> vecty;
    cin >> a >> b;

    vecty = coord(a, b, kt, k);

    n = N(a, b, k);

    sumx = (a + b) / 2.0 * n;
    sumy = sum(a, b, vecty, n, kt);
    sumx2 = sum2(a, b, kt, k);
    sumxy = sum3(a, b, vecty, kt, k);

    a1 = (sumxy - sumx * sumy / n) / (sumx2 - sumx * sumx / n);  //(sumxy * n - sumx * sumy) / (sumx2 * n - sumx);  //  sumx2 > sumx, n >= 1
    a0 = (sumy - sumx * a1) / n;                            // n >= 1

    if(a0 >= 0)
    cout << a1 << "x + " << a0;
    else cout << a1 << "x - " << fabs(a0);

    DROW(a, b, n, a1, a0, kt, k);

    return 0;
}

double F(double a, double kt, double k)
{
    double fx =  a * sin (a) ;
    if (k - fabs(fx) > kt)
        return fi(fx, kt);
    return fx;
}

vector <double> coord(double a, double b, double kt, double k)
{
    vector<double> vect;
    for (double i = a; i - b <= kt; i += k)
    {
       // if (fabs(i) < k)
         //   i = fi(i, kt);
        vect.push_back(F(i, kt, k));        //log(atan((2 * i + 3) / (i + 3) + (6 * i - 1) / (2 - 2 * i)) + 3.1415926535 / 2));
    }
    return vect;
}

double sum(double a, double b, vector <double> vect, int n, double kt)
{
    double sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += vect[i];
    }
    sum = fi(sum, kt);
    return sum;
}

double sum2(double a, double b, double kt, double k)
{
    double sum = 0;
    for (double i = a; i <= b; i += k)
    {
        if (fabs(i) < k)
            i = fi(i, k);
        sum += i * i;
    }
    sum = fi(sum, kt);
    return sum;
}

double sum3(double a, double b, vector <double> vect, double kt, double k)
{
    double sum = 0;
    int j = 0;
    for (double i = a; i - b <= kt; i += k)
    {
   //     if (fabs(i) - k < kt)
     //       i = fi(i, k);
        sum += i * vect[j];
        j++;
    }
    sum = fi(sum, kt);
    return sum;
}

int N(double a, double b, double k)
{
    if (a >= 0 && b >= 0)
        return int((b - a) / k) + 1;
    else //if (a <= 0 && b >= 0)
        return int((-a + b) / k) + 1;
}

double fi(double a, double kt)
{
    if (fabs(a) - kt < kt)
        return 0.0;//int(a * 1000) / 1000.0;
    else return a;
}

void DROW(double a, double b, int n, double a1, double a0, double kt, double k)
{
    RenderWindow window(sf::VideoMode(800, 800), "My window");

    VertexArray func(PrimitiveType::LineStrip, 0);
    for (double x = a; x - b <= kt; x += k)
    {
        func.append(Vertex(Vector2f(x, -1 * F(x, kt, k))));
    }

    double yl = (a1 * a + a0), yr = (a1 * b + a0);

    VertexArray aproxf(Lines, 2);
    aproxf[0].position = Vector2f(a, -yl);
    aproxf[0].color = Color::Yellow;
    aproxf[1].position = Vector2f(b, -yr);
    aproxf[1].color = Color::Green;

    /*VertexArray aproxf1(PrimitiveType::LineStrip, 0);
    for (double x = a; x - b <= kt; x += k / 100.0)
    {
        aproxf1.append(Vertex(Vector2f(x, -1 * (a1 * x + a0) + 10)));
    }*/
   
    int m = max(int(n * k) / 2, max(abs(int(yl)), abs(int(yr))));

    VertexArray fx(Lines, 2);
    fx[0].position = Vector2f(-2 * m , 0);
    fx[0].color = Color::Blue;
    fx[1].position = Vector2f(2 * m, 0);
    fx[1].color = Color::Blue;

    VertexArray fy(Lines, 2);
    fy[0].position = Vector2f(0, -2 * m);
    fy[0].color = Color::Blue;
    fy[1].position = Vector2f(0, 2 * m);
    fy[1].color = Color::Blue;

    View view(FloatRect(-2 * m, -2 * m, 4 * m, 4 * m));
    window.setView(view);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear(Color::Black);

        window.draw(fx);
        window.draw(fy);
        window.draw(func);
        window.draw(aproxf);

        window.display();
    }
}
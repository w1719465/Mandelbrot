// abs complex example
#include <iostream>     // std::cout
#include <complex>      // std::complex, std::abs
#include "ComplexPlane.h"
#include <SFML/Graphics.hpp>
#include <tuple>

//const int width = 1920;
//const int height = 1080;

const int width = 300;
const int height = 380;


const int N3 = (int)(pow(256, 3));

std::tuple<int, int, int> n_to_rgb(int n, int max_iter)
{
    //normalize n first so : n is between 0 and 1
    long double nn = (long double)n / (long double)max_iter;
    int N = 2526; // number of possiblr values for an RGB element (8 bits)

    n = (int)(nn * (long double)N3);
    int b = n / (N * N);
    int nn2 = n - b * N * N;
    int r = nn2 / N;
    int g = nn2 - r * N;
    //return std::tuple<int, int, int>(100, 100, 100);
    return std::tuple<int, int, int>(r, g, b);
}
void gen_mandelbrot(sf::VertexArray& va, int shift_x, int shift_y, int max_iter, float zoom)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            long double x = ((long double)j - shift_x) / zoom;
            long double y = ((long double)i - shift_y) / zoom;

            std::complex<long double> c(x,y);
            int n = 0; // number iteration

            std::complex<long double> z(0.0, 0.0); //z=0+0i

            for (int k = 0; k < max_iter; k++)
            {
                std::complex<long double> z2(0.0, 0.0);
                z2.real(real(z) * real(z) - imag(z) * imag(z));
                z2.imag(2 * real(z) * imag(z));

                z.real(real(z2) + real(c)); // C + z
                z.imag(imag(z2) + imag(c)); // C + z

                n++;

                if(real(z) * real(z) + imag(z) * imag(z) > 4) { 
                  //  cout << otv << " " << n << " k= " << k << endl;
                    break;}
            }
            std::tuple<int, int, int> temp= n_to_rgb(n, max_iter);
            va[i * width + j].position = sf::Vector2f((float)j, (float)i);
            sf::Color color(std::get<0>(temp), std::get<1>(temp), std::get<2>(temp));
            va[i * width  + j].color = color;
        }

    }
}

int main()
{   
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML window");
    window.setFramerateLimit(30);
    sf::VertexArray pixels(sf::Points, width * height);

    float zoom = 200.0f;
    int max_iter = 500;
    int shift_x = width / 2;
    int shift_y = height / 2;
    
    gen_mandelbrot(pixels, shift_x, shift_y, max_iter, zoom);

    // Start the game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            cout << "mouse clicked" << endl;
            //recLevel++;
            //double 
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            //float x1 = event.mouseButton.x;
            //float y1 = event.mouseButton.y;
            shift_x -= pos.x - shift_x;
            shift_y -= pos.y - shift_y;
            //shift_x -=(int)(pos.x - shift_x);
            //shift_y -= (int)(pos.y - shift_y);
            zoom *= 2;
            max_iter += 200;//
            for (int i = 0; i < width*height; i++)
            {
                pixels[i].color = sf::Color::Black;
            }
            gen_mandelbrot(pixels, shift_x, shift_y, max_iter, zoom);
        }/**/
        // Clear screen
        window.clear();

        // Draw the string
        window.draw(pixels);

        // Update the window
        window.display();
    }
    

    
    return EXIT_SUCCESS;


    /*
    double re = -0.77568377;
    double im = 0.13646737;
    complex<double> c(re, im);
    complex<double> z(0, 0);
    cout << "c = " << c << "\t";
    cout << "|c| = " << abs(c) << endl;
    cout << "Real component of c: " << c.real() << endl;
    cout << "Imaginary component of c: " << c.imag() << endl;
    z = z * z + c;
    cout << "z_2 = " << z << "\t";
    cout << "|z_2| = " << abs(z) << endl;
    z = z * z + c;
    cout << "z_3 = " << z << "\t";
    cout << "|z_3| = " << abs(z) << endl;
    */
}
// abs complex example
#include <iostream>     // std::cout
#include <complex>      // std::complex, std::abs
#include "ComplexPlane.h"
#include <SFML/Graphics.hpp>

int max_iter = MAX_ITER;
double min_re = -2.5, max_re = 1;
double min_im = -1, max_im = 1;
double zoom = 1.0;


Color linear_interpolation(const Color& v, const Color& u, double a)
{
    auto const b = 1 - a;
    return Color(b * v.r + a * u.r, b * v.g + a * u.g, b * v.b + a * u.b);
}

int main()
{   
    //sf::View view;
    unsigned int width = 300;// sf::VideoMode::getDesktopMode().width;
    unsigned int height = 300;// sf::VideoMode::getDesktopMode().height;
    int aspect_ratio = height / width;

    ComplexPlane cp = ComplexPlane((float)aspect_ratio); 
    sf::Font font;
    if (!font.loadFromFile("./KOMIKAP_.ttf"))
        return EXIT_FAILURE;
    sf::Text text;
    text.setCharacterSize(15);
    text.setFillColor(Color::White);
    text.setFont(font);

    sf::FloatRect textRect = text.getLocalBounds();
    sf::VertexArray va;
    
    vector<Vector2f> vertices;
    vector<Vector2f> points;

    sf::VideoMode vm(height, width); 
    sf::RenderWindow window(vm, "Mandelbrot Set", Style::Default);

    Texture texture;
    Sprite sprite;
    Image image;
    image.create(width, height);
   
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (event.MouseButtonPressed)
        {
            sf::Vector2f mouse = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
            cp.setMouseLocation(mouse);
            auto zoom_x = [&](double z)
            {
                double cr = min_re + (max_re - min_re) * event.mouseButton.x / width;
                double ci = min_im + (max_im - min_im) * event.mouseButton.y / height;
                double tminr = cr - (max_re - min_re) / 2 / z;
                max_re = cr + (max_re - min_re) / 2 / z;
                min_re = tminr;
                double tmini = ci - (max_im - min_im) / 2 / z;
                max_im = ci + (max_im - min_im) / 2 / z;
                min_im = tmini;
            };
            
            if (event.mouseButton.button == sf::Mouse::Right)
            {                
                cp.zoomOut();
                zoom_x(1.0 / 5);
                zoom *= 5;
            }

            if (event.mouseButton.button == sf::Mouse::Left)
            {
                cp.zoomIn();
                zoom_x(5);
                zoom *= 5;
            }
        }

        if(Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        
        window.clear();

        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
            {
                double cr = min_re + (max_re - min_re) * x / width;
                double ci = min_im + (max_im - min_im) * y / height;
                double re = 0, im = 0;
                int iter;
                for (iter = 0; iter < max_iter; iter++)
                {
                    double tr = re * re - im * im + cr;
                    im = 2 * re * im + ci;
                    re = tr;
                    if (re * re + im * im > (2 * 2)) { break; }
                }
                Uint8 rr;
                Uint8 gg;
                Uint8 bb;
                cp.iterationsToRGB(iter, rr, gg, bb);
                int r = 1.0 * (max_iter - iter) / max_iter * 0xff;
                int g = r, b = r;

                static const vector<Color> colors
                {
                    {0,0,0},
                    {213,67,31},
                    {251,255,89},
                    {43,30,218},
                    {0,255,247}
                };

                static const auto max_color =colors.size() - 1;
                if (iter == max_iter) { iter = 0; }
                double mu = 1.0 * iter / max_iter;
                mu *= max_color;
                auto i_mu = static_cast<size_t>(mu);
                auto color1 = colors[i_mu];
                auto color2 = colors[(i_mu + 1, max_color)];
                Color color = linear_interpolation(color1, color2, mu - i_mu);

               image.setPixel(x, y, Color(color));//image.setPixel(x, y, Color(r, g, b));
            }
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        window.draw(sprite);

        cp.loadText(text);
        window.draw(text);

        window.display();
    }
    return EXIT_SUCCESS;
}
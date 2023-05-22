#include "ComplexPlane.h"

using namespace sf;
using namespace std;



ComplexPlane::ComplexPlane(float aspectRatio)
{
    m_aspectRatio = aspectRatio;
    m_view.setSize(BASE_WIDTH, -BASE_HEIGHT * m_aspectRatio);
    m_view.setCenter(0.0, 0.0);
    m_zoomCount = 0;
    zoom_in_out = "";
    ss_center = "Center: (0,0)";
    ss_cursor = "Cursor: (0,0)";
}
void ComplexPlane::zoomIn()
{
    m_zoomCount++;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_view.setSize(x, y);
    zoom_in_out = "Left-click to Zoom in";
}
void ComplexPlane::zoomOut()
{
    m_zoomCount--;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_view.setSize(x, y);
    zoom_in_out = "Rhight-click to Zoom out";
}
void ComplexPlane::setCenter(Vector2f coord)
{
    m_view.setSize(coord);
    stringstream ss;
    ss << "Center: (" << coord.x << "," << coord.y << ")";
    ss_center = ss.str();
}
View ComplexPlane::getView()
{
    return m_view;
}
void ComplexPlane::setMouseLocation(Vector2f coord)
{
    m_mouseLocation = coord;
    stringstream ss;
    ss << "Cursor: (" << coord.x << "," << coord.y << ")";
    ss_cursor = ss.str();
}
void ComplexPlane::loadText(Text& text)
{
    stringstream ss;
    ss << "Mandelbrot Set" << endl;
    ss << ss_center << endl;
    ss << ss_cursor << endl;
    ss << zoom_in_out;
    text.setString(ss.str());
}
size_t ComplexPlane::countIterations(Vector2f coord)
{
    double min_re = -2.5, max_re = 1;
    double min_im = -1, max_im = 1;
    
    //double cr = min_re + (max_re - min_re) * x / width;
    //double ci = min_im + (max_im - min_im) * y / height;
    double re = 0, im = 0;
    int iter;
    for (iter = 0; iter < MAX_ITER; iter++)
    {
      //  double tr = re * re - im * im + cr;
       // im = 2 * re * im + ci;
       // re = tr;
        if (re * re + im * im > 2 * 2) { break; }
    }
    return iter;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    int a = count / MAX_ITER;
    r = a*(MAX_ITER - count) / MAX_ITER & 0xff;
    g = (r >> 8) & 0xff;// a* (MAX_ITER - count) / MAX_ITER;// *9xff;
    b = a*(MAX_ITER - count) / MAX_ITER & 0xff;
}

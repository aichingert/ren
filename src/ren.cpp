#include "window.cpp"

extern "C" void init_window(int _width, int _height, const char* title) {
    create_html(title);
    create_js();
}

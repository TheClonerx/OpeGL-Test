#include <OpenGL/GL.hpp> // must be included before glfw

#include <Application.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

[[noreturn]] static void glfwErrorCallback(int code, const char* str)
{
    throw std::runtime_error("ERROR:GLFW:" + std::to_string(code) + ":" + str);
}

int main()
{

    try {
        glfwSetErrorCallback(glfwErrorCallback);
        glfwInit();
        Application app;
        app.setup();

        double last_update = glfwGetTime();
        double delta = 0;

        while (app.window().is_open()) {
            app.update(delta);

            if (app.needs_redraw()) {
                app.render();
            }

            app.window().update();
            app.window().display();

            double now = glfwGetTime();
            delta = now - last_update;
            last_update = now;
        }

        glfwTerminate();

    } catch (std::system_error& e) {
        std::cerr << "Unhandled std::system_error:\n\t";
        std::cerr << e.code().category().name() << ": (" << e.code().value() << ") " << e.code().message() << std::endl;
        exit(EXIT_FAILURE);
    } catch (std::exception& e) {
        std::cerr << "Unhandled std::exception:\n\t";
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    } catch (...) {
        std::cerr << "Unhandled unknown exception, rethrowing." << std::endl;
        throw;
    }
}

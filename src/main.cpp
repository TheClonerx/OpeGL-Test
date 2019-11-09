#include "Application.hpp"
#include <iostream>
#include <string_view>
#include <system_error>
#include <vector>

int main()
{

    try {
        Application app;
        app.start();
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
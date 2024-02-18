#include "first_app.hpp"

namespace huhu {

    void FirstApp::run() {
        while(!huhuWindow.shouldClose()) {
            glfwPollEvents();
        }
    }
}
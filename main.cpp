#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>

using namespace std;
using namespace sf;

int main(int argc, char** argv) {
    RenderWindow window(VideoMode(1280/2, 720/2), "Teste");
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();

        window.clear();
        window.display();
    }
    return 0;
}
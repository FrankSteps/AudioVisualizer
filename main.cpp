/*
    Este código foi criado por Terminal Root
    Link: https://www.youtube.com/watch?v=04c8YR7dhP4

    O objetivo deste código é me dar a possibilidade e um pontapé inicial para meus estudos da transformada de fourier aplicado na computação
    Este código será usado para ampliar e melhorar meus código futuros e foi postado bo github para eu não perder este código.

    Conforme meu conhecimento em C++ e em matemática forem ampliando, irei melhorando este projeto conforme minhas necessidades
*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fftw3.h>  //Sou apaixonado pela transformada de Fourier <3
#include <memory>
#include <cmath>

using namespace std;
using namespace sf;

int main(int argc, char** argv) {
    RenderWindow window(VideoMode(1280/2, 720/2), argv[1]);
    window.setFramerateLimit(60);
    
    if(argc < 2){
        cerr << "usage: " << argv[0] << " file.[mp3|wav]\n";
        return EXIT_FAILURE;
    }

    SoundBuffer buffer;
    if(!buffer.loadFromFile(argv[1])){
        cerr << "Failed to load audio file\n";
        return EXIT_FAILURE;
    }

    Sound sound(buffer);
    sound.play();

    const int sample_size = 1024;
    fftw_complex * in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * sample_size);
    fftw_complex * out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * sample_size);

    fftw_plan plan = fftw_plan_dft_1d(sample_size, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    vector<float> spectrum(sample_size / 2);

    const sf::Int16* samples = buffer.getSamples();

    size_t sample_count = buffer.getSampleCount();
    size_t channels = buffer.getChannelCount();
    size_t current_sample = 0;


    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)){
            if (event.type == Event::Closed){
                window.close();
            }
        }
        if(sound.getStatus() == Sound::Stopped){
            break;
        }
        for (int i = 0; i < sample_size; ++i){
            size_t index = (current_sample + i) % (sample_count / channels);
            in[i][0] = samples[index * channels] / 32768.0;
            in[i][1] = 0.0;
        }

        fftw_execute(plan);
        for(int i = 0; i < sample_size / 2; ++i){
            spectrum[i] = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
        }
        current_sample += sample_size;

        window.clear(Color::Black);

        float centerX = window.getSize().x / 2.f;
        float centerY = window.getSize().y / 2.f;

        // metade esquerda (do centro para a direita, espelhada verticalmente)
        for(int i = 0; i < 50; ++i){
            RectangleShape bar;
            bar.setSize(Vector2f(2, spectrum[i] * 2.f));
            bar.setPosition(centerX + i * 6.f, centerY);
            bar.setFillColor(Color::Magenta);
            bar.setRotation(180); 
            window.draw(bar);

            bar.setSize(Vector2f(2, -(spectrum[i] * 2.f)));
            window.draw(bar);
        }

        // metade esquerda (do centro para a esquerda, espelhada verticalmente)
        for(int i = 0; i < 50; ++i){
            RectangleShape bar;
            bar.setSize(Vector2f(2, spectrum[i] * 2.f));
            bar.setPosition(centerX - i * 6.f, centerY); // mesma Y do centro
            bar.setFillColor(Color::Magenta);
            bar.setRotation(180);
            window.draw(bar);

            bar.setSize(Vector2f(2, -(spectrum[i] * 2.f)));
            window.draw(bar);
        }
        window.display();
    }
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
    return EXIT_SUCCESS;
}

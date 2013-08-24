//COPYRIGHT
//{
//SFML
//----
//
//SFML - Copyright (C) 2007-2013 Laurent Gomila - laurent.gom@gmail.com
//
//This software is provided 'as-is', without any express or
//implied warranty. In no event will the authors be held
//liable for any damages arising from the use of this software.
//
//Permission is granted to anyone to use this software for any purpose,
//including commercial applications, and to alter it and redistribute
//it freely, subject to the following restrictions:
//
//1. The origin of this software must not be misrepresented;
//   you must not claim that you wrote the original software.
//   If you use this software in a product, an acknowledgment
//   in the product documentation would be appreciated but
//   is not required.
//
//2. Altered source versions must be plainly marked as such,
//   and must not be misrepresented as being the original software.
//
//3. This notice may not be removed or altered from any
//   source distribution.
//
//
//
//External libraries used by SFML
//-------------------------------
//
//* OpenAL-Soft is under the LGPL license
//* libsndfile is under the LGPL license
//* libjpeg is public domain
//* stb_image and stb_image_write are public domain
//* freetype is under the FreeType license or the GPL license
//* GLEW is under the modified BSD License, the Mesa 3-D License (MIT License), and the Khronos License (MIT License)
//}

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/OpenGL.hpp"
#include "SFML/window.hpp"
#include "SFML/Audio.hpp"
#include <iostream>
#include <cmath>
#include <fstream>
#include "include/math.h"
#include <cstring>

using namespace std;

const string _font_filename = "data//font//Corbel.ttf";
const string _game_icon = "data//icon.png";
const string _console_welcome = "Welcome to the game console. For help type help and press enter";
sf::Font _font;
int _font_size=24, _font_console_size=15;
string _game_name = "Particle_TEST";
sf::Time _frame_interval = sf::milliseconds(16.66666);
bool _paused , _fullscreen, _console;
vector<sf::Texture> _textures;
int _game_width = 800, _game_height = 600;
int* W = &_game_width, *H = &_game_height;
float _player_move_speed = .5, _e = .1, _gravity = 0;
V2 _mouse_position;
vector<string> _console_input;
struct ball {
    V2 v, p;
    float w, r;
    ball() {
    }
    ball(V2 _p) {
        p = _p;
    }
    ball(V2 _p, float _r) {
        p = _p;
        r = _r;
    }
};
vector<ball> _particles;
int _log = 20;

char _player_name[50];
unsigned int _player_name_ind;

float rand01() {
    return (float)rand()/RAND_MAX;
}

void InitParticles() {
    int n = 500;
    for(int i=0; i<n; i++) {
        _particles.push_back(ball(V2(rand01()**W, rand01()**H),rand01()*3+10));
    }
}

bool Init(sf::Window &window) {
    if (!_font.loadFromFile(_font_filename)) {
        return false;
    }
    ifstream fi("data\\DATA");
    string s;
    while(fi>>s) {
        s="data\\"+s + ".png";
        _textures.push_back(sf::Texture());
        if (!_textures.back().loadFromFile(s)) {
            return false;
        }
    }
    for(int i=0; i<_textures.size(); i++)
        _textures[i].setSmooth(true);
    sf::Image icon;
    if (!icon.loadFromFile(_game_icon)) {
        return false;
    }
    window.setIcon(256,256,icon.getPixelsPtr());
    InitParticles();
    _console_input.push_back(string());
    return true;
}

//functions from http://www.cplusplus.com/articles/D9j2Nwbp/

void reverse(string &s) {
    int i=0, j= s.size()-1;
    char c;

    for (; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(INT64 n, string &s) {
    int i;
    INT64 sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s+=(char(n % 10 + '0'));   /* get next digit */
        i++;
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s+=('-');
    s+=('\0');
    reverse(s);
}

//end functions
sf::Time ttime, frame_time;
int _frames;

void console(sf::RenderWindow *window) {
    if(_console) {
        sf::Text text;
        text.setFont(_font);
        text.setCharacterSize(_font_console_size);
        text.setColor(sf::Color::White);
        text.setPosition(sf::Vector2f(0,*H-_font_console_size-1));
        int n = _console_input.size();
        for(int i=0; i<min(_log,n); i++) {
            text.setPosition(sf::Vector2f(0,*H-_font_console_size*(i+1)-1));
            text.setString("> " + _console_input[n-1-i]);
            window->draw(text);
        }
    }
}

void render(sf::RenderWindow *window) {
    _frames++;
    sf::Clock frame;
    sf::Time one_frame = frame.getElapsedTime();
    window->clear(sf::Color::Green);
    sf::Text text;
    text.setFont(_font);
    text.setString(_game_name);
    text.setCharacterSize(_font_size);
    text.setColor(sf::Color::White);
    sf::RectangleShape c;
    c.setTexture(&_textures[0]);
    //c.setFillColor(sf::Color(10,10,10,100));
    int l = 1, m = _textures.size();
    for(int i=0; i<_particles.size(); i++) {
        if(i>100*(l%m)) {
            c.setTexture(&_textures[l]);
            l++;
            if(l==m)l=0;
        }
        ball b = _particles[i];
        c.setSize(sf::Vector2f(b.r*2,b.r*2));
        c.setPosition(sf::Vector2f(b.p.x - b.r, b.p.y - b.r));
        window->draw(c);
    }
    window->draw(text);
    string buffer;
    ttime = (-one_frame + frame.getElapsedTime());
    itoa((ttime.asMicroseconds()),buffer);
    text.setString(buffer);
    text.move(sf::Vector2f(0,20));
    window->draw(text);
    sf::CircleShape mouse = sf::CircleShape(5, 10);
    mouse.move(_mouse_position);
    window->draw(mouse);
    console(window);
    window->display();
}

void key_press() {
    bool player_moved = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player_moved = true;
        _particles[0].v.x-=_player_move_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player_moved = true;
        _particles[0].v.x+=_player_move_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        player_moved = true;
        _particles[0].v.y-=_player_move_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player_moved = true;
        _particles[0].v.y+=_player_move_speed;
    }
}

void move_player() {
}

void move_objects() {
    for(int i=0; i<_particles.size(); i++) {
        ball *b = &_particles[i];
        b->v.y += _gravity;
        b->p += b->v;
    }
}

void impulse_response(ball &b1, ball &b2) {
    float m1 = b1.r, m2 = b2.r;
    V2 v2 =b2.v, v1 =b1.v;
    V2 n = (b2.p - b1.p).norm();
    V2 vp2 = v2, vp1 = v1;
    V2 vr = vp2 - vp1;
    float j = (vr.dot(n) * (-1 - _e))/
              (1/m1 + 1/m2);
    if(j<0)
        return;
    b1.v-= n*j/m1;
    b2.v+= n*j/m2;
}

void separate_objects(ball &b1, ball &b2) {
    float m1 = b1.r, m2 = b2.r;
    float d = (m1+m2 - sqdist(b1.p, b2.p))/2;
    V2 n = (b2.p - b1.p).norm();
    b1.p-= n*d*m2/m1;
    b2.p+= n*d*m1/m2;
}

void check_objectsN_N() {
    for(int i=0; i<_particles.size(); i++) {
        ball* b1 = &_particles[i];
        for(int j=0; j<_particles.size(); j++) {
            if(i==j)continue;
            ball* b2 = &_particles[j];
            float d = dist(b1->p, b2->p);
            float sumr = b1->r+b2->r;
            if(d<sumr * sumr) {
                separate_objects(*b1,*b2);
                impulse_response(*b1,*b2);
            }
        }
    }
}

void check_objects() {
    for(int i=0; i<_particles.size()-1; i++) {
        ball* b1 = &_particles[i];
        for(int j=i+1; j<_particles.size(); j++) {
            ball* b2 = &_particles[j];
            float d = dist(b1->p, b2->p);
            float sumr = b1->r+b2->r;
            if(d<sumr * sumr) {
                separate_objects(*b1,*b2);
                impulse_response(*b1,*b2);
            }
        }
    }
}

void check_wall() {
    for(int i=0; i<_particles.size(); i++) {
        ball* b = &_particles[i];
        bool hitwall = false;
        if(b->p.x+b->r>*W) {
            b->p.x = *W - b->r;
            if(b->v.x>0)
                b->v.x *= -1;
        }
        if(b->p.x-b->r<0) {
            b->p.x = b->r;
            if(b->v.x<0)
                b->v.x *= -1;
        }
        if(b->p.y-b->r<0) {
            b->p.y = b->r;
            if(b->v.y<0)
                b->v.y *= -1;
        }
        if(b->p.y+b->r>*H) {
            b->p.y = *H - b->r;
            if(b->v.y>0)
                b->v.y *= -1;
        }
    }
}

void move_to_mouse1() {
    float speed = 0.051;
    for(int i=0; i<_particles.size(); i++) {
        ball* b = &_particles[i];
        V2 dir = _mouse_position - b->p;
        dir = dir.norm();
        b->v += dir*speed;
    }
}

void move_to_mouse() {
    float speed = 0.5;
    for(int i=0; i<_particles.size(); i++) {
        bool in_range = false;
        ball* b = &_particles[i];
        V2 dir = _mouse_position - b->p;
        float d = sqdist(V2(0,0),dir);
        if(d<100)in_range = true;
        dir = dir.norm();
        if(in_range)
            b->v -= dir*speed*(100-d)/3;
        else
            b->v += dir*speed;
    }
}

void work() {
    key_press();
    //move_player();
    int n = 5;
    move_to_mouse();
    move_objects();
    for(int i=0; i<n; i++) {
        //check_wall();
        check_objects();
    }
}

void fullscreen(int fullscreen, sf::RenderWindow *window) {
    _fullscreen = fullscreen;
    window->create(sf::VideoMode(_game_width, _game_height), _game_name,
                   (_fullscreen ? sf::Style::Fullscreen : sf::Style::Default));
}

void quit() {
    exit(0);
}

void write_help(string s) {
    _console_input.push_back(string(s));
}

bool strcmp(string a1, string a2){
    for(int i=0;i<a2.size();i++){
        if(a2[i]!=a1[i])
            return false;
    }
    return true;
}

void solve_console(string s, sf::RenderWindow *window) {
    if(s=="fullscreen") {
        fullscreen(!_fullscreen, window);
        return;
    }
    if(s=="exit" || s=="quit") {
        window->close();
        quit();
        return;
    }
    if(s=="reset") {
        _particles.clear();
        InitParticles();
        return;
    }
    if(s=="pause") {
        _paused = !_paused;
        return;
    }
    if(strcmp(s,"font_size")) {
        string number = s.substr(10,s.size()-10);
        int n=15;
        n = atoi(number.c_str());
        _font_console_size = n;
        if(_font_console_size<10){
            write_help("console size cannot be smalled than 5");
            write_help("reseting to normal font size 15");
            _font_console_size = 15;
        }
        if(_font_console_size>50){
            write_help("console size cannot be bigger than 50");
            write_help("reseting to normal font size 15");
            _font_console_size = 15;
        }
        return;
    }
    if(s=="help") {
        write_help("-----------");
        write_help("fullscreen");
        write_help("exit || quit");
        write_help("reset");
        write_help("font_size");
        write_help("pause");
        write_help("-----------");
        return;
    }
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(_game_width, _game_height), _game_name, sf::Style::Default, settings);
    if(!Init(window))
        return 0;
    //window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    sf::Clock frame;
    sf::Time one_frame = frame.getElapsedTime();
    while (window.isOpen()) {
        sf::Vector2i m = sf::Mouse::getPosition(window);
        _mouse_position = V2(m.x,m.y);

        sf::Time one_frame = frame.getElapsedTime();

        sf::Event event;
        //event checking
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::KeyPressed: {
                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    window.close();
                    quit();
                    break;
                case sf::Keyboard::Tilde:
                    _console = !_console;
                    break;
                default:
                    break;
                }
            }

            case sf::Event::MouseButtonPressed: {
                switch (event.mouseButton.button) {
                case sf::Mouse::Left:
                    break;
                default:
                    break;
                }
                break;
            }

            case sf::Event::TextEntered: {
                if(event.text.unicode == '`') {
                    _console_input.clear();
                    _console_input.push_back(_console_welcome);
                    _console_input.push_back(string());
                    continue;
                } else
                if(event.text.unicode == 13) {
                    solve_console(_console_input.back(), &window);
                    _console_input.push_back(string());
                    if(_console_input.size()>_log) {
                        _console_input.erase(_console_input.begin());
                    }
                    continue;
                } else
                if (event.text.unicode>=32 && event.text.unicode < 128) {
                } else
                if(event.text.unicode == 8) {
                    //remove = true;
                    continue;
                }
                if(_console) {
                    _console_input.back() += (static_cast<char>(event.text.unicode));
                }
                break;
            }

            case sf::Event::Closed: {
                window.close();
                break;
            }

            case sf::Event::Resized: {
                _game_height = event.size.height;
                _game_width = event.size.width;
                break;
            }

            case sf::Event::LostFocus: {
                _paused = true;
                break;
            }

            case sf::Event::GainedFocus: {
                _paused = false;
                break;
            }

            default:
                break;
            }
        }

        render(&window);
        if(!_paused){
            work();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)){
            if(_console_input.back().size()>0) {
                _console_input.back() = _console_input.back().substr(0,_console_input.back().size()-1);
            }
        }
        if( frame.getElapsedTime() - one_frame < _frame_interval) {
            sf::sleep(_frame_interval + one_frame - frame.getElapsedTime());
        }
    }
    return 0;
}

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
//#include "SFML/OpenGL.hpp"
#include "SFML/Window.hpp"
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
bool _paused  , _fullscreen, _console = true;
vector<sf::Texture> _textures;
int _game_width = 1200, _game_height = 600;
int* W = &_game_width, *H = &_game_height;
float _player_move_speed = .3, _e = .1, _gravity = 0;
V2 _mouse_position;
vector<string> _console_input;
enum _obj_types {CIRCLE=0, FISH, SPIRAL, STAR, BULLET, ICON};
int _obj_num = 4, _spread = 10;
struct ball {
    bool destroy;
    int act_time, int_time;
    int time;
    int mass;
    _obj_types type;
    V2 v, p;
    float w, r;
    ball() {
        time = -1;
        destroy=false;
    }
    ball(V2 _p) {
        time = -1;
        destroy=false;
        p = _p;
    }
    ball(V2 _p, float _r) {
        time = -1;
        destroy=false;
        p = _p;
        r = _r;
        mass = r;
    }
};
vector<ball> _particles;
int _log = 20, _types = 4;
int _iterations= 1;
bool _bullet_col = true;
float _bullet_radius = 5;
float _bullet_time = 9;

char _player_name[50];
unsigned int _player_name_ind;

float rand01() {
    return (float)rand()/RAND_MAX;
}

float rand01(float a, float b) {
    return (float)rand()/RAND_MAX*(b-a) + a;
}

void AddParticles() {
    _particles.push_back(ball(V2(rand01()**W, rand01()**H),rand01()*3+5));
    _particles.back().type = _obj_types(rand()%_types);
}

void InitParticles() {
    int n = 300;
    for(int i=0; i<n; i++) {
        AddParticles();
    }
}

bool Init(sf::Window &window) {
    if (!_font.loadFromFile(_font_filename)) {
        return false;
    }
    ifstream fi("data//DATA");
    string s;
    while(fi>>s) {
        s="data//"+s + ".png";
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

void itoa(sf::Int64 n, string &s) {
    int i;
    sf::Int64 sign;

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
    c.setFillColor(sf::Color(50,50,50,150));
    for(int j=0; j<_textures.size(); j++) {
        c.setTexture(&_textures[j]);
        for(int i=0; i<_particles.size(); i++) {
            ball b = _particles[i];
            if(b.type != j)continue;
            c.setSize(sf::Vector2f(b.r*2,b.r*2));
            c.setPosition(sf::Vector2f(b.p.x - b.r, b.p.y - b.r));
            window->draw(c);
        }
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

void shoot_close_range(V2 dir, float spd){
    float b = _bullet_radius;
    M2 r;
    float dis = _particles[0].r*3;
    V2 p = _particles[0].p;
    float t = _bullet_time;
    V2 disv = _particles[0].v;
    _particles.push_back(ball(p+dir*dis+disv,rand01(b-+b/10,+b/10)));
    _particles.back().type = BULLET;
    _particles.back().v = dir*rand01(spd-spd/5,spd+spd/5)  + disv;
    _particles.back().time = rand01(t-t/5,t+t/5);
    _particles.back().mass = 30;
    //
    for(int i=1;i<_spread;i++){
    r = r.rot(4*i);
    _particles.push_back(ball(p+r * dir*dis+disv,rand01(b-+b/10,+b/10)));
    _particles.back().type = BULLET;
    _particles.back().v = r * dir*rand01(spd-spd/5,spd+spd/5)+ disv;
    _particles.back().time = rand01(t-t/5,t+t/5);
    _particles.back().mass = 30;
    }
    //
    for(int i=1;i<_spread;i++){
    r = r.rot(-4*i);
    _particles.push_back(ball(p+r * dir*dis+disv,rand01(b-+b/10,+b/10)));
    _particles.back().type = BULLET;
    _particles.back().v = r * dir*rand01(spd-spd/5,spd+spd/5)+ disv;
    _particles.back().time = rand01(t-t/5,t+t/5);
    _particles.back().mass = 30;
    }
}

void key_press() {
    bool player_moved = false;
    bool shoot_turn = false;
    V2 dir;
    float move_speed = _player_move_speed /_iterations;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player_moved = true;
        _particles[0].v.x-=move_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player_moved = true;
        _particles[0].v.x+=move_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        player_moved = true;
        _particles[0].v.y-=move_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player_moved = true;
        _particles[0].v.y+=move_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        shoot_turn = true;
        dir.x-=1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        shoot_turn = true;
        dir.x+=1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        shoot_turn = true;
        dir.y-=1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        shoot_turn = true;
        dir.y+=1;
    }
    if(shoot_turn){
            if(dir.magn()>0.00001)
        shoot_close_range(dir,5./_iterations);
    }
}

void move_player() {
}

void move_objects() {
    for(int i=0; i<_particles.size(); i++) {
        ball *b = &_particles[i];
        if(b->time>0)b->time--;
        else if(b->time==0) {
            _particles.erase(_particles.begin()+i);
            i--;
            continue;
        }
        b->v.y += _gravity;
        b->v-=b->v/20;
        b->p += b->v;
    }
}

pair<V2,V2> impulse_response(ball b1, ball b2) {
    float m1 = b1.mass, m2 = b2.mass;
    V2 v2 =b2.v, v1 =b1.v;
    V2 n = (b2.p - b1.p).norm();
    V2 vp2 = v2, vp1 = v1;
    V2 vr = vp2 - vp1;
    float j = (vr.dot(n) * (-1 - _e))/
              (1/m1 + 1/m2);
    if(j<0)
        return make_pair(V2(),V2());
    return make_pair( n*-j/m1, n*j/m1);
}

pair<V2,V2> separate_objects(ball &b1, ball &b2) {
    float r1 = b1.r, r2 = b2.r;
    float m1 = b1.mass, m2 = b2.mass;
    float d = (r1+r2 - sqdist(b1.p, b2.p))/2;
    V2 n = (b2.p - b1.p).norm();
    return make_pair(n*d*-m2/m1,n*d*m1/m2);
}

bool conditions(ball &b1, ball &b2){
    if(b1.type == BULLET && b2.type == BULLET)return _bullet_col;
    if(b1.type == BULLET){
        b2.destroy = true;
        return _bullet_col;
    }
    if(b2.type == BULLET){
        b1.destroy = true;
        return _bullet_col;
    }
    return true;
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
    vector<pair<V2,int> > P, V;
    P.assign(_particles.size(),make_pair(V2(),0));
    V.assign(_particles.size(),make_pair(V2(),0));
    for(int i=0; i<_particles.size()-1; i++) {
        ball* b1 = &_particles[i];
        for(int j=i+1; j<_particles.size(); j++) {
            ball* b2 = &_particles[j];
            float d = dist(b1->p, b2->p);
            float sumr = b1->r+b2->r;
            if(d<sumr * sumr) {
                if(conditions(*b1,*b2) == false)continue;
                pair<V2,V2> pr = separate_objects(*b1,*b2);
                P[i].first += pr.first;
                P[j].first += pr.second;
                P[i].second++;
                P[j].second++;
                pair<V2,V2> vr =impulse_response(*b1,*b2);
                V[i].first += vr.first;
                V[j].first += vr.second;
                V[i].second++;
                V[j].second++;
            }
        }
    }
    for(int i=0;i<_particles.size();i++){
        if(P[i].second!=0){
            //P[i].first/=(float)P[i].second;
            _particles[i].p+=P[i].first;
        }
        if(V[i].second!=0){
            //V[i].first/=(float)V[i].second;
            _particles[i].v+=V[i].first;
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
                b->v.x *= -_e;
        }
        if(b->p.x-b->r<0) {
            b->p.x = b->r;
            if(b->v.x<0)
                b->v.x *= -_e;
        }
        if(b->p.y-b->r<0) {
            b->p.y = b->r;
            if(b->v.y<0)
                b->v.y *= -_e;
        }
        if(b->p.y+b->r>*H) {
            b->p.y = *H - b->r;
            if(b->v.y>0)
                b->v.y *= -_e;
        }
    }
}

void move_to_mouse() {
    float speed = 0.2/_iterations;
    V2 player_pos = _particles[0].p;
    for(int i=1; i<_particles.size(); i++) {
        ball* b = &_particles[i];
        V2 dir = player_pos - b->p- b->v;
        switch (b->type) {
        case SPIRAL: {
            float d = sqdist(V2(0,0),dir);
            dir = dir.norm();
            if(d<100)
                b->v -= dir*speed*(100-d)/3;
            else
                b->v += dir*speed;
            break;
        }
        case CIRCLE: {
            dir = dir.norm();
            b->v += dir*speed;
            break;
        }
        case STAR: {
            dir = player_pos - b->p- b->v*10;
            dir = dir.norm();
            b->v += dir*speed;
            break;
        }
        case FISH: {
            dir = V2(1-rand01()*2,1-rand01()*2);
            b->v += dir*speed*3;
            break;
        }
        default:
            break;
        }
    }
}

void shoot() {
    AddParticles();
    _particles.back().p = _particles[0].p;
    _particles.back().type = BULLET;
    _particles.back().v = (_particles.back().p - _particles[1].p).norm() * 10;
    _particles.back().time = 100;
}

void delete_part() {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        shoot();
    }
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        AddParticles();
    }
}

void destroy_part(){
    for(int i=1;i<_particles.size();i++){
        if(_particles[i].destroy == true){
            //_particles.erase(_particles.begin() + i);
            //i--;
        }
    }
}

void work() {
    //move_player();
    int n = _iterations;
    for(int i=0; i<n; i++) {
        key_press();
        move_to_mouse();
        move_objects();
        check_wall();
        check_objects();
    }
    destroy_part();
    //delete_part();
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

bool strcmp(string a1, string a2) {
    for(int i=0; i<a2.size(); i++) {
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
    if(strcmp(s,"set")) {
        string number = s.substr(4,s.size()-4);
        int n=50;
        n = atoi(number.c_str());
        _particles.clear();
        for(int i=0;i<n;i++){
            AddParticles();
        }
        return;
    }
    if(strcmp(s,"add")) {
        string number = s.substr(4,s.size()-4);
        int n=50;
        n = atoi(number.c_str());
        for(int i=0;i<n;i++){
            AddParticles();
        }
        return;
    }
    if(strcmp(s,"font")) {
        string number = s.substr(5,s.size()-5);
        int n=15;
        n = atoi(number.c_str());
        _font_console_size = n;
        if(_font_console_size<10) {
            write_help("console size cannot be smalled than 5");
            write_help("reseting to normal font size 15");
            _font_console_size = 15;
        }
        if(_font_console_size>50) {
            write_help("console size cannot be bigger than 50");
            write_help("reseting to normal font size 15");
            _font_console_size = 15;
        }
        return;
    }
    if(strcmp(s,"change")) {
        string number = s.substr(7,s.size()-7);
        int n=0;
        n = atoi(number.c_str());
        if(n<0) {
            write_help("number must be bigger than -1");
            n = 0;
        }
        if(n>_obj_num) {
            string nr;
            itoa(_obj_num,nr);
            write_help("number must be smaller than " + nr);
            n = _obj_num-1;
        }
        for(int i=0; i<_particles.size(); i++) {
            if(_particles[i].type<=(_obj_types)_obj_num)_particles[i].type=(_obj_types)n;
        }
        return;
    }
    if(strcmp(s,"spread")) {
        string number = s.substr(7,s.size()-7);
        int n=0;
        n = atoi(number.c_str());
        _spread = n;
        return;
    }
    if(strcmp(s,"bullet")) {
        _bullet_col = !_bullet_col;
        return;
    }
    if(s=="help") {
        write_help("-----------");
        write_help("fullscreen");
        write_help("exit || quit");
        write_help("reset");
        write_help("pause");
        write_help("set");
        write_help("add");
        write_help("font");
        write_help("change");
        write_help("spread");
        write_help("bullet");
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
                case sf::Keyboard::LControl:
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
                if(event.text.unicode == 13) {
                    solve_console(_console_input.back(), &window);
                    _console_input.push_back(string());
                    if(_console_input.size()>_log) {
                        _console_input.erase(_console_input.begin());
                    }
                    continue;
                } else if (event.text.unicode>=32 && event.text.unicode < 128) {
                } else if(event.text.unicode == 8) {
                    if(_console_input.back().size()>0) {
                            _console_input.back() = _console_input.back().substr(0,_console_input.back().size()-1);
                    }
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
        if(!_paused) {
            work();
        }else{
            sf::sleep(sf::milliseconds(60));
        }
        if( frame.getElapsedTime() - one_frame < _frame_interval) {
            sf::sleep(_frame_interval + one_frame - frame.getElapsedTime());
        }
    }
    return 0;
}

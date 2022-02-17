#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <malloc.h>

#include <string>
#include <iostream>
#include <chrono>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
using namespace chrono;

#include "AvocadoMath.h"
#include "AvocadoRenderer.h"
#include "AvocadoInput.h"

using frame = duration<int32_t, ratio<1, 60>>;
using ms = duration<float, milli>;

#pragma region AVOCADO

namespace Avocado
{
    //기본 변수들
    bool running = true;
    SDL_Renderer *renderer;

    //아보카도 스테이트
    class AvocadoState
    {
    public:
        virtual void Init()
        {
        }

        virtual void Render()
        {
        }

        virtual void Update()
        {
        }
    };

    AvocadoState *state;

    void SetState(AvocadoState *_state)
    {
        state = _state;
        state->Init();
    }

    class AvocadoSound
    {
    private:
        Mix_Music *gMusic = NULL;

    public:
        AvocadoSound(const char *_path)
        {
            loadMedia(_path);
        }

        void loadMedia(const char *_path)
        {
            gMusic = Mix_LoadMUS(_path);
            if (gMusic == NULL)
            {
                printf("error: Failed to load music\n");
                return;
            }
        }

        void setVolume(int volume)
        {
            Mix_VolumeMusic(volume);
        }

        void playMusic()
        {
            Mix_PlayMusic(gMusic, -1);
        }

        void resumeMusic()
        {
            if (Mix_PausedMusic() == 1)
                Mix_ResumeMusic();
        }

        void pauseMusic()
        {
            if (!(Mix_PausedMusic() == 1))
                Mix_PauseMusic();
        }

        void stopMusic()
        {
            Mix_HaltMusic();
            free(this);
        }
    };

    //아보카도 이미지
    class AvocadoImage
    {
    private:
        SDL_Texture *image;

    public:
        SDL_Rect crop;

        AvocadoImage(const char *_path)
        {
            SDL_Surface *tempSurface = IMG_Load(_path);
            image = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface);
            crop = {0, 0, tempSurface->w, tempSurface->h};
        }

        AvocadoImage(const char *_path, int x, int y, int w, int h)
        {
            SDL_Surface *tempSurface = IMG_Load(_path);
            image = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface);
            crop = {x, y, w, h};
        }

        SDL_Texture *GetImage()
        {
            return image;
        }
    };

    //아보카도 카메라
    class AvocadoCamera
    {
    private:
    public:
        AvocadoMath::Vector *position = new AvocadoMath::Vector(0, 0, 1);
        double rotation = 0;
    };

    //기본 카메라와 윈도우 선언
    AvocadoCamera *camera;
    SDL_Window *window;

    //아보카도 오브젝트
    class AvocadoObject
    {
    private:
        AvocadoImage *sprite;
        bool flipX = false, flipY = false;
        bool visible = true;
        double rotation = 0;

    public:
        //폰트
        TTF_Font *font;
        AvocadoRenderer::AvocadoColor *color = new AvocadoRenderer::AvocadoColor(255, 255, 255);
        char *text;

        bool isRender = true;

        char *renderMode;

        // render mode setting
        void setRenderMode(const char *_renderMode)
        {
            renderMode = (char *)malloc(sizeof(char) * strlen(_renderMode));
            renderMode = (char *)_renderMode;
        }

        double width, height, renderWidth, renderHeight;
        AvocadoMath::Vector *position = new AvocadoMath::Vector(0, 0);
        AvocadoMath::Vector *renderPosition = new AvocadoMath::Vector(0, 0);

        AvocadoObject(double _x, double _y, double _width, double _height)
        {
            position->SetTransform(_x, _y);
            width = _width;
            height = _height;

            setRenderMode("image");
        }

        virtual void Render();

        virtual void engineRender();

        virtual void Update(){};

#pragma region GETTER_SETTER
        void SetSprite(AvocadoImage *image)
        {
            sprite = image;
        }

        AvocadoImage *GetSprite()
        {
            return sprite;
        }

        void SetWidth(int _width)
        {
            width = _width;
        }
        void SetHeight(int _height)
        {
            height = _height;
        }
        int const GetWidth()
        {
            return width;
        }
        int const GetHeight()
        {
            return height;
        }
        int const GetRenderWidth()
        {
            return renderWidth;
        }
        int const GetRenderHeight()
        {
            return renderHeight;
        }
        bool const GetFlipX()
        {
            return flipX;
        }
        bool const GetFlipY()
        {
            return flipY;
        }
        void SetFlipX(bool _flipX)
        {
            flipX = _flipX;
        }
        void SetFlipY(bool _flipY)
        {
            flipY = _flipY;
        }

        bool const GetVisible()
        {
            return visible;
        }
        void SetVisible(bool _visible)
        {
            visible = _visible;
        }

        bool const GetIsRender()
        {
            return isRender;
        }

        void SetRotation(double _rotation)
        {
            rotation = _rotation;
        }

        double const GetRotation()
        {
            return rotation;
        }

        void Rotate(double _rotation)
        {
            rotation += _rotation;
        }

#pragma endregion
    };

    //렌더 오브젝트 큐
    std::vector<AvocadoObject> renderObjects;

    // put object on render queue
    void AvocadoObject::Render()
    {
        renderObjects.push_back(*this);
    }

    void Update()
    {
        if (state != nullptr)
        {
            state->Update();
        }
    }
    void Render()
    {
        if (state != nullptr)
        {
            state->Render();
        }
    }

    void KeyUpdate()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_MOUSEMOTION)
            {
                AvocadoInput::mousePosition->SetTransform(event.motion.x, event.motion.y);
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                    AvocadoInput::mouseLeft = true;
                if (event.button.button == SDL_BUTTON_RIGHT)
                    AvocadoInput::mouseRight = true;
                if (event.button.button == SDL_MOUSEWHEEL_NORMAL)
                {
                }
                // AvocadoInput::mouseCenter = true;
            }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                AvocadoInput::mouseLeft = true;
            }
            if (event.type == SDL_KEYUP)
            {
                char _key[10] = {};
                _key[0] = event.key.keysym.sym;

                AvocadoInput::removeInput(_key);
            }

            if (event.type == SDL_KEYDOWN)
            {
                char _key[10] = {};
                _key[0] = event.key.keysym.sym;

                if (event.key.keysym.sym == SDLK_UP)
                    memmove(&_key, &"up", sizeof(char) * 5);
                if (event.key.keysym.sym == SDLK_DOWN)
                    memmove(&_key, &"down", sizeof(char) * 5);
                if (event.key.keysym.sym == SDLK_LEFT)
                    memmove(&_key, &"left", sizeof(char) * 5);
                if (event.key.keysym.sym == SDLK_RIGHT)
                    memmove(&_key, &"right", sizeof(char) * 5);

                if (event.key.keysym.sym == SDLK_TAB)
                    memmove(&_key, &"tab", sizeof(char) * 5);
                if (event.key.keysym.sym == SDLK_ALTERASE)
                    memmove(&_key, &"alt", sizeof(char) * 5);
                if (event.key.keysym.sym == SDLK_LSHIFT)
                    memmove(&_key, &"lshift", sizeof(char) * 5);
                if (event.key.keysym.sym == SDLK_RSHIFT)
                    memmove(&_key, &"rshift", sizeof(char) * 5);
                if (event.key.keysym.sym == SDLK_LCTRL)
                    memmove(&_key, &"lctrl", sizeof(char) * 5);
                if (event.key.keysym.sym == SDLK_RCTRL)
                    memmove(&_key, &"rctrl", sizeof(char) * 5);

                if (event.key.keysym.sym == SDLK_ESCAPE)
                    memmove(&_key, &"ecs", sizeof(char) * 5);
                if (event.key.keysym.sym == SDLK_SPACE)
                    memmove(&_key, &"space", sizeof(char) * 5);

                if (event.key.keysym.sym == SDLK_F1)
                    memmove(&_key, &"f1", sizeof(char) * 2);
                if (event.key.keysym.sym == SDLK_F2)
                    memmove(&_key, &"f2", sizeof(char) * 2);
                if (event.key.keysym.sym == SDLK_F3)
                    memmove(&_key, &"f3", sizeof(char) * 2);
                if (event.key.keysym.sym == SDLK_F4)
                    memmove(&_key, &"f4", sizeof(char) * 2);
                if (event.key.keysym.sym == SDLK_F5)
                    memmove(&_key, &"f5", sizeof(char) * 2);
                if (event.key.keysym.sym == SDLK_F6)
                    memmove(&_key, &"f6", sizeof(char) * 2);
                if (event.key.keysym.sym == SDLK_F7)
                    memmove(&_key, &"f7", sizeof(char) * 2);
                if (event.key.keysym.sym == SDLK_F8)
                    memmove(&_key, &"f8", sizeof(char) * 2);
                if (event.key.keysym.sym == SDLK_F9)
                    memmove(&_key, &"f9", sizeof(char) * 2);
                if (event.key.keysym.sym == SDLK_F10)
                    memmove(&_key, &"f10", sizeof(char) * 3);
                if (event.key.keysym.sym == SDLK_F11)
                    memmove(&_key, &"f11", sizeof(char) * 3);
                if (event.key.keysym.sym == SDLK_F12)
                    memmove(&_key, &"f12", sizeof(char) * 3);
                AvocadoInput::addInput(_key);
            }
        }
    }

    bool compareZ(const AvocadoObject &o1, const AvocadoObject &o2)
    {
        return o1.position->GetZ() < o2.position->GetZ();
    }

    void engineRender()
    {
        // sort the queue by z position
        sort(renderObjects.begin(), renderObjects.end(), compareZ);

        for (int i = 0; i < renderObjects.size(); i++)
        {
            renderObjects[i].engineRender();
        }

        renderObjects.clear();
    }

    // main method
    int CreateWindow(const char *title, int width, int height)
    {
        running = true;
        camera = new AvocadoCamera();

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
            return 1;
        }

        TTF_Init();

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
        if (!window)
        {
            printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
            return 1;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            printf("SDL_mixer could not initialize! SDL_mixer Error : &s\n", Mix_GetError());
            return 1;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        if (!renderer)
        {
            printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
            return 1;
        }

        time_point<steady_clock> fpsTimer(steady_clock::now());
        frame FPS{};

        while (running)
        {
            FPS = duration_cast<frame>(steady_clock::now() - fpsTimer);
            if (FPS.count() >= 1)
            {
                fpsTimer = steady_clock::now();
                // cout << "LastFrame: " << duration_cast<ms>(FPS).count() << "ms  |  FPS: " << FPS.count() * 60 << endl;

                SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
                SDL_RenderClear(renderer);
                KeyUpdate();
                Update();
                Render();
                engineRender();
            }
            SDL_RenderPresent(renderer);
        }

        TTF_Quit();

        return 0;
    }

}

#pragma endregion

#pragma region AVOCADO_RENDER_FUNCTIONS

void renderStretchImage(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Texture *texture,
                        double angle, SDL_Rect src, SDL_RendererFlip flip = SDL_FLIP_NONE)
{
    SDL_Rect dst;
    SDL_Point center;

    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;

    center.x = w / 2;
    center.y = h / 2;

    SDL_RenderCopyEx(renderer, texture, &src, &dst, angle, &center, flip);
}

void renderStretchFont(const char *str, int x, int y, TTF_Font *font,
                       double angle, AvocadoRenderer::AvocadoColor *color, SDL_RendererFlip flip = SDL_FLIP_NONE)
{
    SDL_Color _color = {color->GetRed(), color->GetGreen(), color->GetBlue(), color->GetAlpha()};

    SDL_Surface *surface = TTF_RenderText_Blended(font, str, _color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Avocado::renderer, surface);
    SDL_Rect src;
    SDL_Point center;

    src.x = src.y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
    SDL_FreeSurface(surface);

    SDL_Rect dst = {x, y, (int)(surface->w * Avocado::camera->position->GetZ()), (int)(surface->h * Avocado::camera->position->GetZ())};

    center.x = surface->w / 2;
    center.y = surface->h / 2;

    SDL_RenderCopyEx(Avocado::renderer, texture, &src, &dst, angle, &center, flip);
    // SDL_RenderCopy(Avocado::renderer, texture, NULL, &r);
    SDL_DestroyTexture(texture);
}

void renderStretchRect(int x, int y, int width, int height, double angle, AvocadoRenderer::AvocadoColor *color, SDL_RendererFlip flip = SDL_FLIP_NONE)
{

    Avocado::AvocadoImage *_image = new Avocado::AvocadoImage("defaultShapes/rect.png");

    SDL_Rect src, dst;
    SDL_Point center;

    src.x = src.y = 0;
    SDL_QueryTexture(_image->GetImage(), NULL, NULL, &src.w, &src.h);
    SDL_SetTextureColorMod(_image->GetImage(), color->GetRed(), color->GetGreen(), color->GetBlue());
    SDL_SetTextureAlphaMod(_image->GetImage(), color->GetAlpha());

    dst.x = x;
    dst.y = y;
    dst.w = width;
    dst.h = height;

    center.x = width / 2;
    center.y = height / 2;

    SDL_RenderCopyEx(Avocado::renderer, _image->GetImage(), &src, &dst, angle, &center, flip);

    free(_image);
}

void SetColor(AvocadoRenderer::AvocadoColor *color)
{
    SDL_SetRenderDrawColor(Avocado::renderer, color->GetRed(), color->GetGreen(), color->GetBlue(), color->GetAlpha());
}
void SetColor(int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(Avocado::renderer, r, g, b, a);
}
void SetColor(int r, int g, int b)
{
    SDL_SetRenderDrawColor(Avocado::renderer, r, g, b, 255);
}
AvocadoRenderer::AvocadoColor *GetColor()
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(Avocado::renderer, &r, &g, &b, &a);
    AvocadoRenderer::AvocadoColor *_result = new AvocadoRenderer::AvocadoColor(r, g, b, a);

    return _result;
}

#pragma endregion

#pragma region ENGINE_RENDER

void _renderCalculatedObj(Avocado::AvocadoObject *_obj, const char *renderMode)
{

    double fx = 1, fy = 1;

    int Width, Height;
    SDL_GetWindowSize(Avocado::window, &Width, &Height);

    _obj->renderWidth = _obj->GetWidth() * Avocado::camera->position->GetZ();
    _obj->renderHeight = _obj->GetHeight() * Avocado::camera->position->GetZ();

    double _ww = _obj->renderWidth / 2;
    double _hh = _obj->renderHeight / 2;

    if (_obj->GetFlipX())
    {
        fx = -1;
        _ww *= -1;
    }
    if (_obj->GetFlipY())
    {
        fy = -1;
        _hh *= -1;
    }

    _obj->renderWidth *= fx;
    _obj->renderHeight *= fy;

    double _dist = AvocadoMath::GetDistance(
        new AvocadoMath::Vector(Width / 2 + Avocado::camera->position->GetX(), Height / 2 + Avocado::camera->position->GetY()),
        new AvocadoMath::Vector(_obj->position->GetX(), _obj->position->GetY()));
    double _rot = atan2(Height / 2 + Avocado::camera->position->GetY() - _obj->position->GetY(),
                        Width / 2 + Avocado::camera->position->GetX() - _obj->position->GetX()) +
                  Avocado::camera->rotation;
    double xx = (_obj->position->GetX() - (Width / 2 + Avocado::camera->position->GetX()));
    double yy = (_obj->position->GetY() - (Height / 2 + Avocado::camera->position->GetY()));
    double _zDist = _dist * (Avocado::camera->position->GetZ());

    double _zx = (cos(_rot) * _zDist), _zy = (sin(_rot) * _zDist);

    _obj->renderPosition->SetX((_obj->position->GetX() - Avocado::camera->position->GetX() - _ww - (xx + _zx)));
    _obj->renderPosition->SetY((_obj->position->GetY() - Avocado::camera->position->GetY() - _hh - (yy + _zy)));

    SDL_GetWindowSize(Avocado::window, &Width, &Height);

    double _lS = _obj->GetWidth();
    if (_obj->GetWidth() < _obj->GetHeight())
        _lS = _obj->GetHeight();

    double _lsS = Width;
    if (Width < Height)
        _lsS = Height;

    _obj->isRender = true;
    if (GetDistance(_obj->renderPosition, new AvocadoMath::Vector(Width / 2, Height / 2)) >= _lS + _lsS)
    {
        _obj->isRender = false;
        return;
    }

    if (!strcmp(renderMode, "image"))
        renderStretchImage(Avocado::renderer, _obj->renderPosition->GetX(), _obj->renderPosition->GetY(), _obj->renderWidth, _obj->renderHeight, _obj->GetSprite()->GetImage(), (_obj->GetRotation() + Avocado::camera->rotation) / M_PI * 180, _obj->GetSprite()->crop, SDL_FLIP_NONE);

    if (!strcmp(renderMode, "font"))
        renderStretchFont(_obj->text, _obj->renderPosition->GetX(), _obj->renderPosition->GetY(), _obj->font, (_obj->GetRotation() + Avocado::camera->rotation) / M_PI * 180, _obj->color, SDL_FLIP_NONE);

    if (!strcmp(renderMode, "rect"))
        renderStretchRect(_obj->renderPosition->GetX(), _obj->renderPosition->GetY(), _obj->GetWidth(), _obj->GetHeight(), (_obj->GetRotation() + Avocado::camera->rotation) / M_PI * 180, _obj->color, SDL_FLIP_NONE);
}

void RenderImage(Avocado::AvocadoImage *image, double _x, double _y, double _width, double _height)
{
    Avocado::AvocadoObject *obj = new Avocado::AvocadoObject(_x, _y, _width, _height);
    obj->SetSprite(image);

    Avocado::renderObjects.push_back(*obj);
}

void RenderImage(Avocado::AvocadoImage *image, double _x, double _y, double _z, double _width, double _height)
{
    Avocado::AvocadoObject *obj = new Avocado::AvocadoObject(_x, _y, _width, _height);
    obj->SetSprite(image);
    obj->position->SetZ(_z);

    Avocado::renderObjects.push_back(*obj);
}

void RenderText(const char *str, TTF_Font *font, double _x, double _y)
{
    int w, h;
    TTF_SizeText(font, str, &w, &h);
    Avocado::AvocadoObject *obj = new Avocado::AvocadoObject(_x, _y, w, h);

    obj->color = GetColor();
    obj->font = font;

    obj->text = (char *)malloc(strlen(str));
    obj->text = (char *)str;
    obj->setRenderMode("font");

    Avocado::renderObjects.push_back(*obj);
}

void RenderText(const char *str, TTF_Font *font, double _x, double _y, double _z)
{
    int w, h;
    TTF_SizeText(font, str, &w, &h);
    Avocado::AvocadoObject *obj = new Avocado::AvocadoObject(_x, _y, w, h);

    obj->position->SetZ(_z);

    obj->color = GetColor();
    obj->font = font;

    obj->text = (char *)malloc(strlen(str));
    obj->text = (char *)str;
    obj->setRenderMode("font");

    Avocado::renderObjects.push_back(*obj);
}

void RenderRect(double _x, double _y, double _width, double _height)
{
    Avocado::AvocadoObject *obj = new Avocado::AvocadoObject(_x, _y, _width, _height);

    obj->color = GetColor();
    obj->setRenderMode("rect");

    Avocado::renderObjects.push_back(*obj);
}

void Avocado::AvocadoObject::engineRender()
{
    if (!GetVisible())
        return;

    if (renderMode == "image")
    {
        if (GetSprite() == nullptr || GetSprite()->GetImage() == nullptr)
            return;
    }

    _renderCalculatedObj(this, renderMode);
}

#pragma endregion

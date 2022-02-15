#pragma once

#include <stdio.h>

#pragma region AVOCADO_RENDERER

namespace AvocadoRenderer
{
    class AvocadoColor
    {
    private:
        Uint8 r = 0, g = 0, b = 0, a = 255;

    public:
        AvocadoColor(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a)
        {
            if (_r > 255 || _g > 255 || _b > 255 | _a > 255)
            {
                printf("Avocado: Color Error");
            }
            if (_r < 0 || _g < 0 || _b < 0 | _a < 0)
            {
                printf("Avocado: Color Error");
            }

            r = _r;
            g = _g;
            b = _b;
            a = _a;
        }
        AvocadoColor(Uint8 _r, Uint8 _g, Uint8 _b)
        {
            if (_r > 255 || _g > 255 || _b > 255)
            {
                printf("Avocado: Color Error");
            }
            if (_r < 0 || _g < 0 || _b < 0)
            {
                printf("Avocado: Color Error");
            }

            r = _r;
            g = _g;
            b = _b;
            a = 255;
        }
        void SetRed(int _r)
        {
            r = _r;
        }
        void SetGreen(int _g)
        {
            g = _g;
        }
        void SetBlue(int _b)
        {
            b = _b;
        }
        void SetAlpha(int _a)
        {
            a = _a;
        }

        Uint8 GetRed()
        {
            return r;
        }
        Uint8 GetGreen()
        {
            return g;
        }
        Uint8 GetBlue()
        {
            return b;
        }
        Uint8 GetAlpha()
        {
            return a;
        }
    };
}

#pragma endregion
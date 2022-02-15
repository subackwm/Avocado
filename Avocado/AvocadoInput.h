#include "AvocadoMath.h"

#define INPUT_TYPE_COUNT 124

#pragma region AVOCADO_INPUT

namespace AvocadoInput
{
    char inputPressed[INPUT_TYPE_COUNT][10] = {{}};
    AvocadoMath::Vector *mousePosition = new AvocadoMath::Vector(0, 0);
    bool mouseLeft = false, mouseCenter = false, mouseRight = false;

    bool GetInput(const char *inputType)
    {
        for (int i = 0; i < INPUT_TYPE_COUNT; i++)
        {
            if (!strcmp(inputPressed[i], inputType))
            {
                return true;
            }
        }

        return false;
    }

    void removeInput(const char *inputType)
    {
        for (int i = 0; i < INPUT_TYPE_COUNT; i++)
        {
            if (!strcmp(inputPressed[i], inputType))
            {
                memmove(inputPressed[i], &"", sizeof(char) * 10);
                break;
            }
        }
    }

    bool isContain(const char *inputType)
    {
        for (int i = 0; i < INPUT_TYPE_COUNT; i++)
        {
            if (!strcmp(inputPressed[i], inputType))
            {
                return true;
            }
        }
        return false;
    }

    void addInput(const char *inputType)
    {
        if (isContain(inputType))
            return;
        for (int i = 0; i < INPUT_TYPE_COUNT; i++)
        {
            if (!strcmp(inputPressed[i], ""))
            {
                memmove(inputPressed[i], inputType, sizeof(char) * 10);
                break;
            }
        }

    }

}

#pragma endregion
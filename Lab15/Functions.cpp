#include "stdafx.h"

namespace Functions
{
    int* add_element(int* array, int curSize, int element)
    {
        int* temp = new int[curSize + 1];

        for (int i = 0; i < curSize; i++)
        {
            temp[i] = array[i];
        }

        temp[curSize] = element;
        delete[] array;
        return temp;
    }

    char* stringToChar(std::string str)
    {
        int str_size = str.size();
        char* chr = new char[str_size];
        for (size_t i = 0; i < str_size; i++)
        {
            chr[i] = str[i];
        }
        chr[str_size] = '\0';
        return chr;
    }
}

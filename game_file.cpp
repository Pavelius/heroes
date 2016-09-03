#include "main.h"

int	gamefile::sides() const
{
    int r = 0;
    for(auto i : types)
    {
        if(i!=Empthy)
            r++;
    }
    return r;
}

void gamefile::clear()
{
    memset(this, 0, sizeof(gamefile));
    difficult = NormalDifficulty;
    level = LevelNormal;
}

void gamefile::players(int value)
{
    for(auto& e : types)
    {
        if(e==Human)
        {
            if(value>0)
            {
                // Nothing
            }
            else
                e = Computer;
            value--;
        }
    }
}

void gamefile::validate()
{
    for(int i=0; i<6; i++)
    {
        if(types[i]==Empthy)
            continue;
        if(races[i]==Random)
            races[i] = tokens(xrand(Barbarian,Wizard));
    }
}
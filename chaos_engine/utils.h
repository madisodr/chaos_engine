#ifndef UTILS_H
#define UTILS_H

template<size_t SIZE, class T> inline size_t ARRAY_SIZE(T (&arr)[SIZE])
{
    return SIZE;
}

/* Utility Methos */
CRGB wheel(byte wheel_pos)
{
    CRGB c;

    if (wheel_pos < 85) {
        c = CRGB(wheel_pos * 3, 255 - wheel_pos * 3, 0);
    } else if (wheel_pos < 170) {
        wheel_pos -= 85;
        c = CRGB(255 - wheel_pos * 3, 0, wheel_pos * 3);
    } else {
        wheel_pos -= 170;
        c = CRGB(0, wheel_pos * 3, 255 - wheel_pos * 3);
    }

    return c;
}

/* wraps x around mod in both directions */
int modulo(const int x, const int mod)
{
    if (x < 0)
        return mod - abs(x);
    else if (x >= mod)
        return x % mod;
    else
        return x;
}

// return a random double between 0.0 and 1.0    
float random_float() 
{   
    return (float) (random(1,101) / (float) 100.0); 
}


#endif

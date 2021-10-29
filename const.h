#include "renderer/objects/Vector2.h"

#ifndef DEBUG
#ifndef BIGCHUNGUSMODE
const vectors::Vector2d RESOLUTION(120.0f, 50.0f);
#else
const vectors::Vector2d RESOLUTION(200.0f, 200.0f);
#endif
#else
const vectors::Vector2d RESOLUTION(60.0f, 60.0f);
#endif
const int FRAMERATE = 20;
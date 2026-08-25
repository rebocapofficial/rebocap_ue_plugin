#pragma once

#include "Runtime/Launch/Resources/Version.h"

#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1)
    #include "Styling/AppStyle.h"
    #define REBOCAP_STYLE FAppStyle
#else
    #include "EditorStyleSet.h"
    #define REBOCAP_STYLE FEditorStyle
#endif

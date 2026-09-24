//------------------------------------------------------------------------------
// file:	CP_Log.c
// author:	Aidan Clack
// brief:	Text functions for loading fonts and drawing text
//
// Copyright © 2026 DigiPen, All rights reserved.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include "cprocessing.h"
#include "Internal_Log.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#endif

//------------------------------------------------------------------------------
// Defines and Internal Variables:
//------------------------------------------------------------------------------

static CP_SEVERITY severity_level = CP_INFO;

//------------------------------------------------------------------------------
// Internal Functions:
//------------------------------------------------------------------------------

const char* GetLevelString(CP_SEVERITY level) {
    switch (level) 
    {
    case CP_DEBUG:   return "[DEBUG] ";
    case CP_INFO:    return "[INFO]  ";
    case CP_WARNING: return "[WARN]  ";
    case CP_ERROR:   return "[ERROR] ";
    default:         return "[LOG]   ";
    }
}

//------------------------------------------------------------------------------
// Library Functions:
//------------------------------------------------------------------------------

CP_API void	CP_Log(CP_SEVERITY severity, const char* format, ...)
{
    if (severity > severity_level)
        return;

    // We may want to explicitly tell the user the buffer size...
    char buffer[4096];
    const char* prefix = GetLevelString(severity);
    size_t prefixLen = strlen(prefix);

    // Ensure the prefix alone doesn't blow out the buffer
    if (prefixLen >= sizeof(buffer))
        return;

    va_list args;
    va_start(args, format);

    // Format the string
    size_t safeMsgSpace = sizeof(buffer) - prefixLen;
    vsnprintf(buffer + prefixLen, safeMsgSpace, format, args);

    va_end(args);

    // Copy in the prefix.
    memcpy(buffer, prefix, prefixLen);

    size_t bufferLen = strlen(buffer);
    buffer[bufferLen] = '\n';
    buffer[bufferLen + 1] = '\0';

#if defined(_WIN32) || defined(_WIN64)
    OutputDebugStringA(buffer);
#else
    fprintf(stderr, "%s", buffer);
    fflush(stderr);
#endif
}

CP_API void CP_Log_Info(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    CP_Log(CP_INFO, format, args);

    va_end(args);
}

CP_API void CP_Log_Warning(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    CP_Log(CP_WARNING, format, args);

    va_end(args);
}

CP_API void CP_Log_Error(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    CP_Log(CP_ERROR, format, args);

    va_end(args);
}

CP_API void CP_Log_Debug(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    CP_Log(CP_DEBUG, format, args);

    va_end(args);
}

CP_API void CP_Log_SetSeverity(CP_SEVERITY severity)
{
    if (severity < CP_INFO || severity > CP_SEVERITYCOUNT)
        return;

    severity_level = severity;
}
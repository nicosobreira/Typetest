#include "Core/ui/window.h"

#include <stdlib.h>
#include <string.h>

#include "Core/utils/error.h"
#include "Core/utils/math.h"

int Window_GetCols(WINDOW *win)
{
    return getmaxx(win) - 2;
}

int Window_GetLines(WINDOW *win)
{
    return getmaxy(win) - 2;
}

WINDOW *Window_New(WINDOW *base, WindowLayout layout, WindowAlign align)
{
    WindowGeometry geom = Window_SetLayout(base, layout, align);
    Window_CheckAttrs(geom);

    WINDOW *window = newwin(geom.lines, geom.cols, geom.y, geom.x);

    return window;
}

void Window_DrawString(WINDOW *base, String *pString)
{
    const int startX = 0;
    const int startY = 0;
    const int maxCols = getmaxx(base) - startX * 2;
    const int maxLines = getmaxy(base) - startY * 2;

    int totalLength = String_Length(pString);
    int currentLine = startY;

    for (int i = 0; i < totalLength; i += maxCols)
    {
        const int sliceLength = minInt(totalLength - i, maxCols);

        if (currentLine > maxLines)
            break;

        mvwaddnwstr(base, currentLine, startX, String_GetPointerAt(pString, i), sliceLength);

        currentLine++;
    }
}

void Window_ClearRectangle(WINDOW *base, int startY, int startX, int endY, int endX)
{
    if (startY > endY || startX > endX)
        ERROR(1, "%s", "Wrong argument order to function");

    int width = endX - startX + 1;
    char *blanckString = malloc(width + 1);
    memset(blanckString, ' ', width);
    blanckString[width] = '\0';

    for (int y = startY; y <= endY; y++)
    {
        mvwaddstr(base, y, startX, blanckString);
    }

    free(blanckString);
}

WindowGeometry Window_SetLayout(WINDOW *base, WindowLayout layout, WindowAlign align)
{
    switch (layout)
    {
    case WINDOW_LAYOUT_ON_TOP:
        return Window_SetLayout_OnTop(base, align);
    case WINDOW_LAYOUT_CENTER:
        return Window_SetLayout_Center(base);
    default:
        ERROR(1, "Invalid layout \"%zu\"", layout);
        break;
    }

    return (WindowGeometry){0};
}

WindowGeometry Window_SetLayout_Center(WINDOW *base)
{
    const float maxCols = (float)getmaxx(base);
    const float maxLines = (float)getmaxy(base);

    // FIX: This values are hard coded
    // Those values must be passed as arguments, in a struct called Attributes
    // or Positions
    const float colsFactor = 0.8f;
    const float linesFactor = 0.3f;

    float cols = maxCols * colsFactor;
    float lines = maxLines * linesFactor;

    float x = (maxCols - cols) / 2.0f;
    float y = (maxLines - lines) / 2.0f;

    WindowGeometry geom = {
        .x = (int)x,
        .y = (int)y,
        .cols = (int)cols,
        .lines = (int)lines,
    };

    return geom;
}

WindowGeometry Window_SetLayout_OnTop(WINDOW *onTop, WindowAlign align)
{
    if (onTop == stdscr)
        ERROR(1, "%s", "No window can be on top of the stdscr");

    const float maxColsStdscr = (float)getmaxx(stdscr);
    const float maxLinesStdscr = (float)getmaxy(stdscr);

    const float beginCols = (float)getbegx(onTop);
    const float beginLines = (float)getbegy(onTop);

    const float maxCols = (float)getmaxx(onTop);

    // FIX: This values are hard coded
    // Those values must be passed as arguments, in a struct called Attributes
    // or Positions
    const float colsFactor = 0.8f;
    const float linesFactor = 0.2f;

    float cols = maxColsStdscr * colsFactor;
    float lines = maxLinesStdscr * linesFactor;

    // X Aligment
    float x = Window_SetAlign(align, (maxCols - beginCols), beginCols, cols);

    const float offset = .0f;
    float y = beginLines - lines - offset;

    WindowGeometry geom = {
        .x = (int)x,
        .y = (int)y,
        .cols = (int)cols,
        .lines = (int)lines,
    };

    return geom;
}

float Window_SetAlign(WindowAlign align, float ref_size, float ref_pos, float obj_size)
{
    switch (align)
    {
    case WINDOW_ALIGN_CENTER:
        return Window_SetAlign_Center(ref_size, ref_pos, obj_size);
    case WINDOW_ALIGN_LEFT:
        return Window_SetAlign_Left(ref_size, ref_pos, obj_size);
    case WINDOW_ALIGN_RIGHT:
        return Window_SetAlign_Right(ref_size, ref_pos, obj_size);
    default:
        ERROR(1, "%s", "Invalid window aligment");
    }

    return 0;
}

float Window_SetAlign_Center(float ref_size, float ref_pos, float obj_size)
{
    return ref_pos + (obj_size - ref_size) / 2.0f;
}

float Window_SetAlign_Left(float ref_size, float ref_pos, float obj_size)
{
    (void)ref_size;
    (void)obj_size;
    return ref_pos;
}

// FIX: don't work
float Window_SetAlign_Right(float ref_size, float ref_pos, float obj_size)
{
    (void)ref_size;
    (void)ref_pos;
    return obj_size;
}

void Window_CheckAttrs(WindowGeometry geom)
{
    if (geom.x < 0 || geom.x > COLS)
        ERROR(1, "The x value is beyond the stdscr: %d", geom.x);

    if (geom.cols < 0 || geom.cols > COLS)
        ERROR(1, "The cols value is beyond the stdscr: %d", geom.cols);

    if (geom.y < 0 || geom.y > LINES)
        ERROR(1, "The y value is beyond the stdscr: %d", geom.y);

    if (geom.lines < 0 || geom.lines > LINES)
        ERROR(1, "The lines value is beyond the stdscr: %d", geom.lines);
}

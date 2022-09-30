#include "common.h"
#include "renderapp.h"

using namespace tbgl;

int main()
{
    RenderApp renderApp;

    renderApp.Init();

    while (!renderApp.ShouldClose())
    {
        renderApp.Run();
    }

    return 0;
}

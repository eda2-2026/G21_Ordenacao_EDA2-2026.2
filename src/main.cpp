#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "catalogo.h"
#include "menu.h"

int main() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    Catalogo catalogo;
    Menu menu(catalogo);

    menu.executar();

    return 0;
}

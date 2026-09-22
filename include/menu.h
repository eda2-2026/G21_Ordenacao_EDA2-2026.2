#ifndef MENU_H
#define MENU_H

#include "catalogo.h"

class Menu {
    
private:
    Catalogo &catalogo;
    
public:   
    Menu(Catalogo &catalogo); 
    
    void executar();
};

#endif 
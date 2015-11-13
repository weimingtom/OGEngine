#ifndef __BUTTON_H_
#define __BUTTON_H_

#include "Library.h"

/* Classe reférente a logica dos Buttons reutilizavel */

class ClassButton
{
 private:
         BITMAP *ButtonNull;
         BITMAP *ButtonSelect;
         int x, y;
         bool clicked;
 public:
        ClassButton();
        ~ClassButton();
        void InitButton(char *, char *, int x, int y);
        bool MouseButtonSelect(int mouse_x, int mouse_y);
        void DrawButton(BITMAP *GameBuffer);
};

#endif

#ifndef __GUI_H_
#define __GUI_H_

#include "Library.h"

/* Logica reférente a GUI */

class ClassGUI
{
 private:
         BITMAP *ImageGUI;
         bool ActiveGUI;
         int gui_x;
         int gui_y;
         
         int aux_x, aux_y;
 public:
        ClassGUI();
        ~ClassGUI();
        void InitGUI(char *FileGUI, int x, int y);
        void LogicGUI(int, int, int);
        void MoveGUI(int, int);
        void CloseGUI();
        void InfoText(BITMAP *, char *, int, int, int);
        void DrawGUI(BITMAP *);
        
        bool SetGUI();
};

#endif

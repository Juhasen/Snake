#include "winsys.h"
#include "snake.h"


int main ()
{
  CDesktop d;
  d.insert(new CInputLine (CRect (CPoint (0, 10), CPoint (15, 15))));
  d.insert(new CWindow (CRect (CPoint (0, 0), CPoint (20, 10)), '#'));
  d.insert(new CSnake(CRect (CPoint (0, 1), CPoint (45, 25))));
  d.run ();
  return 0;
}

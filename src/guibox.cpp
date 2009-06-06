#include "guibox.h"
#include "caresources.h"


GUIBox::GUIBox(const int n_left, const int n_top, const int n_right, const int n_bottom)
:   ew ( CA_RES->gui_edge2->get_width()),
    eh ( CA_RES->gui_edge2->get_height()),
    bw ( CA_RES->gui_border2->get_width()),
    bh ( CA_RES->gui_border1->get_height())
{
    setPosition(n_left, n_top, n_right, n_bottom);
}


void GUIBox::setPosition(const int n_left, const int n_top, const int n_right, const int n_bottom)
{
    left = n_left;
    top = n_top;
    right = n_right;
    bottom = n_bottom;
}


GUIBox::~GUIBox()
{
}


void GUIBox::display(bool active)
{
    CL_Display::fill_rect( CL_Rect(left+bw, top+bh, right-bw, bottom-bh), CL_Color(0, 0, 0, 64) );

    // Edges:
    //
    CA_RES->gui_edge1->draw( left, top );
    CA_RES->gui_edge2->draw( right-ew, top );
    CA_RES->gui_edge3->draw( left, bottom-eh );
    CA_RES->gui_edge4->draw( right-ew, bottom-eh );

    // Borders:
    //
    CA_RES->gui_border1->draw( CL_Rect(left+ew, top, right-ew, top+bh) );
    CA_RES->gui_border2->draw( CL_Rect(right-bw, top+eh, right, bottom-eh) );
    CA_RES->gui_border3->draw( CL_Rect(left+ew, bottom-bh, right-ew, bottom) );
    CA_RES->gui_border4->draw( CL_Rect(left, top+eh, left+bw, bottom-eh) );
}
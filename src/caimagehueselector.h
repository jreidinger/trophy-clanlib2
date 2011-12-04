#ifndef CAIMAGEHUESELECTOR_H
#define CAIMAGEHUESELECTOR_H

#include <ClanLib/core.h>
#include "caimageview.h"



/** An Image with text above and below it.
    This widget is an interactive hue selector for an image.
    @author Andrew Mustun
*/
class CAImageHueSelector : public CAImageView  {
public:
    CAImageHueSelector( Direction vertical=Horizontal );
    CAImageHueSelector( const std::string upperText,
                        const std::string lowerText,
                        CL_Image image,
                        bool autoResize=true,
                        Direction vertical=Horizontal );
    ~CAImageHueSelector();

    void setImageSize( int w, int h );
    void changeImageHue( bool forward );
    //! Gets the currently selected hue of the image (0-360).
    int  getHue() { return hue; }

    virtual void display( bool active=true );
    virtual void handleKey (const CL_InputEvent &key);

private:
    //! Pointer to the surface with modified hue.

    CL_Image hueImage;
    //! Currently selected hue (0-360).
    int hue;
};

#endif

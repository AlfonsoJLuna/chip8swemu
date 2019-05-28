#ifndef VIDEO_H
#define VIDEO_H


#include <stdbool.h>


/**
 * Initialize SDL and create a window with an OpenGL context.
 *
 * @return 0 if the window was created succesfully, 1 otherwise.
*/
bool videoInitialize();


/**
 * Enable Vertical Sync
 *
 * @param enable
 *
 */
void videoToggleVsync(bool enable);


/**
 * Draw the CHIP-8 screen and the GUI to the window.
*/
void videoRender();


/**
 * Destroy SDL context.
*/
void videoFinalize();


#endif  // VIDEO_H

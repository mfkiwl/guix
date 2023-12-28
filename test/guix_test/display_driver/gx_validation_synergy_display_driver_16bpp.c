/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Application Interface (API)                                         */
/**                                                                       */
/**************************************************************************/
#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "stdio.h"
#include "gx_dave2d_simulation_display_driver.h"


UINT gx_validation_dave2d_display_driver_setup_565rgb(GX_DISPLAY *display);
UINT gx_validation_dave2d_display_driver_setup_565rgb_rotated(GX_DISPLAY *display);

static VOID _gx_validation_display_buffer_565rgb_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    GX_PARAMETER_NOT_USED(canvas);
    GX_PARAMETER_NOT_USED(dirty);
}



UINT gx_validation_dave2d_display_driver_setup_565rgb(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */


    _gx_dave2d_simulation_display_driver_565rgb_setup(display, (VOID *)1, _gx_validation_display_buffer_565rgb_toggle);


    return(GX_SUCCESS);
}

UINT gx_validation_dave2d_display_driver_setup_565rgb_rotated(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */


    _gx_dave2d_simulation_display_driver_565rgb_rotated_setup(display, (VOID *)1, _gx_validation_display_buffer_565rgb_toggle);


    return(GX_SUCCESS);
}

/*VOID gx_validation_graphics_driver_cleanup_565rgb(GX_DISPLAY *display)
   {

   }
 */

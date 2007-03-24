/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */

/*
 *  (GLABELS) Label and Business Card Creation program for GNOME
 *
 *  cairo_label_path.c:  Cairo label path module
 *
 *  Copyright (C) 2001-2007  Jim Evins <evins@snaught.com>.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <config.h>

#include "cairo-label-path.h"

#include <math.h>
#include <glib.h>

#include "debug.h"

/*===========================================*/
/* Private types                             */
/*===========================================*/

/*===========================================*/
/* Private globals                           */
/*===========================================*/

/*===========================================*/
/* Local function prototypes                 */
/*===========================================*/

static void gl_cairo_rect_label_path             (cairo_t                *cr,
                                                  glLabel                *label,
                                                  gboolean                waste_flag);
static void gl_cairo_round_label_path            (cairo_t                *cr,
                                                  glLabel                *label,
                                                  gboolean                waste_flag);
static void gl_cairo_cd_label_path               (cairo_t                *cr,
                                                  glLabel                *label,
                                                  gboolean                waste_flag);


/*--------------------------------------------------------------------------*/
/* Create label path                                                        */
/*--------------------------------------------------------------------------*/
void
gl_cairo_label_path (cairo_t           *cr,
                     glLabel           *label,
                     gboolean           waste_flag)
{
        const glTemplateLabelType *label_type;

        gl_debug (DEBUG_PATH, "START");

        label_type = gl_template_get_first_label_type (label->template);

        switch (label_type->shape) {

        case GL_TEMPLATE_SHAPE_RECT:
                gl_cairo_rect_label_path (cr, label, waste_flag);
                break;

        case GL_TEMPLATE_SHAPE_ROUND:
                gl_cairo_round_label_path (cr, label, waste_flag);
                break;

        case GL_TEMPLATE_SHAPE_CD:
                gl_cairo_cd_label_path (cr, label, waste_flag);
                break;

        default:
                g_message ("Unknown label style");
                break;
        }

        gl_debug (DEBUG_PATH, "END");
}

/*--------------------------------------------------------------------------*/
/* Create rectangular label path                                            */
/*--------------------------------------------------------------------------*/
static void
gl_cairo_rect_label_path (cairo_t           *cr,
                          glLabel           *label,
                          gboolean           waste_flag)
{
        const glTemplateLabelType *label_type;
        gdouble                    w, h, r;
        gdouble                    x_waste, y_waste;

        gl_debug (DEBUG_PATH, "START");

        label_type = gl_template_get_first_label_type (label->template);
        gl_label_get_size (label, &w, &h);
        r = label_type->size.rect.r;

        if (waste_flag)
        {
                x_waste = label_type->size.rect.x_waste;
                y_waste = label_type->size.rect.x_waste;
        }
        else
        {
                x_waste = 0.0;
                y_waste = 0.0;
        }

        if ( r == 0.0 )
        {
                cairo_rectangle (cr, -x_waste, -y_waste, w+x_waste, h+y_waste);
        }
        else
        {
                cairo_new_path (cr);
                cairo_arc_negative (cr, r-x_waste,   r-y_waste,   r, 3*G_PI/2, G_PI);
                cairo_arc_negative (cr, r-x_waste,   h-r+y_waste, r, G_PI,     G_PI/2);
                cairo_arc_negative (cr, w-r+x_waste, h-r+y_waste, r, G_PI/2,   0.);
                cairo_arc_negative (cr, w-r+x_waste, r-y_waste,   r, 2*G_PI,   3*G_PI/2);
                cairo_close_path (cr);
        }

        gl_debug (DEBUG_PATH, "END");
}

/*--------------------------------------------------------------------------*/
/* Create round label path                                                  */
/*--------------------------------------------------------------------------*/
static void
gl_cairo_round_label_path (cairo_t           *cr,
                           glLabel           *label,
                           gboolean           waste_flag)
{
        const glTemplateLabelType *label_type;
        gdouble                    w, h;
        gdouble                    waste;

        gl_debug (DEBUG_PATH, "START");

        label_type = gl_template_get_first_label_type (label->template);
        gl_label_get_size (label, &w, &h);
        
        if (waste_flag)
        {
                waste = label_type->size.round.waste;
        }
        else
        {
                waste = 0.0;
        }

        cairo_arc (cr, w/2, h/2, w/2+waste, 0.0, 2*G_PI);
	cairo_close_path (cr);

        gl_debug (DEBUG_PATH, "END");
}

/*--------------------------------------------------------------------------*/
/* Create cd label path                                                     */
/*--------------------------------------------------------------------------*/
static void
gl_cairo_cd_label_path (cairo_t           *cr,
                        glLabel           *label,
                        gboolean           waste_flag)
{
        const glTemplateLabelType *label_type;
        gdouble                    w, h;
        gdouble                    xc, yc;
        gdouble                    r1, r2;
	gdouble                    theta1, theta2;
        gdouble                    waste;

        gl_debug (DEBUG_PATH, "START");

        label_type = gl_template_get_first_label_type (label->template);
        gl_label_get_size (label, &w, &h);

        xc = w/2.0;
        yc = h/2.0;

        r1 = label_type->size.cd.r1;
        r2 = label_type->size.cd.r2;

        if (waste_flag)
        {
                waste = label_type->size.cd.waste;
        }
        else
        {
                waste = 0.0;
        }

	/*
	 * Outer path (may be clipped in the case of a business card type CD)
	 */
	theta1 = acos (w / (2.0*r1));
	theta2 = asin (h / (2.0*r1));

	cairo_new_path (cr);
	cairo_arc (cr, xc, yc, r1+waste, theta1, theta2);
	cairo_arc (cr, xc, yc, r1+waste, G_PI-theta2, G_PI-theta1);
	cairo_arc (cr, xc, yc, r1+waste, G_PI+theta1, G_PI+theta2);
	cairo_arc (cr, xc, yc, r1+waste, 2*G_PI-theta2, 2*G_PI-theta1);
	cairo_close_path (cr);


        /*
         * Inner path (hole)
         */
	cairo_new_sub_path (cr);
        cairo_arc (cr, xc, yc, r2-waste, 0.0, 2*G_PI);
	cairo_close_path (cr);

        gl_debug (DEBUG_PATH, "END");
}


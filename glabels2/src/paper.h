/*
 *  (GLABELS) Label and Business Card Creation program for GNOME
 *
 *  paper.h:  paper module header file
 *
 *  Copyright (C) 2003  Jim Evins <evins@snaught.com>.
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

#ifndef __PAPER_H__
#define __PAPER_H__

#include <glib.h>

G_BEGIN_DECLS

/*
 *   Paper
 */
typedef struct {
	gchar               *id;
	gchar               *name;
	gdouble              width;
	gdouble              height;
} glPaper;


void                 gl_paper_init                (void);

GList               *gl_paper_get_id_list         (void);
void                 gl_paper_free_id_list        (GList            **ids);

GList               *gl_paper_get_name_list       (void);
void                 gl_paper_free_name_list      (GList            **names);

glPaper             *gl_paper_from_id             (const gchar       *id);
glPaper             *gl_paper_from_name           (const gchar       *name);

gchar               *gl_paper_lookup_id_from_name (const gchar       *name);
gchar               *gl_paper_lookup_name_from_id (const gchar       *id);

glPaper             *gl_paper_dup                 (const glPaper     *orig);
void                 gl_paper_free                (glPaper          **paper);

G_END_DECLS

#endif
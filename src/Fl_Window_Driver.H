//
// A base class for platform specific window handling code
// for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010-2018 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

/**
 \cond DriverDev
 \addtogroup DriverDeveloper
 \{
 */

/** \file Fl_Window_Driver.H
 \brief declaration of class Fl_Window_Driver.
*/

#ifndef FL_WINDOW_DRIVER_H
#define FL_WINDOW_DRIVER_H

#include <FL/Fl_Export.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Overlay_Window.H>

#include <stdlib.h>

class Fl_X;
class Fl_Image;
class Fl_RGB_Image;

// not directly window driver related, but ...
int fl_create_print_window(); // used internally on some platforms

/**
 \brief A base class for platform specific window handling code.

 This class is only for internal use by the FLTK library.

 When porting FLTK to a new platform, many mothods in this class provide
 a minimal default implementation. Some methods must be overridden to make
 sure that the Graphics Driver will draw into the bitmap associated with
 this window.
*/
class FL_EXPORT Fl_Window_Driver
{
  friend class Fl_Window;
private:
  static bool is_a_rescale_; // true when a top-level window is being rescaled

protected:
  Fl_Window *pWindow;
  struct shape_data_type;
  shape_data_type *shape_data_; ///< non-null means the window has a non-rectangular shape
  void flush_Fl_Window(); // accessor to protected Fl_Window::flush()
public:
  Fl_Window_Driver(Fl_Window *);
  virtual ~Fl_Window_Driver();
  static Fl_Window_Driver *newWindowDriver(Fl_Window *);
  int wait_for_expose_value;
  Fl_Offscreen other_xid; // offscreen bitmap (overlay and double-buffered windows)
  virtual int screen_num();
  virtual void screen_num(int) {}


  // --- frequently used accessors to public window data
  /** returns the x coordinate of the window. */
  int x() const { return pWindow->x(); }
  /** returns the y coordinate of the window. */
  int y() const { return pWindow->y(); }
  /** returns the width of the window. */
  int w() const { return pWindow->w(); }
  /** returns the height of the window. */
  int h() const { return pWindow->h(); }
  /** returns whether the window has a border. */
  int border() const { return pWindow->border(); }
  /** returns whether the window itself is visible(). */
  int visible() const { return pWindow->visible(); }
  /** returns whether the window and all its parents are visible(). */
  int visible_r() const { return pWindow->visible_r(); }
  /** returns whether the window is shown(). */
  int shown() const { return pWindow->shown(); }
  /** returns the parent of the window. */
  Fl_Group *parent() const { return pWindow->parent(); }

  // --- accessors to private window data
  int minw();
  int minh();
  int maxw();
  int maxh();
  int dw();
  int dh();
  int aspect();
  int fullscreen_screen_top();
  int fullscreen_screen_bottom();
  int fullscreen_screen_left();
  int fullscreen_screen_right();
  unsigned char size_range_set();
  int force_position();
  void force_position(int c);
  void x(int X);
  void y(int Y);
  void current(Fl_Window *c);
  char show_iconic() { return Fl_Window::show_iconic_; }
  void show_iconic(char c) { Fl_Window::show_iconic_ = c; }
  void i(Fl_X *x) { pWindow->i = x; }
  /** for an Fl_Overlay_Window, returns the value of its overlay_ member variable */
  Fl_Window *overlay() {
    return pWindow->as_overlay_window() ? pWindow->as_overlay_window()->overlay_ : NULL;
  }
  /** for an Fl_Overlay_Window, sets the value of its overlay_ member variable */
  void overlay(Fl_Window *o) {
    if (pWindow->as_overlay_window()) pWindow->as_overlay_window()->overlay_ = o;
  }

  void resize_after_scale_change(int ns, float old_f, float new_f);

  // --- window data
  virtual int decorated_w() { return w(); } // default, should be overidden by driver
  virtual int decorated_h() { return h(); }
  virtual const Fl_Image* shape() { return NULL; }

  // --- window management
  virtual void take_focus();
  virtual void flush(); // the default implementation may be enough
  virtual void flush_double();
  virtual void flush_overlay();
  /** Usable for platform-specific code executed before the platform-independent part of Fl_Window::draw() */
  virtual void draw_begin();
  /** Usable for platform-specific code executed after the platform-independent part of Fl_Window::draw() */
  virtual void draw_end();
  void draw();
  virtual void make_current();
  virtual void label(const char *name, const char *mininame);

  virtual Fl_X *makeWindow() { return 0; }
  virtual void wait_for_expose();
  virtual void destroy_double_buffer();
  virtual void show();
  virtual void resize(int /*X*/, int /*Y*/, int /*W*/, int /*H*/) {}
  virtual void hide() {}
  int hide_common();
  virtual void map() {}
  virtual void unmap() {}
  virtual void fullscreen_on() {}
  virtual void fullscreen_off(int /*X*/, int /*Y*/, int /*W*/, int /*H*/) {}
  virtual void use_border();
  virtual void size_range();
  virtual void iconize() {}
  virtual void decoration_sizes(int *top, int *left,  int *right, int *bottom) {
    *top = *left = *right = *bottom = 0;
  }
  virtual void show_with_args_begin() {}
  virtual void show_with_args_end(int /*argc*/, char ** /*argv*/) {}
  virtual int can_do_overlay();
  virtual void redraw_overlay();

  // --- window cursor stuff
  virtual int set_cursor(Fl_Cursor);
  virtual int set_cursor(const Fl_RGB_Image*, int, int);

  // --- window shape stuff
  void shape_pixmap_(Fl_Image* pixmap); // platform-independent, support function
  virtual void shape(const Fl_Image*) {}
  virtual void shape_alpha_(Fl_Image*, int /*offset*/) {}

  // --- window icon stuff
  virtual void icons(const Fl_RGB_Image* /*icons*/[], int /*count*/) {}
  virtual const void *icon() const {return NULL;}
  virtual void icon(const void *) {}
  virtual void free_icons() {}

  // --- window printing/drawing helper
  virtual void capture_titlebar_and_borders(Fl_RGB_Image*& top, Fl_RGB_Image*& left,
                                            Fl_RGB_Image*& bottom, Fl_RGB_Image*& right);
  virtual int scroll(int /*src_x*/, int /*src_y*/, int /*src_w*/, int /*src_h*/,
                     int /*dest_x*/, int /*dest_y*/,
                     void (*)(void*, int,int,int,int), void*) { return 0; }
  static inline Fl_Window_Driver* driver(const Fl_Window *win) {return win->pWindowDriver;}
};

#endif // FL_WINDOW_DRIVER_H

/**
 \}
 \endcond
 */

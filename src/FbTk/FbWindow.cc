// FbWindow.cc for FbTk - fluxbox toolkit
// Copyright (c) 2002 Henrik Kinnunen (fluxgen at users.sourceforge.net)
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

// $Id: FbWindow.cc,v 1.5 2003/02/02 16:32:40 rathnor Exp $

#include "FbWindow.hh"

#include "Color.hh"
#include "App.hh"

#include <cassert>

namespace FbTk {

Display *FbWindow::s_display = 0;

FbWindow::FbWindow():m_parent(0), m_screen_num(0), m_window(0), m_x(0), m_y(0), 
                     m_width(0), m_height(0), m_border_width(0), m_layeritem(0) {

    if (s_display == 0)
        s_display = App::instance()->display();
}

FbWindow::FbWindow(int screen_num,
                   int x, int y, size_t width, size_t height, long eventmask, 
                   bool override_redirect,
                   int depth,
                   int class_type):
    m_screen_num(screen_num),
    m_parent(0) {
	
    create(RootWindow(FbTk::App::instance()->display(), screen_num), 
           x, y, width, height, eventmask,
           override_redirect, depth, class_type);
};

FbWindow::FbWindow(const FbWindow &parent,
                   int x, int y, size_t width, size_t height, long eventmask,
                   bool override_redirect, 
                   int depth, int class_type):
   m_parent(&parent),
   m_screen_num(parent.screenNumber()) { 

    create(parent.window(), x, y, width, height, eventmask, 
           override_redirect, depth, class_type);
	
	
};

FbWindow::~FbWindow() {
    if (m_window != 0)
        XDestroyWindow(s_display, m_window);
}


void FbWindow::setBackgroundColor(const FbTk::Color &bg_color) {
    XSetWindowBackground(s_display, m_window, bg_color.pixel());
}

void FbWindow::setBackgroundPixmap(Pixmap bg_pixmap) {
    XSetWindowBackgroundPixmap(s_display, m_window, bg_pixmap);
}

void FbWindow::setBorderColor(const FbTk::Color &border_color) {
    XSetWindowBorder(s_display, m_window, border_color.pixel());
}
void FbWindow::setBorderWidth(size_t size) {	
    XSetWindowBorderWidth(s_display, m_window, size);
    m_border_width = size;
}

void FbWindow::setName(const char *name) {
    XStoreName(s_display, m_window, name);
}

void FbWindow::setEventMask(long mask) {
    XSelectInput(s_display, m_window, mask);
}

void FbWindow::clear() {
    XClearWindow(s_display, m_window);
}

FbWindow &FbWindow::operator = (Window win) {
    if (m_window != 0)
        XDestroyWindow(s_display, m_window);
    m_window = win;
    if (m_window != 0)
        updateGeometry();
	
    return *this;
}

void FbWindow::show() {
    XMapWindow(s_display, m_window);
}

void FbWindow::showSubwindows() {
    XMapSubwindows(s_display, m_window);
}

void FbWindow::hide() {
    XUnmapWindow(s_display, m_window);
}

void FbWindow::move(int x, int y) {
    XMoveWindow(s_display, m_window, x, y);
    m_x = x;
    m_y = y;
}

void FbWindow::resize(size_t width, size_t height) {
    XResizeWindow(s_display, m_window, width, height);
    m_width = width;
    m_height = height;
}

void FbWindow::moveResize(int x, int y, size_t width, size_t height) {
    XMoveResizeWindow(s_display, m_window, x, y, width, height);
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
}

void FbWindow::lower() {
    XLowerWindow(s_display, m_window);
}

void FbWindow::raise() {
    XRaiseWindow(s_display, m_window);
}

int FbWindow::screenNumber() const {
    return m_screen_num;
}
void FbWindow::updateGeometry() {
    if (m_window == 0)
        return;

    Window root;
    size_t border_width, depth;
    XGetGeometry(s_display, m_window, &root, &m_x, &m_y,
                 &m_width, &m_height, &border_width, &depth);
}

void FbWindow::create(Window parent, int x, int y,
                      size_t width, size_t height, 
                      long eventmask, bool override_redirect,
                      int depth, int class_type) {
                     

    if (s_display == 0)
        s_display = FbTk::App::instance()->display();

    assert(s_display);

    m_border_width = 0;

    long valmask = CWEventMask;
    XSetWindowAttributes values;
    values.event_mask = eventmask;

    if (override_redirect) {        
        valmask |= CWOverrideRedirect;
        values.override_redirect = True;
    }

    m_window = XCreateWindow(s_display, parent, x, y, width, height,
                             0, // border width  
                             depth, // depth
                             class_type, // class
                             CopyFromParent, // visual
                             valmask, // create mask
                             &values); // create atrribs
    
    assert(m_window);

    updateGeometry();
}

bool operator == (Window win, const FbWindow &fbwin) {
    return win == fbwin.window();
}

};

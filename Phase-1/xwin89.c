/*

    original author:  Gordon Griesel

            date:  Jan 23, 2025

         purpose:  C program for students to practice their C programming

                   over the Winter break. Also, introduce students to the

                   X Window System. We will use the X Window protocol or

                   API to generate output in some of our lab and homework

                   assignments.

    Instructions:

    1. If you make changes to this file, put your name at the top of

       the file. Use one C style multi-line comment to hold your full

       name. Do not remove the original author's name from this or

       other source files please.

    2. Build and run this program by using the provided Makefile.

       At the command-line enter make.

       Run the program by entering ./a.out

       Quit the program by pressing Esc.

       The compile line will look like this:

       gcc xwin89.c -Wall -Wextra -Werror -pedantic -ansi -lX11

       To run this program on the Odin server, you will have to log in

       using the -YC option. Example: ssh myname@odin.cs.csub.edu -YC

    3. See the assignment page associated with this program for more

       instructions.

  */

/* Jacqueline */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <X11/Xlib.h>

#include <X11/keysym.h>

#include <unistd.h>

#include <signal.h>

#include <sys/types.h>

#define MAX_CHILDREN 20

struct Global {

    Display *dpy;

    Window win;

    GC gc;

    int xres, yres;

    int color;

    pid_t pidlist[MAX_CHILDREN];

    int n;

} g;

void x11_cleanup_xwindows(void);

void x11_init_xwindows(void);

void x11_clear_window(void);

void check_mouse(XEvent *e);

int  check_keys(XEvent *e);

void render(void);

void sigchild_handler(int sig); //

void make_child_window(void);

void make_xeyes_window(void);

char **myargv;

char **myenvp;

int child;

int child_closed = 0;

int main(int argc, char *argv[], char *envp[])

{

    myargv = argv;

    myenvp = envp;

    if (argc > 1) {

        if (strcmp(argv[1], "--child--") == 0)

            child = 1;

    }

    if (!child) {

    signal(SIGCHLD, sigchild_handler);

}

    XEvent e;

    int done = 0;

    x11_init_xwindows();

    while (!done) {

        /* Check the event queue */

        while (XPending(g.dpy)) {

            XNextEvent(g.dpy, &e);

            check_mouse(&e);

            done = check_keys(&e);

            render();

        }

        usleep(4000);

    }

    x11_cleanup_xwindows();

    return 0;

}

void x11_cleanup_xwindows(void)

{

    XDestroyWindow(g.dpy, g.win);

    XCloseDisplay(g.dpy);

}

void x11_init_xwindows(void)

{

    int scr;

    if (!(g.dpy = XOpenDisplay(NULL))) {

        fprintf(stderr, "ERROR: could not open display!\n");

        exit(EXIT_FAILURE);

    }

    scr = DefaultScreen(g.dpy);

    g.xres = 400;

    g.yres = 200;

    g.win = XCreateSimpleWindow(g.dpy, RootWindow(g.dpy, scr),

                                1, 1, g.xres, g.yres, 0,

                                0x00ffffff, 0x00000000);

    XStoreName(g.dpy, g.win, "cs3600 xwin sample");

    g.gc = XCreateGC(g.dpy, g.win, 0, NULL);

    XMapWindow(g.dpy, g.win);

    XSelectInput(g.dpy, g.win,

                 ExposureMask | StructureNotifyMask |

                 PointerMotionMask | ButtonPressMask |

                 ButtonReleaseMask | KeyPressMask);

}

void x11_clear_window(void)

{

    XClearWindow(g.dpy, g.win);

}

void check_mouse(XEvent *e)

{

    static int count = 0;

    static int savex = 0;

    static int savey = 0;

    int mx = e->xbutton.x;

    int my = e->xbutton.y;

    if (e->type != ButtonPress

     && e->type != ButtonRelease

     && e->type != MotionNotify)

        return;

    if (e->type == ButtonPress) {

        if (e->xbutton.button == 1) { }

        if (e->xbutton.button == 3) { }

    }

    if (e->type == MotionNotify) {

        if (savex != mx || savey != my) {

            /* mouse moved */

            savex = mx;

            savey = my;

            if (++count >= 20) {

                (child) ? write(1, "c", 1) : write(1, "m", 1);

                count = 0;

            }

        }

    }

}

void make_child_window(void)

{

    pid_t pid = fork();

    if (pid == 0) {

        /* child */

        char *arg[] = { myargv[0], "--child--", NULL };

        extern char **environ;

        execve(myargv[0], arg, environ);

        exit(0);

    } else {

        /* parent */

        if (g.n < MAX_CHILDREN) {

            g.pidlist[g.n++] = pid;

        }

    }

}

void make_xeyes_window(void)

{

    pid_t pid = fork();

    if (pid == 0) {

        /* child */

        char *arg[] = { "/usr/bin/xeyes", NULL };

        extern char **environ;

        execve(arg[0], arg, environ);

        exit(0);

    } else {

        if (g.n < MAX_CHILDREN) {

            g.pidlist[g.n++] = pid;

        }

    }

}

int check_keys(XEvent *e)

{

    int key;

    if (e->type != KeyPress && e->type != KeyRelease)

        return 0;

    key = XLookupKeysym(&e->xkey, 0);

    if (e->type == KeyPress) {

        switch (key) {

            case XK_a:

                g.color = !g.color;

                break;

            case XK_c:

                make_child_window();

                break;

            case XK_x:

                make_xeyes_window();

                break;

            case XK_1:

                break;

            case XK_Escape:  // 1

    if (!child) {

        int i;

        for (i = 0; i < g.n; i++) {

            kill(g.pidlist[i], SIGTERM);

        }

    }

    return 1;                 // 8

        }

    }

    return 0;

}

void sigchild_handler(int sig)

{

    child_closed = 1;

    render();

}

void render(void)

{

    unsigned long fg;

    if (child) {

        fg = (g.color) ? 0x00ffffa0 : 0x00ffbb11;

    } else {

        fg = (g.color) ? 0x00ffbb11 : 0x0033bb11;

    }

    XSetForeground(g.dpy, g.gc, fg);

    XFillRectangle(g.dpy, g.win, g.gc, 0, 0, g.xres, g.yres);

    XSetForeground(g.dpy, g.gc, 0x00000000);//

    if (child) {

    XDrawString(g.dpy, g.win, g.gc, 20, 40,

                "Child window", 12);

} else {

    XDrawString(g.dpy, g.win, g.gc, 20, 40,

                "Parent window", 13);

    if (child_closed) {

        XDrawString(g.dpy, g.win, g.gc, 20, 70,

                    "My child closed.", 16);

    }

}

}

include <config.h>
#include <fltk/run.h>
//#include <fltk/x.h>
#include <fltk/fltk_cairo.h>
#include <fltk/DoubleBufferWindow.h>
#include <fltk/draw.h>
#include <fltk/math.h>

#include <fltk/Group.h>
#include <fltk/TabGroup.h>

#include <iostream>
#include <plplot.h>
#include <plstream.h>

using namespace fltk;

int nsteps = 1000;

class CairoWindow : public Window
{
   public:

      CairoWindow(int argc, const char *argv[])
         : Window(720,540), firstPass(true), ArgC(argc), ArgV(argv)
      {
         n = 0;
         resizable(this);                         // comment this out for fixed-size
         color(fltk::WHITE);                      // desired background color
     }

      CairoWindow(int x, int y, int w, int h, int argc, const char *argv[])
         : Window(x, y, w, h), firstPass(true), ArgC(argc), ArgV(argv)
      {
         n = 0;
         resizable(this);                         // comment this out for fixed-size
         color(fltk::WHITE);                      // desired background color
      }

      bool firstPass;
      int ArgC;
      const char **ArgV;

      void Init()
      {
         // Make sure the cairo context exists before attempting
         // to pass the pointer to plplot.
         if(fltk::cr != NULL)
         {
            // Perform the plplot initialization required by your
            // task.
            pls = new plstream();
            pls->parseopts(&ArgC, ArgV, PL_PARSE_FULL);

            ymin = -0.1;
            ymax = 0.1;

            tmin  = 0.;
            tmax  = 110.;
            tjump = 0.3;

            colbox     = 1;
            collab     = 3;
            styline[0] = colline[0] = 2;      // pens color and line style
            styline[1] = colline[1] = 3;
            styline[2] = colline[2] = 4;
            styline[3] = colline[3] = 5;

            legline[0] = "sum";                       // pens legend
            legline[1] = "sin";
            legline[2] = "sin*noi";
            legline[3] = "sin+noi";

            xlab = 0.; ylab = 0.25;   // legend position

            autoy = true;             // autoscale y
            acc   = true;

            pls->sdev("extcairo");
            pls->init();

            // Need to save the cairo context or else the rest of
            // the fltk drawing calls get out of whack...
            cairo_save(cr);

            pls->cmd(PLESC_DEVINIT, fltk::cr);

            cairo_restore(cr);

            pls->adv(0);
            pls->vsta();

            pls->sError(&pl_errcode, errmsg);

            pls->stripc( &id1, "bcnst", "bcnstv",
                          tmin, tmax, tjump, ymin, ymax,
                          xlab, ylab,
                          autoy, acc,
                          colbox, collab,
                          colline, styline, legline,
                          "t", "", "Strip chart demo" );

            if ( pl_errcode )
            {
               std::cout << errmsg << std::endl;
                delete pls;
                exit( 1 );
            }

            pls->sError(NULL, NULL);

            autoy = false; // autoscale y
            acc   = true;  // accumulate/

            y1 = y2 = y3 = y4 = 0.0;
            dt = 0.1;

            // Only after successfully calling this we can flip the
            // flag to false.
            firstPass = false;
         }
      }

      static void TimeOutCB(void *data)
      {
         CairoWindow *w = (CairoWindow *) data;
         // We check if we should run the real-time data
         // capture and computation, otherwise do not
         // reset the timeout and "stop" the real-time.
         if(w->n < nsteps)
         {
            // If this is the first time we called in here
            // then initialize the class and the plplot interface.
            if(w->firstPass == true)
            {
               w->Init();
            }
            else
            {
               // Otherwise, perform the real-time computation
               // and request a re-draw of the screen from the
               // fltk main loop.
               w->Compute();
               fltk::redraw();
            }
            fltk::repeat_timeout(0.01, w->TimeOutCB, data);
         }
      }

      void Compute()
      {
         // Compute values for each time-step
         t     = (double) n * dt;
        //noise = (1.0 * (rand() / (RAND_MAX + 1.0))) - 0.5;
        noise = pls->randd() - 0.5;
        y1    = y1 + noise;
        y2    = sin( t * M_PI / 18. );
        y3    = y2 * noise;
        y4    = y2 + noise / 3.;

        ++n;
      }

      void draw()
      {
         // So we wait to see if this is the first time we called into here
         // and if it is, then we set timeout. Now go to TimeOutCB to follow
         // the sequence of events.
         if(firstPass == false)
         {
            cairo_save(cr);

            // For a reason I don't understand, adding the
            // cairo_save/restore calls in Init() when passing
            // the cairo context in pls->cmd(PLESC_DEVINIT, fltk::cr)
            // causes the plot to draw up-side-down, so we reflect
            // it back to right-side-up.
            cairo_translate(cr, 0.0, h());
            cairo_scale(cr, 1.0, -1.0);

            if ( n % 2 )
               pls->stripa( id1, 0, t, y1 );
            if ( n % 3 )
                pls->stripa( id1, 1, t, y2 );
            if ( n % 4 )
                pls->stripa( id1, 2, t, y3 );
            if ( n % 5 )
                pls->stripa( id1, 3, t, y4 );

            cairo_restore(cr);
         }
         else
         {
            fltk::add_timeout(0.01, this->TimeOutCB, this);
         }
      }

      static PLINT pl_errcode;
      static char  errmsg[160];

      PLINT n;
   private:
      plstream *pls;

      PLINT      id1;
      bool       autoy, acc;
      PLFLT      y1, y2, y3, y4, ymin, ymax, xlab, ylab;
      PLFLT      t, tmin, tmax, tjump, dt, noise;
      PLINT      colbox, collab, colline[4], styline[4];
      const char *legline[4];
};

PLINT CairoWindow::pl_errcode   = 0;
char  CairoWindow:: errmsg[160] = "";

int main(int argc, char** argv)
{
   fltk::Window *w = new fltk::Window(720, 540);
   w->begin();
   {
      fltk::TabGroup *t = new fltk::TabGroup(0, 0, w->w(), w->h());
      t->begin();
      {
         //fltk::Group *g = new fltk::Group(0, 0, t->w(), t->h() - 24, " Plot ");
         //g->box(fltk::EMBOSSED_BOX);
         //g->begin();
         {
            const char *constArgv[argc];
            for(int i = 0; i < argc; ++i)
            {
               constArgv[i] = "\0"; //argv[i];
            }

            // Create the window
            //CairoWindow *plotWin = new CairoWindow(0, 0, g->w(), g->h(), argc, constArgv);
            CairoWindow *plotWin = new CairoWindow(0, 0, t->w(), t->h() - 24, argc, constArgv);

         }
         //g->end();
      }
      t->end();
   }
   w->end();
   w->show();

   // The plplot library expects argv as const pointers
   // but fltk does not, so we create a const copy to pass
   // to plplot.
   //const char *constArgv[argc];
   //for(int i = 0; i < argc; ++i)
   //{
   //   constArgv[i] = argv[i];
   //}

   //// Create the window
   //CairoWindow window(argc, constArgv);
   //window.show(argc,argv);

   // In fltk, ONLY after calling the main loop in run() will the
   // fltk cairo context be created and the surface initialized.
   // Trying to operate on the fltk cairo context before calling run()
   // will result in the use of a null pointer. From here go to draw() call
   // in CairoWindow() for more comments on these steps.
   return fltk::run();
}

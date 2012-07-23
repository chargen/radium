/* Copyright 2003 Kjetil S. Matheussen

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. */

#include <stdbool.h>
#include <math.h>

#include "MyWidget.h"
#include "Qt_Bs_edit_proc.h"
#include "Qt_instruments_proc.h"
#include "Qt_colors_proc.h"
#include "Qt_Menues_proc.h"
#include "Qt_Fonts_proc.h"

#include <qpainter.h>
#include <qstatusbar.h>
#include <qmainwindow.h>


#ifdef USE_QT4
//#include <QMainWindow>
#  include <Q3PointArray>
#  include <QPixmap>
#endif

#include <unistd.h>

#include <X11/Xlib.h>
#include "../X11/X11_keyboard_proc.h"

//#include "../X11/X11_Bs_edit_proc.h"
//#include "../X11/X11_MidiProperties_proc.h"

#include "../X11/X11_Qtstuff_proc.h"
#include "../X11/X11_ReqType_proc.h"

#include "../common/gfx_proc.h"
#include "../common/gfx_op_queue_proc.h"
#include "../common/settings_proc.h"


#ifdef USE_QT4
#  define DRAW_PIXMAP_ON_WIDGET(dst_widget, x, y, src_pixmap, from_x, from_y, width, height) \
     dst_widget->painter->drawPixmap(x,y,*(src_pixmap),from_x,from_y,width,height)
#  define DRAW_PIXMAP_ON_PIXMAP(dst_pixmap, x, y, src_pixmap, from_x, from_y, width, height) \
     dst_pixmap##_painter->drawPixmap(x,y,*(src_pixmap),from_x,from_y,width,height)
#else
#  define DRAW_PIXMAP_ON_WIDGET(dst_widget, x, y, src_pixmap, from_x, from_y, width, height) \
     bitBlt(dst_widget,x+XOFFSET,y+YOFFSET,src_pixmap,from_x,from_y,width,height)
#  define DRAW_PIXMAP_ON_PIXMAP(dst_pixmap, x, y, src_pixmap, from_x, from_y, width, height) \
     bitBlt(dst_pixmap,x,y,src_pixmap,from_x,from_y,width,height)
#endif


RPoints::RPoints() {
  this->qpa=new Q3PointArray(INITIALPOOLSIZE);

  this->num_points=0;

  //  super(0);
}

void RPoints::addPoint(int x,int y){
  if((int)this->qpa->size() <= (int)this->num_points){
    this->qpa->resize(this->qpa->size()*2);
  }
  this->qpa->setPoint(this->num_points,x,y);
  this->num_points++;
}

void RPoints::drawPoints(QPainter *qp){

  if(this->num_points==0) return;

  qp->drawPoints(*this->qpa,0,this->num_points);

  this->num_points=0;
}


MyWidget::MyWidget( struct Tracker_Windows *window,QWidget *parent, const char *name )
  : QFrame( parent, name, Qt::WStaticContents | Qt::WResizeNoErase | Qt::WRepaintNoErase | Qt::WNoAutoErase )
{
  this->qpixmap=NULL;
  this->window=window;

  setEditorColors(this);

  for(int lokke=0;lokke<8;lokke++){
    this->rpoints[lokke]=new RPoints();
  }

  this->setMouseTracking(true);

  //this->setFrameStyle(QFrame::Raised );
  this->setFrameStyle(QFrame::Sunken );
  this->setFrameShape(QFrame::Panel);
  this->setLineWidth(2);

}

MyWidget::~MyWidget()
{
}

static QMainWindow *g_main_window = NULL;;
static MyWidget *g_mywidget = NULL;

//#include <qpalette.h>
int GFX_CreateVisual(struct Tracker_Windows *tvisual){
  QFont font = QFont("Monospace");

  char *fontstring = SETTINGS_read_string((char*)"font",NULL);
  if(fontstring!=NULL)
    font.fromString(fontstring);

  setFontValues(tvisual, font);

  if(g_main_window!=NULL){
    tvisual->os_visual.main_window = g_main_window;
    tvisual->os_visual.widget = g_mywidget;

    tvisual->width=g_mywidget->get_editor_width();
    tvisual->height=g_mywidget->get_editor_height();
    
    //g_mywidget->qpixmap=new QPixmap(g_mywidget->width(),mywidget->height());
    //g_mywidget->qpixmap->fill( mywidget->colors[0] );		/* grey background */

    g_mywidget->window = tvisual;

    return 0;
  }

  //QMainWindow *main_window = new QMainWindow(NULL, "Radium", Qt::WStyle_Customize | Qt::WStyle_NoBorder);// | Qt::WStyle_Dialog);
  QMainWindow *main_window = new QMainWindow(NULL, "Radium");
  tvisual->os_visual.main_window = main_window;
#ifdef USE_QT4
  main_window->setAttribute(Qt::WA_PaintOnScreen);
  main_window->setAttribute(Qt::WA_OpaquePaintEvent);
  main_window->setAttribute(Qt::WA_NoSystemBackground);
#endif
  main_window->setBackgroundMode(Qt::NoBackground);

  //QPalette pal = QPalette(main_window->palette());
  //pal.setColor( QPalette::Active, QColorGroup::Background, Qt::green);
  //pal.setColor(main_window->backgroundRole(), Qt::blue);
  //main_window->setPalette(pal);
  //main_window->menuBar()->setPalette(pal);

  MyWidget *mywidget=new MyWidget(tvisual,main_window,"name");
  mywidget->setFocus();
#ifdef USE_QT4
  mywidget->setAttribute(Qt::WA_PaintOnScreen);
  mywidget->setAttribute(Qt::WA_OpaquePaintEvent);
  mywidget->setAttribute(Qt::WA_NoSystemBackground);
#endif
  main_window->setBackgroundMode(Qt::NoBackground);

  main_window->resize(800,400);
  mywidget->setMinimumWidth(400);
  mywidget->setMinimumHeight(200);

  main_window->setCaption("Radium editor window");
  main_window->statusBar()->message( "Ready", 2000 );

  {
    QStatusBar *status_bar = main_window->statusBar();
    mywidget->status_label = new QLabel(status_bar);//"");
    mywidget->status_label->setFrameStyle(QFrame::Sunken);
    //mywidget->status_frame->
    status_bar->addWidget(mywidget->status_label, 1, true);
  }

  // helvetica
  mywidget->font = font;

  //mywidget->font->setStyleHint(QFont::TypeWriter);
  //mywidget->font->setFixedPitch(false);

  initMenues(main_window->menuBar());

 if(tvisual->height==0 || tvisual->width==0){
    tvisual->x=0;
    tvisual->y=0;
    tvisual->width=mywidget->get_editor_width();
    tvisual->height=mywidget->get_editor_height();
  }

  tvisual->os_visual.widget=mywidget;

  mywidget->qpixmap=new QPixmap(mywidget->width(),mywidget->height());
#ifdef USE_QT3
  mywidget->qpixmap->setOptimization(QPixmap::BestOptim);
#endif
  mywidget->qpixmap->fill( mywidget->colors[0] );		/* grey background */

  mywidget->cursorpixmap=new QPixmap(mywidget->width(),mywidget->height());
#ifdef USE_QT3
  mywidget->cursorpixmap->setOptimization(QPixmap::BestOptim);
#endif
  mywidget->cursorpixmap->fill( mywidget->colors[7] );		/* the xored background color for the cursor.*/

  //BS_SetX11Window((int)main_window->x11AppRootWindow());
  //X11_MidiProperties_SetX11Window((int)main_window->x11AppRootWindow());

  g_main_window = main_window;
  g_mywidget = mywidget;

  return 0;
}

int GFX_ShutDownVisual(struct Tracker_Windows *tvisual){

  close_all_instrument_widgets();

  // Reuse it instead.
#if 0
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;

  //BS_StopXSend();
  //X11_MidiProperties_StopXSend();

  delete mywidget->qpixmap;
  delete mywidget->cursorpixmap;

  mywidget->close();
#endif
  return 0;
}

void GFX_SetMinimumWindowWidth(struct Tracker_Windows *tvisual, int width){
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;
  mywidget->setMinimumWidth(width);
}


static void QGFX_bouncePoints(MyWidget *mywidget){
  for(int lokke=0;lokke<8;lokke++){
    mywidget->qpixmap_painter->setPen(mywidget->colors[lokke]);
    mywidget->rpoints[lokke]->drawPoints(mywidget->qpixmap_painter);
  }
}

void QGFX_C2V_bitBlt(
		    struct Tracker_Windows *window,
		    int from_x1,int from_x2,
		    int to_y
		    ){
  MyWidget *mywidget=(MyWidget *)window->os_visual.widget;

  QGFX_bouncePoints(mywidget);

  DRAW_PIXMAP_ON_WIDGET(mywidget,
 	 from_x1,to_y,
	 mywidget->cursorpixmap,
	 // The next two lines are the ones that should be used.
	 //	 from_x1,0,
	 //	 from_x2-from_x1+1,window->fontheight
	 // The next two lines are for debugging.
	 from_x1,0,
	 window->wblock->a.x2+-from_x1+1+100,window->fontheight
	 );
}


/* window,x1,x2,x3,x4,height, y pixmap */
void QGFX_C_DrawCursor(
				      struct Tracker_Windows *window,
				      int x1,int x2,int x3,int x4,int height,
				      int y_pixmap
				      ){
  MyWidget *mywidget=(MyWidget *)window->os_visual.widget;

  QGFX_bouncePoints(mywidget);

  mywidget->cursorpixmap_painter->fillRect(x1,0,x4,height,mywidget->colors[7]);
  mywidget->cursorpixmap_painter->fillRect(x2,0,x3-x2+1,height,mywidget->colors[1]);

  //mywidget->cursorpixmap_painter->setCompositionMode(QPainter::CompositionMode_Xor);

  // TODO: fix Qt4
#ifdef USE_QT3
  bitBlt(
         mywidget->cursorpixmap,
         0,0,
         mywidget->qpixmap,
         0,y_pixmap,
         x4+1,height
         ,Qt::XorROP
         );
#endif
}


void QGFX_P2V_bitBlt(
		    struct Tracker_Windows *window,
		    int from_x,int from_y,
		    int to_x,int to_y,
		    int width,int height
		    ){
  
  MyWidget *mywidget=(MyWidget *)window->os_visual.widget;

  QGFX_bouncePoints(mywidget);

  DRAW_PIXMAP_ON_WIDGET(
                        mywidget,
                        to_x,to_y,
                        mywidget->qpixmap,
                        from_x,from_y,
                        width,height
                        );

  /*
  QGFX_C2V_bitBlt(
		 window,
		 from_x,width,
		 (window->wblock->curr_realline-window->wblock->top_realline)*window->fontheight+window->wblock->t.y1
		 );
  */
}

static void draw_filled_box(MyWidget *mywidget,QPainter *painter,int color,int x,int y,int x2,int y2);

void QGFX_P_FilledBox(struct Tracker_Windows *tvisual,int color,int x,int y,int x2,int y2){
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;
  draw_filled_box(mywidget,mywidget->qpixmap_painter,color,x,y,x2,y2);
}

void QGFX_P_Box(struct Tracker_Windows *tvisual,int color,int x,int y,int x2,int y2){
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;
  mywidget->qpixmap_painter->setPen(mywidget->colors[color]);
  mywidget->qpixmap_painter->drawRect(x,y,x2-x+1,y2-y+1);
}


static QColor mix_colors(const QColor &c1, const QColor &c2, float how_much){
  float a1 = how_much;
  float a2 = 1.0f-a1;

  if(c1.red()==0 && c1.green()==0 && c1.blue()==0){ // some of the black lines doesn't look look very good.
    int r = 34*a1 + c2.red()*a2;
    int g = 34*a1 + c2.green()*a2;
    int b = 34*a1 + c2.blue()*a2;

    return QColor(r,g,b);
  }else{

    int r = c1.red()*a1 + c2.red()*a2;
    int g = c1.green()*a1 + c2.green()*a2;
    int b = c1.blue()*a1 + c2.blue()*a2;

    return QColor(r,g,b);
  }
}


// Xiaolin Wu's line algorithm (antialized lines)
//
// Code copied from http://rosettacode.org/wiki/Xiaolin_Wu's_line_algorithm and slightly modified

static void plot(int x, int y, QPainter *painter, const QColor &background, const QColor &foreground, float brightness){
  if(brightness>1.0f)
    brightness=1.0f;

  painter->setPen(mix_colors(background, foreground, brightness));
  painter->drawPoint(x,y);
}

#define ipart_(X) ((int)(X)) // Note, for negative numbers, floor must be used instead. (no negative numbers used here though)
#define round_(X) ((int)(((double)(X))+0.5))
#define fpart_(X) (((double)(X))-(double)ipart_(X))
#define rfpart_(X) (1.0-fpart_(X)) 
#define swap_(a, b) do{ __typeof__(a) tmp;  tmp = a; a = b; b = tmp; }while(0)

static void draw_line_antialias(
                                QPainter *painter,
                                unsigned int x1, unsigned int y1,
                                unsigned int x2, unsigned int y2,
                                const QColor &background,
                                const QColor &foreground
                                )
{
  double dx = (double)x2 - (double)x1;
  double dy = (double)y2 - (double)y1;

  if ( fabs(dx) > fabs(dy) ) {

    if ( x2 < x1 ) {
      swap_(x1, x2);
      swap_(y1, y2);
    }


    double gradient = dy / dx;
    double xend = round_(x1);
    double yend = y1 + gradient*(xend - x1);
    double xgap = rfpart_(x1 + 0.5);
    int xpxl1 = xend;
    int ypxl1 = ipart_(yend);
    plot(xpxl1, ypxl1, painter, foreground, background, rfpart_(yend)*xgap);
    plot(xpxl1, ypxl1+1, painter, foreground, background, fpart_(yend)*xgap);
    double intery = yend + gradient;
 
    xend = round_(x2);
    yend = y2 + gradient*(xend - x2);
    xgap = fpart_(x2+0.5);
    int xpxl2 = xend;
    int ypxl2 = ipart_(yend);
    plot(xpxl2, ypxl2, painter, foreground, background, rfpart_(yend) * xgap);
    plot(xpxl2, ypxl2 + 1, painter, foreground, background, fpart_(yend) * xgap);
 
    int x;
    for(x=xpxl1+1; x <= (xpxl2-1); x++) {
      plot(x, ipart_(intery), painter, foreground, background, rfpart_(intery));
      plot(x, ipart_(intery) + 1, painter, foreground, background, fpart_(intery));
      intery += gradient;
    }

  } else {

    if ( y2 < y1 ) {
      swap_(x1, x2);
      swap_(y1, y2);
    }

    double gradient = dx / dy;
    double yend = round_(y1);
    double xend = x1 + gradient*(yend - y1);
    double ygap = rfpart_(y1 + 0.5);
    int ypxl1 = yend;
    int xpxl1 = ipart_(xend);
    plot(xpxl1, ypxl1, painter, foreground, background, rfpart_(xend)*ygap);
    plot(xpxl1, ypxl1+1, painter, foreground, background, fpart_(xend)*ygap);
    double interx = xend + gradient;
 
    yend = round_(y2);
    xend = x2 + gradient*(yend - y2);
    ygap = fpart_(y2+0.5);
    int ypxl2 = yend;
    int xpxl2 = ipart_(xend);
    plot(xpxl2, ypxl2, painter, foreground, background, rfpart_(xend) * ygap);
    plot(xpxl2, ypxl2 + 1, painter, foreground, background, fpart_(xend) * ygap);
 
    int y;
    for(y=ypxl1; y <= (ypxl2); y++) {
      plot(ipart_(interx), y, painter, foreground, background, rfpart_(interx));
      plot(ipart_(interx) + 1, y, painter, foreground, background, fpart_(interx));
      interx += gradient;
    }
  }
}
#undef swap_
#undef plot_
#undef ipart_
#undef fpart_
#undef round_
#undef rfpart_


void QGFX_P_Line(struct Tracker_Windows *tvisual,int color,int x,int y,int x2,int y2){
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;

  if(x!=x2 && y!=y2){

    draw_line_antialias(mywidget->qpixmap_painter,x,y,x2,y2,mywidget->colors[0],mywidget->colors[color]);

  }else{

    mywidget->qpixmap_painter->setPen(mywidget->colors[color]);

#if 0
    QPen pen(mywidget->colors[color],4,Qt::SolidLine);  
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    mywidget->qpixmap_painter->setPen(pen);
#endif

    mywidget->qpixmap_painter->drawLine(x,y,x2,y2);
    //  printf("drawline, x: %d, y: %d, x2: %d, y2: %d\n",x,y,x2,y2);
  }

}

void QGFX_P_Point(
	struct Tracker_Windows *tvisual,
	int color,
	int x,int y
	)
{
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;

  color=color>7?7:color<0?0:color;

#if 1
  mywidget->rpoints[color]->addPoint(x,y);
#else
  mywidget->qpixmap_painter->setPen(mywidget->colors[color]);
  mywidget->qpixmap_painter->drawPoint(x,y);
#endif
}


static void draw_text(struct Tracker_Windows *tvisual,
                      QPainter *painter,
                      int color,
                      char *text,
                      int x,
                      int y,
                      int width,
                      int flags
){
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;

  if(flags & TEXT_CLIPRECT){
    if(width==TEXT_IGNORE_WIDTH){
      RError("width can not be TEXT_IGNORE_WIDTH when using the TEXT_CLIPRECT flag");
    }else{
      if(width<=0)
        return;
      painter->setClipRect(x,y,width,tvisual->fontheight+20);
    }
    painter->setClipping(true);
  }

  if(flags & TEXT_INVERT){
    draw_filled_box(mywidget,painter,color,x,y,x+(tvisual->fontwidth*strlen(text)),y+tvisual->fontheight);
  }else if(flags & TEXT_CLEAR){
    draw_filled_box(mywidget,painter,0,x,y,x+(tvisual->fontwidth*strlen(text)),y+tvisual->fontheight);
  }

  painter->setPen(mywidget->colors[flags&TEXT_INVERT ? 0 : color]);

  if(flags & TEXT_BOLD){
    mywidget->font.setBold(true);
    painter->setFont(mywidget->font);
  }

  if(flags & TEXT_CENTER){
    QRect rect(x,y,tvisual->fontwidth*strlen(text),tvisual->org_fontheight);
    painter->drawText(rect, Qt::AlignVCenter, text);
  }else{
    painter->drawText(x,y+tvisual->org_fontheight-1,text);
  }

  if(flags & TEXT_BOLD){
    mywidget->font.setBold(false);
    painter->setFont(mywidget->font);
  }

  if(flags & TEXT_CLIPRECT){
    painter->setClipping(false);
  }
}                      

void QGFX_P_Text(
	struct Tracker_Windows *tvisual,
	int color,
	char *text,
	int x,
	int y,
        int width,
	int flags
){
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;
  draw_text(tvisual,mywidget->qpixmap_painter,color,text,x,y,width,flags);
}

void QGFX_Line(struct Tracker_Windows *tvisual,int color,int x,int y,int x2,int y2){
  
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;
//  QColor *qcolor=mywidget->colors[color];

  mywidget->painter->setPen(mywidget->colors[color]);
  mywidget->painter->drawLine(x,y,x2,y2);
//  printf("drawline, x: %d, y: %d, x2: %d, y2: %d\n",x,y,x2,y2);
  
}


void QGFX_All_Line(struct Tracker_Windows *tvisual,int color,int x,int y,int x2,int y2){
  QGFX_Line(tvisual,color,x,y,x2,y2);
}


void QGFX_Box(struct Tracker_Windows *tvisual,int color,int x,int y,int x2,int y2){
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;
  mywidget->painter->setPen(mywidget->colors[color]);
  mywidget->painter->drawRect(x,y,x2-x+1,y2-y+1);
}

static void draw_filled_box(MyWidget *mywidget, QPainter *painter,int color,int x,int y,int x2,int y2){
  painter->fillRect(x,y,x2-x+1,y2-y+1,mywidget->colors[color]);
}

void QGFX_FilledBox(struct Tracker_Windows *tvisual,int color,int x,int y,int x2,int y2){
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;
  draw_filled_box(mywidget,mywidget->painter,color,x,y,x2,y2);
}


void QGFX_Slider_FilledBox(struct Tracker_Windows *tvisual,int color,int x,int y,int x2,int y2){
  QGFX_FilledBox(tvisual,color,x,y,x2,y2);
}


void QGFX_All_FilledBox(struct Tracker_Windows *tvisual,int color,int x,int y,int x2,int y2){
  QGFX_FilledBox(tvisual,color,x,y,x2,y2);
}

void QGFX_Text(
	struct Tracker_Windows *tvisual,
	int color,
	char *text,
	int x,
	int y,
        int width,
        int flags
){
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;
  draw_text(tvisual,mywidget->painter,color,text,x,y,width,flags);
}

static void Qt_BLine(struct Tracker_Windows *tvisual,int color,int x,int y,int x2,int y2){
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;

//  QColor *qcolor=mywidget->colors[color];

  mywidget->qpixmap_painter->setPen(mywidget->colors[color]);
  mywidget->qpixmap_painter->drawLine(x,y,x2,y2);
//  printf("drawline, x: %d, y: %d, x2: %d, y2: %d\n",x,y,x2,y2);
}

void QGFX_P_DrawTrackBorderDouble(
	struct Tracker_Windows *tvisual,
	int x, int y, int y2
){
  Qt_BLine(tvisual,1,x,y,x,y2);
  Qt_BLine(tvisual,2,x+1,y,x+1,y2);
}

void QGFX_P_DrawTrackBorderSingle(
	struct Tracker_Windows *tvisual,
	int x, int y, int y2
){
  Qt_BLine(tvisual,2,x,y,x,y2);
}

void QGFX_V_DrawTrackBorderDouble(
	struct Tracker_Windows *tvisual,
	int x, int y, int y2
){
  //  Qt_BLine(tvisual,2,x,y,x,y2);
  //Qt_BLine(tvisual,2,x+1,y,x+1,y2);
}

void QGFX_V_DrawTrackBorderSingle(
	struct Tracker_Windows *tvisual,
	int x, int y, int y2
){
  //  Qt_BLine(tvisual,2,x,y,x,y2);
}


void GFX_SetWindowTitle(struct Tracker_Windows *tvisual,char *title){
  g_main_window->setCaption(title);
}

void GFX_SetStatusBar(struct Tracker_Windows *tvisual,char *title){
  //QMainWindow *main_window = (QMainWindow *)tvisual->os_visual.main_window;
  //main_window->statusBar()->message(title);

  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;
  mywidget->status_label->setText(title);

#if 0
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;
  //mywidget->setCaption(title);

  mywidget->painter->fillRect(0,mywidget->height()-28,mywidget->width(),mywidget->height(),mywidget->colors[0]);

  mywidget->painter->setPen(mywidget->colors[1]);
  mywidget->painter->drawText(0,mywidget->height()-28+tvisual->org_fontheight+2,title);
#endif
}

void QGFX_Scroll(
	struct Tracker_Windows *tvisual,
	int dx,int dy,
	int x,int y,
	int x2,int y2
){
  RWarning("QGFX_Scroll is not implemented. (Should not be needed).\n");
#if 0
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;
  //  const QPaintDevice *ai=(QPaintDevice *)mywidget;

  QGFX_bouncePoints(mywidget);

  mywidget->painter->drawPixmap(
		   x+dx,y+dy,
		   *mywidget,
		   x,y,x2-x+1,y2-y+1
		   );
#endif
  /*
	if(dy<0){
	  //RectFill(tvisual->os_visual.window->RPort,(LONG)x,(LONG)y2+dy,(LONG)x2,(LONG)y2);
		QGFX_FilledBox(tvisual,0,x,y2+dy,x2,y2);
	}else{
	  //		RectFill(tvisual->os_visual.window->RPort,(LONG)x,(LONG)y,(LONG)x2,(LONG)y+dy);
		QGFX_FilledBox(tvisual,0,x,y,x2,y+dy-1);
	}
  */
}

void QGFX_P_Scroll(
	struct Tracker_Windows *tvisual,
	int dx,int dy,
	int x,int y,
	int x2,int y2
){
  MyWidget *mywidget=(MyWidget *)tvisual->os_visual.widget;
  //  const QPaintDevice *ai=(QPaintDevice *)mywidget;

  QGFX_bouncePoints(mywidget);

  DRAW_PIXMAP_ON_PIXMAP(
                        mywidget->qpixmap,
                        x+dx,y+dy,
                        mywidget->qpixmap,
                        x,y,x2-x+1,y2-y+1
                        );


  /*
	if(dy<0){
	  //RectFill(tvisual->os_visual.window->RPort,(LONG)x,(LONG)y2+dy,(LONG)x2,(LONG)y2);
		QGFX_FilledBox(tvisual,0,x,y2+dy,x2,y2);
	}else{
	  //		RectFill(tvisual->os_visual.window->RPort,(LONG)x,(LONG)y,(LONG)x2,(LONG)y+dy);
		QGFX_FilledBox(tvisual,0,x,y,x2,y+dy-1);
	}
  */
}


void QGFX_ScrollDown(
	struct Tracker_Windows *tvisual,
	int dx,int dy,
	int x,int y,
	int x2,int y2
){return ;}



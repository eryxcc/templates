// biblioteka do kolorow, grafiki, i zapisu w SDL i w formacie SVG
// w trakcie budowy - Eryk Kopczynski

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>
#include <math.h>
#include <complex>
#include <string>
#include <vector>

#ifdef USDL
#include <SDL/SDL.h>

#ifdef UTTF
#include <SDL/SDL_ttf.h>
#endif

#ifdef UGFX
#include <SDL/SDL_gfxPrimitives.h>
#endif

#else
typedef unsigned char Uint8;
#endif

#ifdef UGD
#include <gd.h>
#endif

using namespace std;

typedef long double ld;
#ifndef Size
#define Size(x) (int((x).size()))
#endif

const char *fontfile = "VeraBd.ttf";

// colors (int)
//==============
// uzywamy zapisu: 0xAARRGGBB (jak w SDLu)

ld cta(int col) {
  col >>= 24;
  col &= 0xFF;
  return col / 255.0;
  }

bool isfilled(int col) { return col & 0xFF000000; }

// SDL_gfx wymaga koloru w innym formacie...
int ctgfx(int col) {
  return (col << 8) | ((col >> 24) & 0xFF);
  // (col & 0xFFFFFF) << 8 + (Uint32(col & 0xFF000000)) >> 24;
  }

int gltc(int col) {
  return ((col >> 8) & 0xFFFFFF) | ((col << 24) & 0xFF000000);
  // (col & 0xFFFFFF) << 8 + (Uint32(col & 0xFF000000)) >> 24;
  }

// mieszanie 2 kolorow (bez alpha)
int mixcolor(int col, int ncol) {
  return ((col&0xFEFEFEFE) + (ncol&0xFEFEFEFE)) >> 1;
  }

// i-ta skladowa koloru (0=B, 1=G, 2=R, 3=A), mozna zapisywac
Uint8& part(int& col, int i) {
  Uint8* c = (Uint8*) &col;
  return c[i];
  }

Uint8 mixpart(Uint8 a, Uint8 b, Uint8 c) {
  return (a*(255-c)+b*c+127)/255;
  }

void alphablend(int& col, int ncol, int nv) {
  int s3 = part(ncol, 3);
  for(int i=0; i<3; i++) {
    part(col, i) = 
      (part(col, i) * (255-s3) + part(ncol, i) * s3 + 128) >> 8;
    }
  part(col, 3) = nv;
  }

// 2d vector
//===========

struct xform;

struct vec {
  ld x, y;
  vec(ld _x, ld _y) : x(_x), y(_y) { }
  vec() : x(0), y(0) {}
  virtual void tform(const xform& x);
  };

vec& operator += (vec& a, const vec b) { a.x += b.x; a.y += b.y; return a; }
vec& operator -= (vec& a, const vec b) { a.x -= b.x; a.y -= b.y; return a; }

// coordinatewise multiplication and division
vec& operator *= (vec& a, const vec b) { a.x *= b.x; a.y *= b.y; return a; }
vec& operator *= (vec& a, ld scalar) { a.x *= scalar; a.y *= scalar; return a; }
vec& operator /= (vec& a, const vec b) { a.x /= b.x; a.y /= b.y; return a; }
vec& operator /= (vec& a, ld scalar) { a.x /= scalar; a.y /= scalar; return a; }

vec  operator +  (vec a, const vec b) { return a+=b; }
vec  operator -  (vec a, const vec b) { return a-=b; }
vec  operator *  (vec a, const vec b) { return a*=b; }
vec  operator /  (vec a, const vec b) { return a/=b; }

vec  operator *  (vec a, ld scalar)   { return a*=scalar; }
vec  operator *  (ld scalar, vec a)   { return a*=scalar; }
vec  operator /  (vec a, ld scalar)   { return a/=scalar; }
vec  operator /  (ld scalar, vec a)   { return a/=scalar; }


// cross product
ld   operator ^  (const vec a, const vec b) { return a.x*b.y - a.y*b.x; }

// scalar product
ld   operator |  (const vec a, const vec b) { return a.x*b.x + a.y*b.y; }

// complex multiplication
vec  operator &  (const vec a, const vec b) { return vec(a.x*b.x-a.y*b.y, a.x*b.y+a.y*b.x); }

// rectangles
// (nieprzetestowane!)
//============

struct rec {
  vec c1, c2;
  rec(const vec& _1, const vec& _2) : c1(_1), c2(_2) { }
  };

bool inrec(const vec& v, const rec &r) {
  return v.x >= r.c1.x && v.y >= r.c1.y && v.x <= r.c2.x && v.y <= r.c2.y;
  }

// transformations
// (nieprzetestowane!)
//=================

struct xform {
  ld mxx, mxy, myx, myy, tx, ty;
  xform(ld _mxx, ld _mxy, ld _myx, ld _myy, ld _tx, ld _ty) : 
    mxx(_mxx), mxy(_mxy), myx(_myx), myy(_myy), tx(_tx), ty(_ty) {}
  };

ld det(const xform& x) { return x.mxx*x.mxy - x.mxy * x.myx; }

ld scalefactor(const xform& x) { return sqrt(abs(det(x))); }

xform xshift(const vec& _s) { return xform(0,0,0,0, _s.x, _s.y); }

xform xscale(ld m) { return xform(m,0,0,m, 0,0); }

xform xscale(const vec& _m) { return xform(_m.x,0,0,_m.y, 0,0); }

xform xspin(ld alpha) { 
  ld s = sin(alpha), c = cos(alpha);
  return xform(c, s, -s, c, 0, 0); 
  }

void vec::tform(const xform& a) {
  (*this) = vec(a.tx + a.mxx * x + a.myx * y, a.ty + a.mxy * x + a.myy * y);
  }

xform operator * (const xform &a, const xform &b) {

  return xform(
    a.mxx*b.mxx+a.myx*b.mxy,
    a.mxy*b.mxx+a.myy*b.mxy,
    a.mxx*b.myx+a.myx*b.myy,
    a.mxy*b.myx+a.myy*b.myy,  
    a.tx + a.mxx*b.tx + a.myx*b.ty, 
    a.ty + a.mxy*b.tx + a.myy*b.ty
    );
  }

// vec operator * (const xform& x, const vec a) { return (a & x.amul) + x.shift; }

// styles
//========

struct style {
  int stroke, fill;
  ld width; // uwaga: 'width' dziala tylko w drawSvg, nie w drawSDL
  style(int st, int fi, ld wi) : stroke(st), fill(fi), width(wi) {}
  };

char* stylestr(style &s) {
  static char buf[600];
  sprintf(buf, "style=\"stroke:#%06x;stroke-opacity:%.3Lf;stroke-width:%Lfpx;fill:#%06x;fill-opacity:%.3Lf\"",
    s.stroke & 0xFFFFFF, cta(s.stroke),
    s.width,
    s.fill & 0xFFFFFF, cta(s.fill)
    );
  return buf;
  }

// pictures
//==========

struct picturegroup;
typedef struct picture* pic;

struct picture {
  virtual void drawSvg(FILE *f) { printf("not implemented\n"); }
#ifdef USDL
  virtual void drawSDL(SDL_Surface *s) { printf("not implemented\n"); }
#endif
  virtual void drawPic(picturegroup& p);
  virtual void tform(const xform& x) { printf("not implemented\n"); }
  virtual pic clone() const { return new picture(); }
  virtual ~picture() {}
  };

struct line : picture {
  style b;
  vec v1, v2;
  line(style _b, vec _v1, vec _v2) : b(_b), v1(_v1), v2(_v2) { }
  void drawSvg(FILE *f) { 
    fprintf(f, "<line x1='%Lf' y1='%Lf' x2='%Lf' y2='%Lf' %s/>\n",
      v1.x, v1.y, v2.x, v2.y, stylestr(b)
      );
    }
#ifdef UGFX
  void drawSDL(SDL_Surface *s) { 
    aalineColor(s, v1.x, v1.y, v2.x, v2.y, ctgfx(b.stroke));
    }
#endif
  virtual void tform(const xform& x) { v1.tform(x); v2.tform(x); }
  pic clone() const { return new line(b, v1, v2); }
  };

struct text : picture {
  style b;
  vec v;
  ld size;
  string txt;
  text(style _b, vec _v, ld _s, const string& _txt) : 
    b(_b), v(_v), size(_s), txt(_txt) { }
  void drawSvg(FILE *f) { 
    fprintf(f, "<text x='%Lf' y='%Lf' font-size='%Lf' %s>%s</text>\n",
      v.x, v.y, size, stylestr(b), txt.c_str()
      );
    }
  virtual void tform(const xform& x) { v.tform(x); printf("size not tf\n"); }
  pic clone() const { return new text(b, v, size, txt); }
  };

struct path : picture {
  style b;
  vector<vec> lst;
  path(style _b) : b(_b)  { }
  path(style _b, const vector<vec>& _l) : b(_b), lst(_l) { }
  void add(vec v) { lst.push_back(v); }
  void drawSvg(FILE *f) { 
    if(Size(lst) < 2) return;
    for(int i=0; i<Size(lst); i++) {
      if(i == 0)
        fprintf(f, "<path d=\"M ");
      else
        fprintf(f, " L ");
      fprintf(f, "%Lf %Lf", lst[i].x, lst[i].y);
      }
    
    fprintf(f, "\" %s/>\n", stylestr(b));
    }
#ifdef UGFX
  void drawSDL(SDL_Surface *s) { 
    Sint16 sx[Size(lst)];
    Sint16 sy[Size(lst)];
    for(int i=0; i<Size(lst); i++) sx[i] = Sint16(lst[i].x), sy[i] = Sint16(lst[i].y);

//  for(int i=0; i<Size(lst); i++) sx[i] = Sint16(lst[i].x), sy[i] = Sint16(lst[i].y);

    if(isfilled(b.fill))
      filledPolygonColor(s, sx, sy, Size(lst), ctgfx(b.fill));
    
    /* if(isfilled(b.stroke))
      aapolygonColor(s, sx, sy, Size(lst), ctgfx(b.stroke)); */
    
    for(int i=1; i<Size(lst); i++)
      aalineColor(s, sx[i-1], sy[i-1], sx[i], sy[i], ctgfx(b.stroke));
    }
#endif
  void tform(const xform& x) { 
    for(int i=0; i<Size(lst); i++) lst[i].tform(x);
    }
  pic clone() const { return new path(b, lst); }
  };

struct circle : picture {
  style b;
  vec center;
  ld radius;
  circle(style _b, vec _c, ld _r) : b(_b), center(_c), radius(_r)  { }
  void drawSvg(FILE *f) { 
    fprintf(f, "<circle cx='%Lf' cy='%Lf' r='%Lf' %s/>\n",
      center.x, center.y, radius, stylestr(b));
    }
  pic clone() const { return new circle(b, center, radius); }
#ifdef UGFX
  void drawSDL(SDL_Surface *s) { 
    if(isfilled(b.fill))
      filledCircleColor(s, Sint16(center.x), Sint16(center.y), Sint16(radius), ctgfx(b.fill));
    if(isfilled(b.stroke))
      aacircleColor(s, Sint16(center.x), Sint16(center.y), Sint16(radius), ctgfx(b.stroke));
    }
#endif
  };

struct picturegroup : picture {
  vector<pic> v;
  void addPtr(pic p) { v.push_back(p); }
  void add(const picture& p) { v.push_back(p.clone()); }
  ~picturegroup() { for(int i=0; i<Size(v); i++) delete v[i]; }
  pic clone() const {
    auto g = new picturegroup;
    for(int i=0; i<Size(v); i++) g->v.push_back(v[i]->clone());
    return g;
    }
  void drawSvg(FILE *f) {
    for(int i=0; i<Size(v); i++) v[i]->drawSvg(f);
    }
#ifdef UGFX
  void drawSDL(SDL_Surface *s) {
    for(int i=0; i<Size(v); i++) v[i]->drawSDL(s);
    }
#endif
  virtual void tform(const xform& x) { 
    for(int i=0; i<Size(v); i++) v[i]->tform(x);
    }
  };

void picture::drawPic(picturegroup &p) { p.add(*this); }

void drawSvg(const char *fname, int width, int height, pic p) {
  FILE *f = fopen(fname, "wt");
  fprintf(f, "<svg width=\"%d\" height=\"%d\">\n", width, height);
  p->drawSvg(f);
  fprintf(f, "</svg>\n");
  fclose(f);
  }

void drawSvg(string fname, int width, int height, pic p) {
  drawSvg(fname.c_str(), width, height, p);
  }


/*
void addtext(float x1, float y1, char *buf, int col, int fsize) {
  fprintf(f, "<text x='%f' y='%f' fill='#%06x' font-size='%d'>%s</text>\n",
    x1*SC, -y1*SC, col, fsize, buf);
  }

void addrect(float x1, float y1, float x2, float y2, float wi, int col, int icol) {
  fprintf(f, "<rect x='%f' y='%f' width='%f' height='%f' "
    "style=\"stroke:#%06x;fill:#%06x;stroke-opacity:1;fill-opacity:%d;stroke-width:%fpx\"/>\n",
    x1*SC, -y2*SC, (x2-x1)*SC, (y2-y1)*SC, col, icol, icol >= 0 ? 1 : 0, wi);
  }
*/

#ifdef USDL

int err;

SDL_Surface *surface; // ekran

#ifdef UTTF
TTF_Font *font[256];
#endif

// zwraca podany piksel z podanego surface -- mozna na to pisac (po SDL_Lock)
int& pix(int x, int y, SDL_Surface *s = surface) {
  if(x<0 || y<0 || x>=s->w || y>=s->h) return err;
  
  unsigned char *dst = (unsigned char*) s->pixels;
  dst += y*s->pitch;
  int* ptr = (int*) dst;
  return ptr[x];
  }

// inicjalizacja grafiki
void initGraph(int sx, int sy, const string& title, int flags) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL error\n");
    SDL_Quit();
    }

  surface = SDL_SetVideoMode(sx, sy, 32, flags);

  SDL_WM_SetCaption(title.c_str(), 0);
  SDL_EnableKeyRepeat(500, 10);
  SDL_EnableUNICODE(1);

#ifdef UTTF
  if(TTF_Init() != 0) {
    printf("TTF error\n");
    exit(2);
    }
#endif
  }

// pomocnicze
void zrobRozmiar(int siz) {
#ifdef UTTF
  if(!font[siz]) {
    font[siz] = TTF_OpenFont(fontfile, siz);
    if (font[siz] == NULL) {
      printf("error: Font error (%s)\n", TTF_GetError());
      exit(1);
      }
    }
#endif
  }

// pomocnicze
int textwidth(int siz, const string &str) {
#ifdef UTTF
  if(str.size() == 0) return 0;
  
  int w, h;
  zrobRozmiar(siz);
  TTF_SizeText(font[siz], str.c_str(), &w, &h);
  return w;
#else
  return 0;
#endif
  }

// wypisuje napis, alx=0 (left align), alx=2 (right align), alx=1 (centralnie)
bool displaystr(int x, int y, int size, string str, int color, int alx, SDL_Surface *tgt = surface) {

#ifdef UTTF
  if(str.size() == 0 || size <= 0) return false;

  SDL_Color col;
  col.r = (color >> 16) & 255;
  col.g = (color >> 8 ) & 255;
  col.b = (color >> 0 ) & 255;
  
  zrobRozmiar(size);

  // SDL_Color black;
 
  SDL_Surface *txt = TTF_RenderUTF8_Blended(font[size], str.c_str(), col);
  
  if(txt == NULL) return false;

  SDL_Rect rect;

  rect.w = txt->w;
  rect.h = txt->h;

  rect.x = x - rect.w * alx / 2;
  rect.y = y;// - rect.h/2;
  
  SDL_BlitSurface(txt, NULL, tgt, &rect); 
  SDL_FreeSurface(txt);
  
#endif
  return false;
  }

#include <gd.h>

// tworzymy pomocniczy Surface
SDL_Surface* emptySurface(int sx, int sy) {
  return SDL_CreateRGBSurface(SDL_SWSURFACE, sx, sy, 32, 0xFF<<16,0xFF<<8,0xFF,0xFF<<24);
  }

int alphamult = 1; // nie wiem czy 1 czy 2

// Surface na podstawie GD
SDL_Surface* GDtoSDL(gdImagePtr im) {
  SDL_Surface *s = emptySurface(im->sx, im->sy);
  for(int y=0; y<im->sy; y++)
  for(int x=0; x<im->sx; x++) {
    if(im->trueColor)
      pix(x, y, s) = im->tpixels[y][x];
    else {
      int p = im->pixels[y][x];
      int& px(pix(x, y, s));
      part(px, 2) = im->red[p];
      part(px, 1) = im->green[p];
      part(px, 0) = im->blue[p];
      part(px, 3) = 255 - alphamult * im->alpha[p];
      }
    part(pix(x, y, s), 3) = 255 - alphamult * part(pix(x, y, s), 3);
    }
  return s;
  }

// z GD do Surface
gdImagePtr SDLtoGD(SDL_Surface* s) {
  gdImagePtr im = gdImageCreateTrueColor(s->w, s->h);
  for(int y=0; y<im->sy; y++)
  for(int x=0; x<im->sx; x++) {
    part(pix(x, y, s), 3) = (255 - part(pix(x, y, s), 3)) / 2;
    im->tpixels[y][x] = pix(x, y, s);
    }
  return im;
  }

// czytanie z .png
// (wole uzywac biblioteki GD zamiast SDL_Image, bo w ten sposob moge tez
// zapisywac)
SDL_Surface* readPng(const char *fname) {
  FILE *f = fopen(fname, "rb");
  gdImagePtr im = gdImageCreateFromPng(f);
  SDL_Surface *s = GDtoSDL(im);
  gdImageDestroy(im);
  fclose(f);
  return s;
  }

SDL_Surface* readJpg(const char *fname) {
  FILE *f = fopen(fname, "rb");
  gdImagePtr im = gdImageCreateFromJpeg(f);
  SDL_Surface *s = GDtoSDL(im);
  gdImageDestroy(im);
  fclose(f);
  return s;
  }

void writePng(const char *fname, SDL_Surface* s) {
  FILE *f = fopen(fname, "wb");
  gdImagePtr im = SDLtoGD(s);
  gdImageSaveAlpha(im, 1);
  gdImagePng(im, f);
  gdImageDestroy(im);
  fclose(f);
  }

void writePngAlpha(const char *fname, SDL_Surface* s) {
  FILE *f = fopen(fname, "wb");
  gdImagePtr im = SDLtoGD(s);
  im->saveAlphaFlag = true;
  gdImageSaveAlpha(im, 1);
  gdImagePng(im, f);
  gdImageDestroy(im);
  fclose(f);
  }

void writeJpg(const char *fname, SDL_Surface* s, int q) {
  FILE *f = fopen(fname, "wb");
  gdImagePtr im = SDLtoGD(s);
  gdImageSaveAlpha(im, 1);
  gdImageJpeg(im, f, q);
  gdImageDestroy(im);
  fclose(f);
  }

#endif
#endif

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <math.h>

#ifdef M_PI
#define PI M_PI
#else
#define PI 3.1415926535897932384626433832795029L
#endif

void interp(const double *in, double *out, int n, int pm);
void drawBoundingBox(GLfloat x, GLfloat y, GLfloat z, GLfloat r);
void drawTube(double *opts, int on, int pm, double r);
void HSVtoRGB(float h, float s, float v, float rgb[4]);

inline double
magnitude(const double v[3]) {
  return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

inline double
normalize(double v[3]) {
  double mag = magnitude(v);
  if(mag != 0.0) {
    v[0] /= mag;
    v[1] /= mag;
    v[2] /= mag;
  }
  return mag;
}

inline void
cross(const double u[3], const double v[3], double r[3]) {
  r[0] =   u[1]*v[2] - u[2]*v[1];
  r[1] = -(u[0]*v[2] - u[2]*v[0]);
  r[2] =   u[0]*v[1] - u[1]*v[0];
}

inline double
dot(const double u[3], const double v[3]) {
  return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

inline void
multiply(const double mat[9], double v[3]) {
  double v2[3] = { mat[0]*v[0] + mat[1]*v[1] + mat[2]*v[2],
		   mat[3]*v[0] + mat[4]*v[1] + mat[5]*v[2],
		   mat[6]*v[0] + mat[7]*v[1] + mat[8]*v[2] };
  v[0] = v2[0];
  v[1] = v2[1];
  v[2] = v2[2];
}

inline void
clamp(float &x, float l, float h) {
  if(x < l) x = l;
  else if(x > h) x = h;
}

inline void
clamp(double &x, float l, float h) {
  if(x < l) x = l;
  else if(x > h) x = h;
}

void interp(const double *in, double *out, int n, int pm);
void drawBoundingBox(double x, double y, double z, double r);
void drawTube(double *opts, int on, int pm);  
void HSVtoRGB(float h, float s, float v, float rgb[4]);

#endif

#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include "graphics_utils.h"

//----------------------------------------------------------------------

static char const * const rcsid_graphics_utils_cpp = "$Id: graphics_utils.cpp,v 1.1 2004/01/24 23:26:34 huangwei Exp $";

//----------------------------------------------------------------------

void
interp(const double *in, double *out, int n, int pm) {
  pm++;
  
  double *b = new double[n];
  double *u = new double[n];
  double *v = new double[n];
  double *z = new double[n];

  int i, j, k;
  for(j=0; j<3; j++) {
    for(i=0; i<n-1; i++) {
      b[i] = 6 * (in[(i+1)*3+j] - in[i*3+j]) / pm;
    }
    u[1] = 2 * 2 * pm;
    v[1] = b[1] - b[0];
    for(i=2; i<n-1; i++) {
      u[i] = 2 * (2 * pm) - pm * pm / u[i-1];
      v[i] = b[i] - b[i-1] - pm * v[i-1] / u[i-1];
    }
    z[n-1] = 0;
    for(i = n-2; i>=1; i--) {
      z[i] = (v[i] - pm * z[i+1]) / u[i];
    }
    z[0] = 0;

    for(i=0; i<n; i++) {
      out[(i*pm+0)*3+j] = in[i*3+j];
      if(i < n-1) {
	for(k=1; k<pm; k++) {
	  double a = (z[i+1] - z[i]) / (6.0 * pm);
	  double b = z[i] / 2;
	  double c = -pm * z[i+1] / 6.0 - pm * z[i] / 3.0 + (in[(i+1)*3+j] - in[i*3+j]) / pm;
	  double dt = k;
	  double s = in[i*3+j] + dt * (c + dt * (b + dt * a));
	  out[(i*pm+k)*3+j] = s;
	}
      }
    }
  }
  
  delete[] b;
  delete[] u;
  delete[] v;
  delete[] z;
}

void
drawBoundingBox(GLfloat x, GLfloat y, GLfloat z, GLfloat r) {
  GLfloat color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };  
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);

  glBegin(GL_LINE_STRIP);
  glNormal3f(-.57735027f, -.57735027f, -.57735027f); glVertex3f(x-r, y-r, z-r);
  glNormal3f( .57735027f, -.57735027f, -.57735027f); glVertex3f(x+r, y-r, z-r);  
  glNormal3f( .57735027f,  .57735027f, -.57735027f); glVertex3f(x+r, y+r, z-r);
  glNormal3f(-.57735027f,  .57735027f, -.57735027f); glVertex3f(x-r, y+r, z-r);
  glNormal3f(-.57735027f, -.57735027f, -.57735027f); glVertex3f(x-r, y-r, z-r);

  glNormal3f(-.57735027f, -.57735027f,  .57735027f); glVertex3f(x-r, y-r, z+r);
  glNormal3f( .57735027f, -.57735027f,  .57735027f); glVertex3f(x+r, y-r, z+r);
  glNormal3f( .57735027f,  .57735027f,  .57735027f); glVertex3f(x+r, y+r, z+r);
  glNormal3f(-.57735027f,  .57735027f,  .57735027f); glVertex3f(x-r, y+r, z+r);
  glNormal3f(-.57735027f, -.57735027f,  .57735027f); glVertex3f(x-r, y-r, z+r);
  glEnd();
  
  glBegin(GL_LINES);
  glNormal3f(-.57735027f, -.57735027f, -.57735027f); glVertex3f(x-r, y-r, z-r);
  glNormal3f(-.57735027f, -.57735027f,  .57735027f); glVertex3f(x-r, y-r, z+r);
  
  glNormal3f( .57735027f, -.57735027f, -.57735027f); glVertex3f(x+r, y-r, z-r);
  glNormal3f( .57735027f, -.57735027f,  .57735027f); glVertex3f(x+r, y-r, z+r);
  
  glNormal3f( .57735027f,  .57735027f, -.57735027f); glVertex3f(x+r, y+r, z-r);
  glNormal3f( .57735027f,  .57735027f,  .57735027f); glVertex3f(x+r, y+r, z+r);
  
  glNormal3f(-.57735027f,  .57735027f, -.57735027f); glVertex3f(x-r, y+r, z-r);
  glNormal3f(-.57735027f,  .57735027f,  .57735027f); glVertex3f(x-r, y+r, z+r);
  glEnd();
  
  GLfloat zero[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, zero);
}

void
drawTube(double *opts, int on, int pm, double r) {
  if(on >= 2) {
    int n;
    double *pts;
    if(pm > 1) {
      n = on + (on - 1) * pm;
      pts = new double[n*3];
      interp(opts, pts, on, pm);
    } else {
      n = on;
      pts = opts;
    }
    double *nrms = new double[n*3];
    int i, j, k;

#if 0
    for(i=0; i<n; i++) {
      cout << i << '\t' << pts[i*3+0] << '\t' << pts[i*3+1] << '\t' << pts[i*3+2] << '\n';
    }
#endif

#if 1
    double p[3], pNext[3];
    double s[3], sNext[3], sPrev[3];
    double w[3], nP[3], normal[3], nrm[3];
    int ns = 10;
    
    {
      double c[3], q[3], f1, f2;
      double theta;
      bool largeRotation;
      for(i=0; i<n; i++) {
	if(i==0) {
	  for(j=0; j<3; j++) {
	    p[j] = pts[0*3+j];
	    pNext[j] = pts[1*3+j];
	    sPrev[j] = pNext[j] - p[j];
	    sNext[j] = sPrev[j];
	  }
	  if(normalize(sNext) == 0) {
	    abort();
	  }
	  if(n > 2) {
	    double ftmp[3];
	    for(j=0; j<3; j++) {
	      ftmp[j] = pts[2*3+j];
	      ftmp[j] -= pNext[j];
	    }
	    if(normalize(ftmp) == 0.0) {
	      abort();
	    }
	    cross(sNext, ftmp, normal);
	  }
	  if(n<=2 || normalize(normal) == 0.0) {
	    for(j=0; j<3; j++) {
	      if(sNext[j] != 0.0) {
		normal[(j+2)%3] = 0.0;
		normal[(j+1)%3] = 1.0;
		normal[j] = -sNext[(j+1)%3]/sNext[j];
		break;
	      }
	    }
	  }
	  normalize(normal);
	  for(j=0; j<3; j++) {
	    nrms[i*3+j] = normal[j];
	  }
	} else if(i==n-1) {
	  for(j=0; j<3; j++) {
	    nrms[i*3+j] = normal[j];
	  }
	} else {
	  for(j=0; j<3; j++) {
	    p[j] = pNext[j];
	    pNext[j] = pts[(i+1)*3+j];
	    sPrev[j] = sNext[j];
	    sNext[j] = pNext[j] - p[j];
	  }
	  if(normalize(sNext) == 0.0) {
	    abort();
	  }
	  cross(sPrev, normal, w);
	  if(normalize(w) == 0.0) {
	    abort();
	  }
	  if(dot(sPrev, sNext) < 0.0) {
	    largeRotation = true;
	  } else {
	    largeRotation = false;
	  }
	  cross(sNext, sPrev, q);
	  if((theta = asin(normalize(q))) == 0.0) {
	    for(j=0; j<3; j++) {
	      nrms[i*3+j] = normal[j];
	    }
	    continue;
	  }
	  if(largeRotation) {
	    if(theta > 0.0) {
	      theta = PI - theta;
	    } else {
	      theta = -PI - theta;
	    }
	  }
	  for(j=0; j<3; j++) {
	    c[j] = sNext[j] + sPrev[j];
	  }
	  normalize(c);
	  f1 = dot(q, normal);
	  f2 = 1.0 - f1*f1;
	  if(f2 > 0.0) {
	    f2 = sqrt(1.0 - f1*f1);
	  } else {
	    f2 = 0.0;
	  }
	  cross(c, q, w);
	  cross(sPrev, q, c);
	  if(dot(normal, c) * dot(w, c) < 0.0) {
	    f2 = -1.0 * f2;
	  }
	  for(j=0; j<3; j++) {
	    normal[j] = f1*q[j] + f2*w[j];
	    nrms[i*3+j] = normal[j];
	  }
	}
      }
    }

    double theta = 2.0*PI/ns;
    
    int nn = n * ns;
    double *npts = new double[nn*3];
    double *nnrms = new double[nn*3];
    
    for(i=0; i<n; i++) {
      if(i==0) {
	for(j=0; j<3; j++) {
	  p[j] = pts[0+j];
	  pNext[j] = pts[1*3+j];
	  sNext[j] = pNext[j] - p[j];
	  sPrev[j] = sNext[j];
	}
      } else if(i==n-1) {
	for(j=0; j<3; j++) {
	  sPrev[j] = sNext[j];
	  p[j] = pNext[j];
	}
      } else {
	for(j=0; j<3; j++) {
	  p[j] = pNext[j];
	  pNext[j] = pts[(i+1)*3+j];
	  sPrev[j] = sNext[j];
	  sNext[j] = pNext[j] - p[j];
	}
      }
      for(j=0; j<3; j++) {
	nrm[j] = nrms[i*3+j];
      }
      if(normalize(sNext) == 0) {
	abort();
      }
      for(j=0; j<3; j++) {
	s[j] = (sPrev[j] + sNext[j]) / 2.0;
      }
      if(normalize(s) == 0.0) {
	cross(sPrev, nrm, s);
      }
      cross(s, nrm, w);
      if(normalize(w) == 0.0) {
	abort();
      }
      cross(w, s, nP);
      normalize(nP);
      for(k=0; k<ns; k++) {
	for(j=0; j<3; j++) {
	  normal[j] = w[j] * cos(k*theta) + nP[j] * sin(k*theta);
	  s[j] = p[j] + r * normal[j];
	  npts[(i*ns+k)*3+j] = s[j];
	  nnrms[(i*ns+k)*3+j] = normal[j];
	}
      }
    }
    
    for(k=0; k<ns; k++) {
      glBegin(GL_QUAD_STRIP);
      int i1 = (k+1) % ns;
      for(j=0; j<n; j++) {
	int i2 = j * ns;
	glNormal3f(nnrms[(i2+i1)*3  ], nnrms[(i2+i1)*3+1], nnrms[(i2+i1)*3+2]);
	glVertex3f(npts[(i2+i1)*3  ], npts[(i2+i1)*3+1], npts[(i2+i1)*3+2]);
	
	glNormal3f(nnrms[(i2+k)*3  ], nnrms[(i2+k)*3+1], nnrms[(i2+k)*3+2]);
	glVertex3f(npts[(i2+k)*3  ], npts[(i2+k)*3+1], npts[(i2+k)*3+2]);
      }
      glEnd();
    }
#if 0
    for(k=0; k<ns; k++) {
      glBegin(GL_LINES);
      int i1 = (k+1) % ns;
      for(j=0; j<n; j++) {
	int i2 = j * ns;
	glVertex3f(npts[(i2+i1)*3], npts[(i2+i1)*3+1], npts[(i2+i1)*3+2]);
	glVertex3f(npts[(i2+i1)*3]+nnrms[(i2+i1)*3], npts[(i2+i1)*3+1]+nnrms[(i2+i1)*3+1], npts[(i2+i1)*3+2]+nnrms[(i2+i1)*3+2]);
      }
      glEnd();
    }
#endif

    if(pts != opts) {
      delete[] pts;
    }
    delete[] nrms;
    delete[] npts;
    delete[] nnrms;
#endif
  }
}
  
void
HSVtoRGB(float h, float s, float v, float rgb[4]) {
  rgb[3] = 1.0;
  if(s == 0.0) {
    rgb[0] = v;
    rgb[1] = v;
    rgb[2] = v;
  } else {
    if(h == 360) h = 0;
    h /= 60;
    int i = (int)h;
    float f = h - i;
    float p = v * (1 - s);
    float q = v * (1 - (s * f));
    float t = v * (1 - (s * (1 - f)));
    switch(i) {
    case 0: rgb[0] = v; rgb[1] = t; rgb[2] = p; break;
    case 1: rgb[0] = q; rgb[1] = v; rgb[2] = p; break;
    case 2: rgb[0] = p; rgb[1] = v; rgb[2] = t; break;
    case 3: rgb[0] = p; rgb[1] = q; rgb[2] = v; break;
    case 4: rgb[0] = t; rgb[1] = p; rgb[2] = v; break;
    case 5: rgb[0] = v; rgb[1] = p; rgb[2] = q; break;
    default: abort();
    }
  }
}


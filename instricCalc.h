
#ifndef _INTRISIC_CALC_
#define _INTRISIC_CALC_

static double norm( double a, double b, double c )
{
    return( sqrt( a*a + b*b + c*c ) );
}

static double dot( double a1, double a2, double a3,
		   double b1, double b2, double b3 )
{
    return( a1 * b1 + a2 * b2 + a3 * b3 );
}

//matriz de parâmetros intrínsecos padrão para câmeras que capturam na resolução de 640 x 480 (usada pelo ARToolKit)
typedef struct
{
    int  xsize;
    int  ysize;
    double mat[3][4];
    double dist_factor[4];
} Intrinsic3x4;

static int paramDecompMat(double source[3][4], double cpara[3][4], double trans[3][4])
{
  int       r, c;
  double    Cpara[3][4];
  double    rem1, rem2, rem3;

  if( source[2][3] >= 0 ) {
    for( r = 0; r < 3; r++ ){
      for( c = 0; c < 4; c++ ){
        Cpara[r][c] = source[r][c];
      }
    }
  }
  else {
    for( r = 0; r < 3; r++ ){
      for( c = 0; c < 4; c++ ){
        Cpara[r][c] = -(source[r][c]);
      }
    }
  }

  for( r = 0; r < 3; r++ ){
    for( c = 0; c < 4; c++ ){
      cpara[r][c] = 0.0;
    }
  }
  cpara[2][2] = norm( Cpara[2][0], Cpara[2][1], Cpara[2][2] );
  trans[2][0] = Cpara[2][0] / cpara[2][2];
  trans[2][1] = Cpara[2][1] / cpara[2][2];
  trans[2][2] = Cpara[2][2] / cpara[2][2];
  trans[2][3] = Cpara[2][3] / cpara[2][2];

  cpara[1][2] = dot( trans[2][0], trans[2][1], trans[2][2],
                     Cpara[1][0], Cpara[1][1], Cpara[1][2] );
  rem1 = Cpara[1][0] - cpara[1][2] * trans[2][0];
  rem2 = Cpara[1][1] - cpara[1][2] * trans[2][1];
  rem3 = Cpara[1][2] - cpara[1][2] * trans[2][2];
  cpara[1][1] = norm( rem1, rem2, rem3 );
  trans[1][0] = rem1 / cpara[1][1];
  trans[1][1] = rem2 / cpara[1][1];
  trans[1][2] = rem3 / cpara[1][1];

  cpara[0][2] = dot( trans[2][0], trans[2][1], trans[2][2],
                     Cpara[0][0], Cpara[0][1], Cpara[0][2] );
  cpara[0][1] = dot( trans[1][0], trans[1][1], trans[1][2],
                     Cpara[0][0], Cpara[0][1], Cpara[0][2] );
  rem1 = Cpara[0][0] - cpara[0][1]*trans[1][0] - cpara[0][2]*trans[2][0];
  rem2 = Cpara[0][1] - cpara[0][1]*trans[1][1] - cpara[0][2]*trans[2][1];
  rem3 = Cpara[0][2] - cpara[0][1]*trans[1][2] - cpara[0][2]*trans[2][2];
  cpara[0][0] = norm( rem1, rem2, rem3 );
  trans[0][0] = rem1 / cpara[0][0];
  trans[0][1] = rem2 / cpara[0][0];
  trans[0][2] = rem3 / cpara[0][0];

  trans[1][3] = (Cpara[1][3] - cpara[1][2]*trans[2][3]) / cpara[1][1];
  trans[0][3] = (Cpara[0][3] - cpara[0][1]*trans[1][3]
                             - cpara[0][2]*trans[2][3]) / cpara[0][0];

  for( r = 0; r < 3; r++ ){
    for( c = 0; c < 3; c++ ){
              cpara[r][c] /= cpara[2][2];
    }
  }

  return 0;
}

static double* findGLIntrinsic(Intrinsic3x4 intrinsic3x4, double gnear = 1, double gfar = 1000)
{
  double *m = new double[16];

  double   icpara[3][4];
  double   trans[3][4];
  double   p[3][3], q[4][4];
  int      i, j;

  if(paramDecompMat(intrinsic3x4.mat, icpara, trans) < 0)
  {
    printf("findGLIntrinsic: Parameter Error!!\n");
    exit(0);
  }

  for( i = 0; i < 3; i++ ) {
    for( j = 0; j < 3; j++ ) {
      p[i][j] = icpara[i][j] / icpara[2][2];
    }
  }
  q[0][0] = (2.0 * p[0][0] / intrinsic3x4.xsize);
  q[0][1] = (2.0 * p[0][1] / intrinsic3x4.xsize);
  q[0][2] = ((2.0 * p[0][2] / intrinsic3x4.xsize)  - 1.0);
  q[0][3] = 0.0;

  q[1][0] = 0.0;
  q[1][1] = -(2.0 * p[1][1] / intrinsic3x4.ysize); //originally there was no - (minus)
  q[1][2] = ((2.0 * p[1][2] / intrinsic3x4.ysize) - 1.0);
  q[1][3] = 0.0;

  q[2][0] = 0.0;
  q[2][1] = 0.0;
  q[2][2] = (gfar + gnear)/(gfar - gnear);
  //q[2][3] = -q[2][2] - 1.0;
  q[2][3] = -2.0 * gfar * gnear / (gfar - gnear); //this last is the original line from artoolkit but does not work

  q[3][0] = 0.0;
  q[3][1] = 0.0;
  q[3][2] = 1.0;
  q[3][3] = 0.0;

  for( i = 0; i < 4; i++ ) {
    for( j = 0; j < 3; j++ ) {
      m[i+j*4] = q[i][0] * trans[0][j]
               + q[i][1] * trans[1][j]
               + q[i][2] * trans[2][j];
    }
    m[i+3*4] = q[i][0] * trans[0][3]
             + q[i][1] * trans[1][3]
             + q[i][2] * trans[2][3]
             + q[i][3];
  }

  return m;
}

static Intrinsic3x4 findIntrinsic(int width = 640, int height = 480)
{
  Intrinsic3x4 intrinsic3x4;

  //first initialize the default matrix
  intrinsic3x4.xsize = 640;
  intrinsic3x4.ysize = 480;

  intrinsic3x4.dist_factor[0] = 318.5;
  intrinsic3x4.dist_factor[1] = 263.5;
  intrinsic3x4.dist_factor[2] = 26.2;
  intrinsic3x4.dist_factor[3] = 1.012757;

  intrinsic3x4.mat[0][0] = 700.95147;
  intrinsic3x4.mat[0][1] = 0;
  intrinsic3x4.mat[0][2] = 316.50000;
  intrinsic3x4.mat[0][3] = 0;

  intrinsic3x4.mat[1][0] = 0;
  intrinsic3x4.mat[1][1] = 726.094182;
  intrinsic3x4.mat[1][2] = 241.50000;
  intrinsic3x4.mat[1][3] = 0;

  intrinsic3x4.mat[2][0] = 0;
  intrinsic3x4.mat[2][1] = 0;
  intrinsic3x4.mat[2][2] = 1.00000;
  intrinsic3x4.mat[2][3] = 0;

  //now update the default matrix according to the widht and height values
  double  xscale;
  double  yscale;
  int     i;

  xscale = (double)width  / (double)(intrinsic3x4.xsize);
  yscale = (double)height / (double)(intrinsic3x4.ysize);

  intrinsic3x4.xsize = width;
  intrinsic3x4.ysize = height;

  for( i = 0; i < 4; i++ ) {
    intrinsic3x4.mat[0][i] = intrinsic3x4.mat[0][i] * xscale;
    intrinsic3x4.mat[1][i] = intrinsic3x4.mat[1][i] * yscale;
    intrinsic3x4.mat[2][i] = intrinsic3x4.mat[2][i];
  }

  intrinsic3x4.dist_factor[0] = intrinsic3x4.dist_factor[0] * xscale; //x center of distortion
  intrinsic3x4.dist_factor[1] = intrinsic3x4.dist_factor[1] * yscale; //y center of distortion
  intrinsic3x4.dist_factor[2] = intrinsic3x4.dist_factor[2] / (xscale*yscale); //distortion factor
  intrinsic3x4.dist_factor[3] = intrinsic3x4.dist_factor[3]; //scale factor

  return intrinsic3x4;
}

#endif

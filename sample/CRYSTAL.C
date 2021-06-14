/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                   Fractal "Crystal" Growth Generator                    บ
บ                                                                         บ
บ           Crystal.C = Monte-Carlo simulation of crystal growth          บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
บ Requires: BkDefs.H, BkGlobs.H, BkMath.H, BkMath.C, BkGraph.H, BkGraph.C บ
บ                                                                         บ
บ                       C conversion by Addison Rose                      บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

   A "crystal is grown by moving particles into a region containing a "seed"
crystal.  The particles move about in a random fashion, and if a particle
touches the seed, it will stick.  The resultant shapes are very similar to
natural amorphous crystal growth.
*/

#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "BkDefs.H"
#include "BkGlobs.H"
#include "BkMath.H"
#include "BkGraph.H"

#define  ScatterProbability   0.25

Palette_Register PalArray;

main()
{
  short Success, Failure;
  int xval, yval, dx, dy, r, s;
  double Rad, Ph, PrtRad, TwoPi;
  unsigned char ScatProb;


  TwoPi = 2.0 * Pi;
  Rad = 5.0;

  Init_Graphics(1);

  Init_Palette(PalArray);
  Set_Palette(PalArray);

  CentreX = XRes / 2;
  CentreY = YRes / 2;

  randomize();

  // probability of scattering per interation
  ScatProb = Round(1.0 / ScatterProbability);

  // Put in the "seed"
  Plot(CentreX, CentreY, 63);

  while (!kbhit()) {

      // Pick a random angle to start at,
      //   and put the point on a circle of Radius Rad
      Ph = random(32768) * TwoPi;
      xval = CentreX + Round(Rad * CosD(Ph));
      yval = CentreY + Round(Rad * SinD(Ph));

      // Pick a random direction
      dx = (random(32767) % 3) - 1;
      dy = (random(32767) % 3) - 1;

      Plot(xval, yval, 63);                     // Plot the point

      Success = 0;
      Failure = 0;

      while (!Success && !Failure) {

	Plot(xval, yval, 0);                     // Clear the point

	r = random(32768) % ScatProb;            // Has the particle scattered?

	if (r) {
	  dx = (random(32767) % 3) - 1;          // pick a new direction
	  dy = (random(32767) % 3) - 1;          // at random
	}

	xval = xval + dx;                        // Move the particle
	yval = yval + dy;

	Plot(xval, yval, 63);                   // Plot the new point

	if ((Get_Pixel(xval+1, yval  ) != 0) ||  // Is the particle touching any
	    (Get_Pixel(xval+1, yval+1) != 0) ||  // point that is lit up?
	    (Get_Pixel(xval,   yval+1) != 0) ||
	    (Get_Pixel(xval-1, yval+1) != 0) ||
	    (Get_Pixel(xval-1, yval  ) != 0) ||
	    (Get_Pixel(xval-1, yval-1) != 0) ||
	    (Get_Pixel(xval,   yval-1) != 0) ||
	    (Get_Pixel(xval+1, yval-1) != 0))
	  Success = 1;

	// great speed can be achieved by setting up a 2-D array and testing
	//   the points in the array, as opposed to Get_Pixel from the screen

	// Is the particle out of bounds?

	if ( (xval < 0) || (xval > XRes) || (yval < 0) || (yval > YRes) )
	  Failure = 1;

	// To speed things up, throw the particle out if it gets
	//   too far away from the object

	else {
	  PrtRad = IntSqrt(Sqr(xval - CentreX) + Sqr(yval - CentreY));
	  if (PrtRad > Rad + 15.0) Failure = 1;
	}

      }

      if (Failure) Plot(xval, yval, 0);          // Throw out the particle
      else {
	// Check to see If the radius is bigger than the object, and adjust
	PrtRad = IntSqrt(Sqr(xval - CentreX) + Sqr(yval - CentreY));
	if (PrtRad > Rad - 8.0) Rad = PrtRad + 8.0;
	if (Rad > CentreX) Rad = CentreX;
	if (Rad > CentreY) Rad = CentreY;
      }

  }

  Exit_Graphics();
  return 0;
}

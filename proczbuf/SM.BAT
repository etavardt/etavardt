cls
rem Quiet - Rational Systems Credit
set DOS4G=quiet
rem
rem Polygon Smooth
rem      (prepares polygons for a Phong Shader)
rem This version by
rem      Christopher D. Watkins and Stephen B. Coy
rem Based on the public domain Sandpaper III by
rem      Mike Schoenborn
type %1.raw | smooth > %1.bo

rem Example of Average Filter
rem
rem Enter :  FILT infile outfile
rem
filter -f average.flt -i %1.img -o %2.img
img2tga %2

#
# R scripts to study the geometry. 
#
#mD <- read.table("/scratch/lbne/lebrun/steppingPropCO103_H1Xby2.txt", h=T);
mD <- read.table("/scratch/lbne/lebrun/steppingPropCO103_H2Xby2.txt", h=T);
rr <- sqrt(mD$x*mD$x + mD$y*mD$y);
ro <- sqrt(mD$xo*mD$xo + mD$yo*mD$yo);
plot(mD$z, rr, type='p', pch='.', xlim=c(-260., -100.), ylim=c(25., 50.));
lines(mD$zo, ro, type='p', pch='.', col="blue");
#

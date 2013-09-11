#
# R scripts to study the geometry. 
#
#mD <- read.table("/scratch/lbne/lebrun/steppingPropa103_b1.txt", h=T);
mD <- read.table("/scratch/lbne/lebrun/steppingPropa103_by2.txt", h=T);
#mD <- read.table("/scratch/lbne/lebrun/steppingPropa101_l20.txt", h=T);
rr <- sqrt(mD$x*mD$x + mD$y*mD$y);
ro <- sqrt(mD$xo*mD$xo + mD$yo*mD$yo);
plot(mD$z, rr, type='p', pch='.', xlim=c(-400., 1600.), ylim=c(0., 50.));
lines(mD$zo, ro, type='p', pch='.', col="blue");
#
# Assume misalignments in X forthe Horn1. Target is perfectly aligned. 
#
mDBerylPo <- subset(mD, (mD$matPost == "Beryllium")); 
mDBerylPe <- subset(mD, (mD$matPre == "Beryllium")); 
#  Only true for /LBNE/det/TargetLengthIntoHorn 61 cm
mDBerylPoCap <- subset(mDBerylPo, (abs(mDBerylPo$z - 300) < 50.)); 
mDBerylPeCap <- subset(mDBerylPe, (abs(mDBerylPe$z - 300) < 50.)); 
rrBerylPoCap <- sqrt(mDBerylPoCap$x*mDBerylPoCap$x + mDBerylPoCap$y*mDBerylPoCap$y);
rrBerylPeCap <- sqrt(mDBerylPeCap$x*mDBerylPeCap$x + mDBerylPeCap$y*mDBerylPeCap$y);
#
mDNeck <- subset(mD, ((mD$z > 500.) & (mD$z < 535.)));
rrNeck <- sqrt(mDNeck$x*mDNeck$x + mDNeck$y*mDNeck$y);
#
plot(mDNeck$x, mDNeck$y, type='p', pch='*', col="blue");
lines(mDNeck$xo, mDNeck$yo, type='p', pch='*', col="blue");
#
lines(mDBerylPoCap$x, mDBerylPoCap$y, type='p', pch='.', col="red");
lines(mDBerylPoCap$xo, mDBerylPoCap$yo, type='p', pch='.', col="red");
lines(mDBerylPeCap$x, mDBerylPeCap$y, type='p', pch='.', col="red");
lines(mDBerylPeCap$xo, mDBerylPeCap$yo, type='p', pch='.', col="red");

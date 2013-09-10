#
# 
#mDM <- read.table("./FieldTrajectories_Horn1_1_MisA.txt", h=T);
mDM <- read.table("./FieldTrajectories_Horn1_1_MisAy.txt", h=T);
mD <- read.table("./FieldTrajectories_Horn1_1_NoMisA.txt", h=T);
#
# Valid only for LengthIntoHorn = 61 cm. 
#
mDNeck <- subset(mD, ((mD$z > 500.) & (mD$z < 540)));
mDMNeck <- subset(mDM, ((mDM$z > 500.) & (mDM$z < 540)));
rrNeck <- sqrt(mDNeck$x*mDNeck$x + mDNeck$y*mDNeck$y);
rrMNeck <- sqrt(mDMNeck$x*mDMNeck$x + mDMNeck$y*mDMNeck$y);
#
bPhiNeck <- sqrt(mDNeck$bx*mDNeck$bx + mDNeck$by*mDNeck$by);
bPhiMNeck <- sqrt(mDMNeck$bx*mDMNeck$bx + mDMNeck$by*mDMNeck$by);
# The field ought to peak for high value of X, 
# because the neck has been displace twowards positive x
#plot(mDNeck$x, mDNeck$by, type='p', pch='*', xlim=c(-20., 20.))
#lines(mDMNeck$x, mDMNeck$by, type='p', pch=18, col="blue")
# For y 
plot(mDNeck$y, mDNeck$bx, type='p', pch='*', xlim=c(-20., 20.))
lines(mDMNeck$y, mDMNeck$bx, type='p', pch=18, col="blue")
# 

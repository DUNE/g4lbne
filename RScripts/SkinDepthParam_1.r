#
# R scripts to study skin depth effects. Variable SkinDepthCorrInnerRad 
#
mDp0 <- read.table("../FieldMapHorn1Default.txt", h=T);
mDp25 <- read.table("../FieldMapHorn1SKDepthp25.txt", h=T);
mDp8 <- read.table("../FieldMapHorn1SKDepthp8.txt", h=T);
mDNp8 <- subset(mDp8, (abs(mDp8$z - 825.) < 15.))
mDNp25 <- subset(mDp25, (abs(mDp25$z - 825.) < 15.))
mDNp0 <- subset(mDp0, (abs(mDp0$z - 825.) < 15.))
#
aPdf <- pdf("./plotsDec13/SkinDepthFieldMap_Horn1Neck_1.pdf", width=7., height=5., family="serif", useDingbats=F);
plot(mDNp0$r, mDNp0$bphi, type='b', xlim=c(5., 25.), lty=1, pch=15, col="black", 
     xlab=" R [mm]", ylab=expression(paste(B[phi], "  Tesla")));
lines(mDNp25$r, mDNp25$bphi, type='b', lty=1, pch=16, col="blue");
lines(mDNp8$r, mDNp8$bphi, type='b', lty=1, pch=16, col="red");
legend(x="topleft", ncol=1, c("Default", "Skd=0.25", "Skd=0.8"), lty=c(1,1,1), pch=c(15,16,17), col=c("black", "blue", "red"));
dev.off();
#

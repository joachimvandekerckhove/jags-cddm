load dic
load cddm
model in "cddm.bug"
data in "cddm.data"
compile, nchains(1)
parameters in "cddm.init"
initialize
update 5
monitor set drift, thin(1)
monitor set bound, thin(1)
monitor set theta0, thin(1)
monitor set ter0, thin(1)
monitor deviance
update 5
coda *, stem('samples_1_')


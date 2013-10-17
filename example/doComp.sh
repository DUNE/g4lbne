version1=$1
version2=$2
macro1=$3
macro2=$4
current1=$5
current2=$6
plist1=$7
plist2=$8
ndloc1=$9
ndloc2=${10}
fdloc1=${11}
fdloc2=${12}
name1=${13}
name2=${14}



root -q -b eventRateComparison.C\(\"oscillated\",\"$macro1\",\"$macro2\",\"$version1\",\"$version2\",\"${current1}\",\"${current2}\",\"${ndloc1}\",\"${ndloc2}\",\"${plist1}\",\"${plist2}\",\"${name1}\",\"${name2}\"\);
root -q -b eventRateComparison.C\(\"oscillated\",\"$macro1\",\"$macro2\",\"$version1\",\"$version2\",\"-${current1}\",\"-${current2}\",\"${ndloc1}\",\"${ndloc2}\",\"${plist1}\",\"${plist2}\",\"${name1}\",\"${name2}\"\);

root -q -b eventRateComparison.C\(\"oscillated\",\"$macro1\",\"$macro2\",\"$version1\",\"$version2\",\"${current1}\",\"${current2}\",\"${fdloc1}\",\"${fdloc2}\",\"${plist1}\",\"${plist2}\",\"${name1}\",\"${name2}\"\);

root -q -b eventRateComparison.C\(\"oscillated\",\"$macro1\",\"$macro2\",\"$version1\",\"$version2\",\"-${current1}\",\"-${current2}\",\"${fdloc1}\",\"${fdloc2}\",\"${plist1}\",\"${plist2}\",\"${name1}\",\"${name2}\"\);

root -q -b eventRateComparison.C\(\"unoscillated\",\"$macro1\",\"$macro2\",\"$version1\",\"$version2\",\"${current1}\",\"${current2}\",\"${ndloc1}\",\"${ndloc2}\",\"${plist1}\",\"${plist2}\",\"${name1}\",\"${name2}\"\);
root -q -b eventRateComparison.C\(\"unoscillated\",\"$macro1\",\"$macro2\",\"$version1\",\"$version2\",\"-${current1}\",\"-${current2}\",\"${ndloc1}\",\"${ndloc2}\",\"${plist1}\",\"${plist2}\",\"${name1}\",\"${name2}\"\);

root -q -b eventRateComparison.C\(\"unoscillated\",\"$macro1\",\"$macro2\",\"$version1\",\"$version2\",\"${current1}\",\"${current2}\",\"${fdloc1}\",\"${fdloc2}\",\"${plist1}\",\"${plist2}\",\"${name1}\",\"${name2}\"\);
root -q -b eventRateComparison.C\(\"unoscillated\",\"$macro1\",\"$macro2\",\"$version1\",\"$version2\",\"-${current1}\",\"-${current2}\",\"${fdloc1}\",\"${fdloc2}\",\"${plist1}\",\"${plist2}\",\"${name1}\",\"${name2}\"\);


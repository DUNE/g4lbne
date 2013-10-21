user=$1
version=$2
macro=$3
current=$4
plist=$5
ndloc=$6
fdloc=$7
name=$8

root -q -b eventRateComparison.C\(\"oscillated\",\"$macro\",\"$user\",\"$version\",\"${current}\",\"${ndloc}\",\"${fdloc}\",\"${plist}\",\"${name}\"\);
root -q -b eventRateComparison.C\(\"unoscillated\",\"$macro\",\"$user\",\"$version\",\"${current}\",\"${ndloc}\",\"${fdloc}\",\"${plist}\",\"${name}\"\);


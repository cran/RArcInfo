\name{RArcInfo}
\alias{RArcInfo}


\title{RArcInfo}
\description{
This package allows the user to import into R binary coverages in format
Arc/Info V 7.x. These coverages represent geographical data in several forms:
points, lines, polygons, point labels, etc. 

 RArcInfo uses the library AVCE00, written by Daniel Morissette, to whom I would
thank fo his marvelous work. But RArcInfo is much more than a wrapper of this library, because
it provides functions to plot the data and draw maps.

 Since the geographical data are separated into several files, RArcInfo provides a different 
function to read each file. These functions are called get.XXXdata, where XXX is the
file name to open (usually the extension is 'adf'):

\item get.arcdata

 ARC files contain arcs definition and their vertices.

\item get.bnddata

 BND files contain coordinates for the boundary of the data.

\item get.cntdata

 CNT files contain polygon centroid information.

\item get.labdata

 LAB files contain label point records.

\item get.paldata

 PAL files contain the polygon definitions.

\item get.toldata

 TOL files contain the tolerance values that were used when processing the polygon coverage.

\item get.txtdata

 TXT files contain annotations (or labels) about the data.


 Besides these files, binary coverage store several tables containing additional information
(like name of the city, population, etc.). To get this data, RArcInfo provides the
next functions:


\item get.tablenames 

 Gets all the table names and the coverage each table belongs to.

\item get.tablefields

 Gets the names of the fields for a given table.

\item get.tabledata

 Gets the data stored in the table.


In order to plot the data, RArcInfo has several functions:

\item plotarc

 Plots all the arcs.

\item plotpal

 Plots all the polygons.

\item plotpoly

 Like plotpal, but it allows to select the polygons we want to plot, colour and 
other stuff. This is useful to plot maps according the value of some covariate.
  

Finally, to get all the names of the coverages, the user can call 'get.namesofcoverages'.

}


\seealso{get.arcdata, get.bnddata, get.cntdata, get.labdata, get.paldata, get.toldata, 
get.txtdata, get.tablenames, get.tablefields, get.tabledata, get.namesofcoverages
}


\references{
More information about this kind of data can be found at
\url{http://pages.infinit.net/danmo/e00/docs/v7_bin_cover.html}.
}

\keyword{file}

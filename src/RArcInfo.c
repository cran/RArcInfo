#include<stdio.h>
#include<dirent.h>
#include"RArcInfo.h"
#include"avc.h"

#include<R.h>
#include<Rinternals.h>
#include<Rdefines.h>

int _AVCBinReadNextArcDir(AVCRawBinFile *psInfo, AVCTableDef *psArcDir);
AVCBinFile *_AVCBinReadOpenTable(const char *pszInfoPath, const char *pszTableName);


//It just reads the file and directory names under "directory"
SEXP get_names_of_coverages(SEXP directory)
{
	int i,n;
	struct dirent **namelist;
	SEXP lnames;
	
	if(!isString(directory))
		error("directory is not a string");

	
	n = scandir(CHAR(STRING_ELT(directory,0)), &namelist, 0, alphasort);
	printf("Number of directories and files: %d\n",n);

//I should test whether the names belong to common files or directories

	if(n<=2)
		return R_NilValue;
	//else

	PROTECT(lnames=NEW_STRING(n-2));

	for(i=2;i<n;i++)
	{
		printf("File[%d]=%s\n",i, namelist[i]->d_name);
		SET_STRING_ELT(lnames,i-2,COPY_TO_USER_STRING(namelist[i]->d_name));
	}
	UNPROTECT(1);
	return lnames;
}


//It returns the table names and something more:
//- Arc file
//- Number of fields
//- Register Size
//- Number of registers
//- External/Internal Table Identifier

SEXP get_table_names(SEXP directory)
{
	SEXP tables, aux;
	AVCRawBinFile *arcfile;
	AVCTableDef tabledefaux;
	char arcdir[256], *dirname;
	int i,n;

	dirname=CHAR(STRING_ELT(directory,0));
	strcpy(arcdir,dirname);
	if(dirname[strlen(dirname)-1]=='/')
		strcat(arcdir,"arc.dir");
	else
		strcat(arcdir,"/arc.dir");

	if(!(arcfile=AVCRawBinOpen(arcdir,"r")))
	{
		error("Error opening arc.dir");
	}

	n=0;
	while(!AVCRawBinEOF(arcfile))
	{
		if(!_AVCBinReadNextArcDir(arcfile, &tabledefaux))
			n++;
	}

	AVCRawBinFSeek(arcfile, 0,SEEK_SET);

	PROTECT(tables=NEW_LIST(n));
	i=0;
	while(!AVCRawBinEOF(arcfile))
	{
		if(_AVCBinReadNextArcDir(arcfile, &tabledefaux))
			break;

		SET_VECTOR_ELT(tables,i,NEW_LIST(6));
		aux=VECTOR_ELT(tables,i);

		//Table name
		SET_VECTOR_ELT(aux,0,NEW_STRING(1));
		SET_STRING_ELT(aux,0, COPY_TO_USER_STRING(tabledefaux.szTableName));

		SET_VECTOR_ELT(aux,1,NEW_STRING(1));
		SET_STRING_ELT(aux,1, COPY_TO_USER_STRING(tabledefaux.szInfoFile));
		
		SET_VECTOR_ELT(aux,2,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,2,ScalarInteger(tabledefaux.numFields));

		SET_VECTOR_ELT(aux,3,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,3,ScalarInteger(tabledefaux.nRecSize));

		SET_VECTOR_ELT(aux,4,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,4,ScalarInteger(tabledefaux.numRecords));

		SET_VECTOR_ELT(aux,5,NEW_LOGICAL(1));
		if(!strcmp(tabledefaux.szExternal,"XX"))
			SET_VECTOR_ELT(aux,5, ScalarLogical(1));
		else
			SET_VECTOR_ELT(aux,5, ScalarLogical(0));
		
		i++;
	}
	UNPROTECT(1);
	return tables;
}


//It returns the fields of the table (name and type)
SEXP get_table_fields(SEXP info_dir, SEXP table_name)
{
	int i;
	char dir[256];
	SEXP data, aux;
	AVCBinFile *tablefile;
	AVCTableDef *tabledef;
	AVCFieldInfo *fields;

	strcpy(dir, CHAR(STRING_ELT(info_dir,0)));
	i=strlen(dir);
	if(dir[i-1]!='/')
	{
		dir[i]='/';
		dir[i+1]='\0';
	}

tablefile=_AVCBinReadOpenTable(dir, CHAR(STRING_ELT(table_name,0)) );

	if(!tablefile)
		error("The path to the info directory is invalid or the table doesn't exist");
	
	tabledef=(tablefile->hdr).psTableDef;
	fields=tabledef->pasFieldDef;

	PROTECT(data=NEW_LIST(tabledef->numFields));

	for(i=0;i<tabledef->numFields;i++)
	{
		SET_VECTOR_ELT(data,i, NEW_LIST(2));
		aux=VECTOR_ELT(data,i);	


		SET_VECTOR_ELT(aux,0, NEW_STRING(1));
		SET_STRING_ELT(aux,0, COPY_TO_USER_STRING(fields[i].szName));

		SET_VECTOR_ELT(aux,1, NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,1, ScalarInteger(fields[i].nType1));
	}

	UNPROTECT(1);
	return data;
}

//It imports the data from an arc file
SEXP get_arc_data(SEXP directory, SEXP coverage, SEXP filename) 
{
	int i,j,n;
	double *pdata;
	char pathtofile[257];
	AVCArc *reg;
	AVCBinFile *file;
	SEXP data,aux;


	strcpy(pathtofile, CHAR(STRING_ELT(directory,0)));

	if(pathtofile[strlen(pathtofile)-1]=='/')
                strcat(pathtofile,CHAR(STRING_ELT(coverage,0)));
        else
	{
                strcat(pathtofile,"/");
		strcat(pathtofile,CHAR(STRING_ELT(coverage,0)));
	}

	if(pathtofile[strlen(pathtofile)-1]!='/')
		strcat(pathtofile,"/");


	printf("%s\n",pathtofile);
	if(!(file=AVCBinReadOpen(pathtofile,CHAR(STRING_ELT(filename,0)), AVCFileARC)))
		error("Error opening file");

	n=0;

	while(AVCBinReadNextArc(file)){n++;}

	printf("%d\n",n);
	PROTECT(data=NEW_LIST(n));

	if(AVCBinReadRewind(file))
		error("Rewind");

	for(i=0;i<n;i++)
	{
		
		if(!(reg=(AVCArc*)AVCBinReadNextArc(file)))
			error("Error while reading register");

		SET_VECTOR_ELT(data,i,NEW_LIST(8));	
		aux=VECTOR_ELT(data,i);

		SET_VECTOR_ELT(aux,0,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,0,ScalarInteger(reg->nArcId));

		SET_VECTOR_ELT(aux,1,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,1,ScalarInteger(reg->nUserId));

		SET_VECTOR_ELT(aux,2,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,2,ScalarInteger(reg->nFNode));

		SET_VECTOR_ELT(aux,3,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,3,ScalarInteger(reg->nTNode));

		SET_VECTOR_ELT(aux,4,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,4,ScalarInteger(reg->nLPoly));

		SET_VECTOR_ELT(aux,5,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,5,ScalarInteger(reg->nRPoly));

		SET_VECTOR_ELT(aux,6,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,6,ScalarInteger(reg->numVertices));

		SET_VECTOR_ELT(aux,7,NEW_NUMERIC(2*(reg->numVertices)));


		for(j=0;j<reg->numVertices;j++)
		{
			pdata=REAL(VECTOR_ELT(aux,7));

			pdata[2*j]=reg->pasVertices[j].x;
			pdata[2*j+1]=reg->pasVertices[j].y;
		}

	}

	UNPROTECT(1);


	return data;
}

SEXP get_bnd_data(SEXP info_dir, SEXP tablename)
{
	int i;
	double *d;
	AVCBinFile *tablefile;
	AVCField *datafield;
	SEXP data;

if(!(tablefile=_AVCBinReadOpenTable( CHAR(STRING_ELT(info_dir,0)), CHAR(STRING_ELT(tablename,0)) )) )
	error("Couldn't open table");

	tablefile->eFileType=AVCFileTABLE;

	if(AVCBinReadRewind(tablefile))	
		error("Couldn't open table");

	if(tablefile->hdr.psTableDef->numFields!=4 && tablefile->hdr.psTableDef->numRecords!=1)
		error("The file is not in BND format");

	datafield=AVCBinReadNextTableRec(tablefile);

	PROTECT(data=NEW_NUMERIC(4));

	d=REAL(data);

// //Fix this: The data can be of type float too.
	for(i=0;i<4;i++)
		d[i]=datafield[i].dDouble;

	UNPROTECT(1);

	return data;
}



SEXP get_pal_data(SEXP directory, SEXP coverage, SEXP filename) 
{
	int i,j,n;
	int *idata;
	double *ddata;
	char pathtofile[257];
	AVCPal *reg;
	AVCBinFile *file;
	SEXP data,aux;


	strcpy(pathtofile, CHAR(STRING_ELT(directory,0)));

	if(pathtofile[strlen(pathtofile)-1]=='/')
                strcat(pathtofile,CHAR(STRING_ELT(coverage,0)));
        else
	{
                strcat(pathtofile,"/");
		strcat(pathtofile,CHAR(STRING_ELT(coverage,0)));
	}

	if(pathtofile[strlen(pathtofile)-1]!='/')
		strcat(pathtofile,"/");


	printf("%s\n",pathtofile);
	if(!(file=AVCBinReadOpen(pathtofile,CHAR(STRING_ELT(filename,0)), AVCFilePAL)))
		error("Error opening file");

	n=0;

	while(AVCBinReadNextPal(file)){n++;}

	printf("%d\n",n);
	PROTECT(data=NEW_LIST(n));

	if(AVCBinReadRewind(file))
		error("Rewind");

	for(i=0;i<n;i++)
	{
		
		if(!(reg=(AVCPal*)AVCBinReadNextPal(file)))
			error("Error while reading register");

		SET_VECTOR_ELT(data,i,NEW_LIST(5));	
		aux=VECTOR_ELT(data,i);

		SET_VECTOR_ELT(aux,0,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,0,ScalarInteger(reg->nPolyId));

		SET_VECTOR_ELT(aux,1,NEW_NUMERIC(2));
		ddata=REAL(VECTOR_ELT(aux,1));
		ddata[0]=reg->sMin.x;
		ddata[1]=reg->sMin.y;

		SET_VECTOR_ELT(aux,2,NEW_NUMERIC(2));
		ddata=REAL(VECTOR_ELT(aux,2));
		ddata[0]=reg->sMax.x;
		ddata[1]=reg->sMax.y;

		SET_VECTOR_ELT(aux,3,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,3,ScalarInteger(reg->numArcs));


		SET_VECTOR_ELT(aux,4,NEW_INTEGER(3*(reg->numArcs)));
		for(j=0;j<reg->numArcs;j++)
		{
			idata=INTEGER(VECTOR_ELT(aux,4));

			idata[3*j]=reg->pasArcs[j].nArcId;
			idata[3*j+1]=reg->pasArcs[j].nFNode;
			idata[3*j+2]=reg->pasArcs[j].nAdjPoly;
		}

	}

	UNPROTECT(1);

	return data;
}



SEXP get_lab_data(SEXP directory, SEXP coverage, SEXP filename) 
{
	int i,n;
	double *ddata;
	char pathtofile[257];
	AVCLab *reg;
	AVCBinFile *file;
	SEXP data,aux;


	strcpy(pathtofile, CHAR(STRING_ELT(directory,0)));

	if(pathtofile[strlen(pathtofile)-1]=='/')
                strcat(pathtofile,CHAR(STRING_ELT(coverage,0)));
        else
	{
                strcat(pathtofile,"/");
		strcat(pathtofile,CHAR(STRING_ELT(coverage,0)));
	}

	if(pathtofile[strlen(pathtofile)-1]!='/')
		strcat(pathtofile,"/");


	printf("%s\n",pathtofile);
	if(!(file=AVCBinReadOpen(pathtofile,CHAR(STRING_ELT(filename,0)), AVCFileLAB)))
		error("Error opening file");

	n=0;

	while(AVCBinReadNextLab(file)){n++;}

	printf("%d\n",n);
	PROTECT(data=NEW_LIST(n));

	if(AVCBinReadRewind(file))
		error("Rewind");

	for(i=0;i<n;i++)
	{
		
		if(!(reg=(AVCLab*)AVCBinReadNextLab(file)))
			error("Error while reading register");

		SET_VECTOR_ELT(data,i,NEW_LIST(3));	
		aux=VECTOR_ELT(data,i);

		SET_VECTOR_ELT(aux,0,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,0,ScalarInteger(reg->nValue));

		SET_VECTOR_ELT(aux,1,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,1,ScalarInteger(reg->nPolyId));

		SET_VECTOR_ELT(aux,2,NEW_NUMERIC(6));
		ddata=REAL(VECTOR_ELT(aux,2));

		ddata[0]=reg->sCoord1.x;
		ddata[1]=reg->sCoord1.y;
		ddata[2]=reg->sCoord2.x;
		ddata[3]=reg->sCoord2.y;
		ddata[4]=reg->sCoord3.x;
		ddata[5]=reg->sCoord3.y;
		

	}

	UNPROTECT(1);

	return data;
}


SEXP get_cnt_data(SEXP directory, SEXP coverage, SEXP filename) 
{
	int i,j,n, *idata;
	double *ddata;
	char pathtofile[257];
	AVCCnt *reg;
	AVCBinFile *file;
	SEXP data,aux;


	strcpy(pathtofile, CHAR(STRING_ELT(directory,0)));

	if(pathtofile[strlen(pathtofile)-1]=='/')
                strcat(pathtofile,CHAR(STRING_ELT(coverage,0)));
        else
	{
                strcat(pathtofile,"/");
		strcat(pathtofile,CHAR(STRING_ELT(coverage,0)));
	}

	if(pathtofile[strlen(pathtofile)-1]!='/')
		strcat(pathtofile,"/");


	printf("%s\n",pathtofile);
	if(!(file=AVCBinReadOpen(pathtofile,CHAR(STRING_ELT(filename,0)), AVCFileCNT)))
		error("Error opening file");

	n=0;

	while(AVCBinReadNextCnt(file)){n++;}

	printf("%d\n",n);
	PROTECT(data=NEW_LIST(n));

	if(AVCBinReadRewind(file))
		error("Rewind");

	for(i=0;i<n;i++)
	{
		
		if(!(reg=(AVCCnt*)AVCBinReadNextCnt(file)))
			error("Error while reading register");

		SET_VECTOR_ELT(data,i,NEW_LIST(4));	
		aux=VECTOR_ELT(data,i);

		SET_VECTOR_ELT(aux,0,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,0,ScalarInteger(reg->nPolyId));

		SET_VECTOR_ELT(aux,1,NEW_NUMERIC(2));
		ddata=REAL(VECTOR_ELT(aux,1));
		ddata[0]=reg->sCoord.x;
		ddata[1]=reg->sCoord.y;

		SET_VECTOR_ELT(aux,2,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,2,ScalarInteger(reg->numLabels));

		if(reg->numLabels >0)
		{
			SET_VECTOR_ELT(aux,3,NEW_INTEGER(reg->numLabels));
			idata=INTEGER(VECTOR_ELT(aux,3));

			for(j=0;j<reg->numLabels;j++)
			{
				printf("%d\n", reg->panLabelIds[j]);
				idata[j]=reg->panLabelIds[j];
			}
		}

	}

	UNPROTECT(1);

	return data;
}



SEXP get_tol_data(SEXP directory, SEXP coverage, SEXP filename) 
{
	int i,n;
	char pathtofile[257];
	AVCTol *reg;
	AVCBinFile *file;
	SEXP data,aux;


	strcpy(pathtofile, CHAR(STRING_ELT(directory,0)));

	if(pathtofile[strlen(pathtofile)-1]=='/')
                strcat(pathtofile,CHAR(STRING_ELT(coverage,0)));
        else
	{
                strcat(pathtofile,"/");
		strcat(pathtofile,CHAR(STRING_ELT(coverage,0)));
	}

	if(pathtofile[strlen(pathtofile)-1]!='/')
		strcat(pathtofile,"/");


	printf("%s\n",pathtofile);
	if(!(file=AVCBinReadOpen(pathtofile,CHAR(STRING_ELT(filename,0)), AVCFileCNT)))
		error("Error opening file");

	n=0;

	while(AVCBinReadNextTol(file)){n++;}

	printf("%d\n",n);
	PROTECT(data=NEW_LIST(n));

	if(AVCBinReadRewind(file))
		error("Rewind");

	for(i=0;i<n;i++)
	{
		
		if(!(reg=(AVCTol*)AVCBinReadNextTol(file)))
			error("Error while reading register");

		SET_VECTOR_ELT(data,i,NEW_LIST(3));	
		aux=VECTOR_ELT(data,i);

		SET_VECTOR_ELT(aux,0,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,0,ScalarInteger(reg->nIndex));

		SET_VECTOR_ELT(aux,1,NEW_INTEGER(1));
		SET_VECTOR_ELT(aux,1,ScalarInteger(reg->nFlag));

		SET_VECTOR_ELT(aux,2,NEW_NUMERIC(1));
		SET_VECTOR_ELT(aux,2,ScalarReal(reg->dValue));


	}

	UNPROTECT(1);

	return data;
}



SEXP get_table_data(SEXP infodir, SEXP coverage, SEXP tablename) 
{
	int i,j,n;
	char pathtoinfodir[257];
	AVCTableDef *tabledef;
	AVCField *reg;
	AVCBinFile *file;
	SEXP data,aux;

	strcpy(pathtoinfodir, CHAR(STRING_ELT(infodir,0)));

	if(pathtoinfodir[strlen(pathtoinfodir)-1]!='/')
		strcat(pathtoinfodir,"/");

	if(!(file=AVCBinReadOpen(pathtoinfodir,CHAR(STRING_ELT(tablename,0)),AVCFileTABLE)))
	{
		error("Couldn't open table file\n");
	}

	n=0;

	while(AVCBinReadNextTableRec(file)){n++;}

	AVCBinReadRewind(file);

	PROTECT(data=NEW_LIST(n));
	tabledef=(file->hdr).psTableDef;

	for(i=0;i<n;i++)
	{
		reg=AVCBinReadNextTableRec(file);

		SET_VECTOR_ELT(data,i,NEW_LIST(tabledef->numFields));
		aux=VECTOR_ELT(data,i);

		for(j=0;j<tabledef->numFields;j++)
		{
//			printf("%d %d %d\n",i,j,tabledef->pasFieldDef[j].nType1);
			switch(tabledef->pasFieldDef[j].nType1)
			{
				case 1: 
				case 2:
				SET_VECTOR_ELT(aux,j, NEW_STRING(1)); 
	SET_STRING_ELT(aux,j, COPY_TO_USER_STRING(reg[j].pszStr)); 
				break;

				case 3:
				SET_VECTOR_ELT(aux,j, NEW_INTEGER(1)); 
SET_STRING_ELT(aux,j,ScalarInteger(atoi(reg[j].pszStr)));
				break;

				case 4:
				SET_VECTOR_ELT(aux,j, NEW_NUMERIC(1)); 
SET_STRING_ELT(aux,j,ScalarReal(atof(reg[j].pszStr)));
				break;

				case 5:
				SET_VECTOR_ELT(aux,j, NEW_INTEGER(1)); 
				SET_VECTOR_ELT(aux,j, ScalarInteger(reg[j].nInt32)); 
				break;
				
				case 6:
				SET_VECTOR_ELT(aux,j, NEW_NUMERIC(1)); 


				SET_VECTOR_ELT(aux,j, ScalarReal(reg[j].dDouble)); 
				break;
			}


		}
	}

	UNPROTECT(1);
	return data;
}

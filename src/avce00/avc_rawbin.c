/**********************************************************************
 * $Id: avc_rawbin.c,v 1.1.1.1 2001/06/27 20:10:56 vir Exp $
 *
 * Name:     avc_rawbin.c
 * Project:  Arc/Info vector coverage (AVC)  BIN->E00 conversion library
 * Language: ANSI C
 * Purpose:  Raw Binary file access functions.
 * Author:   Daniel Morissette, danmo@videotron.ca
 *
 **********************************************************************
 * Copyright (c) 1999, Daniel Morissette
 *
 * All rights reserved.  This software may be copied or reproduced, in
 * all or in part, without the prior written consent of its author,
 * Daniel Morissette (danmo@videotron.ca).  However, any material copied
 * or reproduced must bear the original copyright notice (above), this 
 * original paragraph, and the original disclaimer (below).
 * 
 * The entire risk as to the results and performance of the software,
 * supporting text and other information contained in this file
 * (collectively called the "Software") is with the user.  Although 
 * considerable efforts have been used in preparing the Software, the 
 * author does not warrant the accuracy or completeness of the Software.
 * In no event will the author be liable for damages, including loss of
 * profits or consequential damages, arising out of the use of the 
 * Software.
 * 
 **********************************************************************
 *
 * $Log: avc_rawbin.c,v $
 * Revision 1.1.1.1  2001/06/27 20:10:56  vir
 * Initial release (0.1) under the cvs tree at Sourceforge.
 *
 * Revision 1.1.1.1  2001/06/27 20:04:13  vir
 * Initial release (0.1) under the CVS tree.
 *
 * Revision 1.6  1999/08/29 15:05:43  daniel
 * Added source filename in "Attempt to read past EOF" error message
 *
 * Revision 1.5  1999/06/08 22:09:03  daniel
 * Allow opening file with "r+" (but no real random access support yet)
 *
 * Revision 1.4  1999/05/11 02:10:51  daniel
 * Added write support
 *
 * Revision 1.3  1999/03/03 19:55:21  daniel
 * Fixed syntax error in the CPL_MSB version of AVCRawBinReadInt32()
 *
 * Revision 1.2  1999/02/25 04:20:08  daniel
 * Modified AVCRawBinEOF() to detect EOF even if AVCRawBinFSeek() was used.
 *
 * Revision 1.1  1999/01/29 16:28:52  daniel
 * Initial revision
 *
 **********************************************************************/

#include "avc.h"

/*=====================================================================
 * Stuff related to buffered reading of raw binary files
 *====================================================================*/

/**********************************************************************
 *                          AVCRawBinOpen()
 *
 * Open a binary file for reading with buffering, or writing.
 *
 * Returns a valid AVCRawBinFile structure, or NULL if the file could
 * not be opened or created.
 *
 * AVCRawBinClose() will eventually have to be called to release the 
 * resources used by the AVCRawBinFile structure.
 **********************************************************************/
AVCRawBinFile *AVCRawBinOpen(const char *pszFname, const char *pszAccess)
{
    AVCRawBinFile *psFile;

    psFile = (AVCRawBinFile*)CPLCalloc(1, sizeof(AVCRawBinFile));

    /*-----------------------------------------------------------------
     * Validate access mode and open/create file.  
     * For now we support only: "r" for read-only or "w" for write-only
     * or "a" for append.
     *
     * A case for "r+" is included here, but random access is not
     * properly supported yet... so this option should be used with care.
     *----------------------------------------------------------------*/
    if (EQUALN(pszAccess, "r+", 2))
    {
        psFile->eAccess = AVCReadWrite;
        psFile->fp = VSIFOpen(pszFname, "r+b");
    }
    else if (EQUALN(pszAccess, "r", 1))
    {
        psFile->eAccess = AVCRead;
        psFile->fp = VSIFOpen(pszFname, "rb");
    }
    else if (EQUALN(pszAccess, "w", 1))
    {
        psFile->eAccess = AVCWrite;
        psFile->fp = VSIFOpen(pszFname, "wb");
    }
    else if (EQUALN(pszAccess, "a", 1))
    {
        psFile->eAccess = AVCWrite;
        psFile->fp = VSIFOpen(pszFname, "ab");
    }
    else
    {
        CPLError(CE_Failure, CPLE_IllegalArg,
                 "Acces mode \"%s\" not supported.", pszAccess);
        CPLFree(psFile);
        return NULL;
    }

    /*-----------------------------------------------------------------
     * Check that file was opened succesfully, and init struct.
     *----------------------------------------------------------------*/
    if (psFile->fp)
    {
        /* The psFile structure should be initialized here... 
         * but there is nothing to initialize for now.
         */

        psFile->pszFname = CPLStrdup(pszFname);
    }
    else
    {
        CPLError(CE_Failure, CPLE_OpenFailed,
                 "Failed to open file %s", pszFname);
        CPLFree(psFile);
        psFile = NULL;
    }

    return psFile;
}

/**********************************************************************
 *                          AVCRawBinClose()
 *
 * Close a binary file previously opened with AVCRawBinOpen() and release
 * any memory used by the handle.
 **********************************************************************/
void AVCRawBinClose(AVCRawBinFile *psFile)
{
    if (psFile)
    {
        if (psFile->fp)
            VSIFClose(psFile->fp);
        CPLFree(psFile->pszFname);
        CPLFree(psFile);
    }
}

/**********************************************************************
 *                          AVCRawBinReadBytes()
 *
 * Copy the number of bytes from the input file to the specified 
 * memory location.
 **********************************************************************/
static GBool bDisableReadBytesEOFError = FALSE;

void AVCRawBinReadBytes(AVCRawBinFile *psFile, int nBytesToRead, GByte *pBuf)
{
    /* Make sure file is opened with Read access
     */
    if (psFile == NULL || 
        (psFile->eAccess != AVCRead && psFile->eAccess != AVCReadWrite))
    {
        CPLError(CE_Failure, CPLE_FileIO,
                "AVCRawBinReadBytes(): call not compatible with access mode.");
        return;
    }

    /* Quick method: check to see if we can satisfy the request with a
     * simple memcpy... most calls should take this path.
     */
    if (psFile->nCurPos + nBytesToRead <= psFile->nCurSize)
    {
        memcpy(pBuf, psFile->abyBuf+psFile->nCurPos, nBytesToRead);
        psFile->nCurPos += nBytesToRead;
        return;
    }

    /* This is the long method... it supports reading data that 
     * overlaps the input buffer boundaries.
     */
    while(nBytesToRead > 0)
    {
        /* If we reached the end of our memory buffer then read another
         * chunk from the file
         */
        CPLAssert(psFile->nCurPos <= psFile->nCurSize);
        if (psFile->nCurPos == psFile->nCurSize)
        {
            psFile->nOffset += psFile->nCurSize;
            psFile->nCurSize = VSIFRead(psFile->abyBuf, sizeof(GByte),
                                        AVCRAWBIN_READBUFSIZE, psFile->fp);
            psFile->nCurPos = 0;
        }

        if (psFile->nCurSize == 0)
        {
            /* Attempt to read past EOF... generate an error.
             *
             * Note: AVCRawBinEOF() can set bDisableReadBytesEOFError=TRUE
             *       to disable the error message whils it is testing
             *       for EOF.
             */
            if (bDisableReadBytesEOFError == FALSE)
                CPLError(CE_Failure, CPLE_FileIO,
                         "Attempt to read past EOF in %s.", psFile->pszFname);
            return;
        }

        /* If the requested bytes are not all in the current buffer then
         * just read the part that's in memory for now... the loop will 
         * take care of the rest.
         */
        if (psFile->nCurPos + nBytesToRead > psFile->nCurSize)
        {
            int nBytes;
            nBytes = psFile->nCurSize-psFile->nCurPos;
            memcpy(pBuf, psFile->abyBuf+psFile->nCurPos, nBytes);
            psFile->nCurPos += nBytes;
            pBuf += nBytes;
            nBytesToRead -= nBytes;
        }
        else
        {
            /* All the requested bytes are now in the buffer... 
             * simply copy them and return.
             */
            memcpy(pBuf, psFile->abyBuf+psFile->nCurPos, nBytesToRead);
            psFile->nCurPos += nBytesToRead;

            nBytesToRead = 0;   /* Terminate the loop */
        }
    }
}

/**********************************************************************
 *                          AVCRawBinFSeek()
 *
 * Move the read pointer to the specified location.
 *
 * As with fseek(), the specified position can be relative to the 
 * beginning of the file (SEEK_SET), or the current position (SEEK_CUR).
 * SEEK_END is not supported.
 **********************************************************************/
void AVCRawBinFSeek(AVCRawBinFile *psFile, int nOffset, int nFrom)
{
    int  nTarget = 0;

    CPLAssert(nFrom == SEEK_SET || nFrom == SEEK_CUR);

    /* Supported only with read access for now
     */
    CPLAssert(psFile && psFile->eAccess != AVCWrite);
    if (psFile == NULL || psFile->eAccess == AVCWrite)
        return;

    /* Compute destination relative to current memory buffer 
     */
    if (nFrom == SEEK_SET)
        nTarget = nOffset - psFile->nOffset;
    else if (nFrom == SEEK_CUR)
        nTarget = nOffset + psFile->nCurPos;

    /* Is the destination located inside the current buffer?
     */
    if (nTarget > 0 && nTarget <= psFile->nCurSize)
    {
        /* Requested location is already in memory... just move the 
         * read pointer
         */
        psFile->nCurPos = nTarget;
    }
    else
    {
        /* Requested location is not part of the memory buffer...
         * move the FILE * to the right location and be ready to 
         * read from there.
         */
        VSIFSeek(psFile->fp, psFile->nOffset+nTarget, SEEK_SET);
        psFile->nCurPos = 0;
        psFile->nCurSize = 0;
        psFile->nOffset = psFile->nOffset+nTarget;
    }

}

/**********************************************************************
 *                          AVCRawBinEOF()
 *
 * Return TRUE if there is no more data to read from the file or
 * FALSE otherwise.
 **********************************************************************/
GBool AVCRawBinEOF(AVCRawBinFile *psFile)
{
    if (psFile == NULL || psFile->fp == NULL)
        return TRUE;

    /* In write access mode, always return TRUE, since we always write
     * at EOF for now.
     */
    if (psFile->eAccess != AVCRead && psFile->eAccess != AVCReadWrite)
        return TRUE;

    /* If the file pointer has been moved by AVCRawBinFSeek(), then
     * we may be at a position past EOF, but VSIFeof() would still
     * return FALSE.
     * To prevent this situation, if the memory buffer is empty,
     * we will try to read 1 byte from the file to force the next
     * chunk of data to be loaded (and we'll move the the read pointer
     * back by 1 char after of course!).  
     * If we are at the end of the file, this will trigger the EOF flag.
     */
    if (psFile->nCurPos == 0 && psFile->nCurSize == 0)
    {
        char c;
        /* Set bDisableReadBytesEOFError=TRUE to temporarily disable 
         * the EOF error message from AVCRawBinReadBytes().
         */
        bDisableReadBytesEOFError = TRUE;
        AVCRawBinReadBytes(psFile, 1, &c);
        bDisableReadBytesEOFError = FALSE;

        if (psFile->nCurPos > 0)
            AVCRawBinFSeek(psFile, -1, SEEK_CUR);
    }

    return (psFile->nCurPos == psFile->nCurSize && 
            VSIFEof(psFile->fp));
}


/**********************************************************************
 *                          AVCRawBinRead<datatype>()
 *
 * Arc/Info files are binary files with MSB first (Motorola) byte 
 * ordering.  The following functions will read from the input file
 * and return a value with the bytes ordered properly for the current 
 * platform.
 **********************************************************************/
GInt16  AVCRawBinReadInt16(AVCRawBinFile *psFile)
{
    GInt16 n16Value;

    AVCRawBinReadBytes(psFile, 2, (GByte*)(&n16Value));

#ifdef CPL_LSB
    return (GInt16)CPL_SWAP16(n16Value);
#else
    return n16Value;
#endif
}

GInt32  AVCRawBinReadInt32(AVCRawBinFile *psFile)
{
    GInt32 n32Value;

    AVCRawBinReadBytes(psFile, 4, (GByte*)(&n32Value));

#ifdef CPL_LSB
    return (GInt32)CPL_SWAP32(n32Value);
#else
    return n32Value;
#endif
}

float   AVCRawBinReadFloat(AVCRawBinFile *psFile)
{
    float fValue;

    AVCRawBinReadBytes(psFile, 4, (GByte*)(&fValue));

#ifdef CPL_LSB
    *(GUInt32*)(&fValue) = CPL_SWAP32(*(GUInt32*)(&fValue));
#endif
    return fValue;
}

double  AVCRawBinReadDouble(AVCRawBinFile *psFile)
{
    double dValue;

    AVCRawBinReadBytes(psFile, 8, (GByte*)(&dValue));

#ifdef CPL_LSB
    CPL_SWAPDOUBLE(&dValue);
#endif

    return dValue;
}



/**********************************************************************
 *                          AVCRawBinWriteBytes()
 *
 * Write the number of bytes from the buffer to the file.
 *
 * If a problem happens, then CPLError() will be called and 
 * CPLGetLastErrNo() can be used to test if a write operation was 
 * succesful.
 **********************************************************************/
void AVCRawBinWriteBytes(AVCRawBinFile *psFile, int nBytesToWrite, GByte *pBuf)
{
    /*----------------------------------------------------------------
     * Make sure file is opened with Write access
     *---------------------------------------------------------------*/
    if (psFile == NULL || 
        (psFile->eAccess != AVCWrite && psFile->eAccess != AVCReadWrite))
    {
        CPLError(CE_Failure, CPLE_FileIO,
              "AVCRawBinWriteBytes(): call not compatible with access mode.");
        return;
    }

    if (VSIFWrite(pBuf, nBytesToWrite, 1, psFile->fp) != 1)
        CPLError(CE_Failure, CPLE_FileIO,
                 "Writing to %s failed.", psFile->pszFname);

    /*----------------------------------------------------------------
     * In write mode, we keep track of current file position ( =nbr of
     * bytes written) through psFile->nCurPos
     *---------------------------------------------------------------*/
    psFile->nCurPos += nBytesToWrite;
}


/**********************************************************************
 *                          AVCRawBinWrite<datatype>()
 *
 * Arc/Info files are binary files with MSB first (Motorola) byte 
 * ordering.  The following functions will reorder the byte for the
 * value properly and write that to the output file.
 *
 * If a problem happens, then CPLError() will be called and 
 * CPLGetLastErrNo() can be used to test if a write operation was 
 * succesful.
 **********************************************************************/
void  AVCRawBinWriteInt16(AVCRawBinFile *psFile, GInt16 n16Value)
{
#ifdef CPL_LSB
    n16Value = (GInt16)CPL_SWAP16(n16Value);
#endif

    AVCRawBinWriteBytes(psFile, 2, (GByte*)&n16Value);
}

void  AVCRawBinWriteInt32(AVCRawBinFile *psFile, GInt32 n32Value)
{
#ifdef CPL_LSB
    n32Value = (GInt32)CPL_SWAP32(n32Value);
#endif

    AVCRawBinWriteBytes(psFile, 4, (GByte*)&n32Value);
}

void  AVCRawBinWriteFloat(AVCRawBinFile *psFile, float fValue)
{
#ifdef CPL_LSB
    *(GUInt32*)(&fValue) = CPL_SWAP32(*(GUInt32*)(&fValue));
#endif

    AVCRawBinWriteBytes(psFile, 4, (GByte*)&fValue);
}

void  AVCRawBinWriteDouble(AVCRawBinFile *psFile, double dValue)
{
#ifdef CPL_LSB
    CPL_SWAPDOUBLE(&dValue);
#endif

    AVCRawBinWriteBytes(psFile, 8, (GByte*)&dValue);
}


/**********************************************************************
 *                          AVCRawBinWriteZeros()
 *
 * Write a number of zeros (sepcified in bytes) at the current position 
 * in the file.
 *
 * If a problem happens, then CPLError() will be called and 
 * CPLGetLastErrNo() can be used to test if a write operation was 
 * succesful.
 **********************************************************************/
void AVCRawBinWriteZeros(AVCRawBinFile *psFile, int nBytesToWrite)
{
    char acZeros[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int i;

    /* Write by 8 bytes chunks.  The last chunk may be less than 8 bytes 
     */
    for(i=0; i< nBytesToWrite; i+=8)
    {
        AVCRawBinWriteBytes(psFile, MIN(8,(nBytesToWrite-i)), 
                            (GByte*)acZeros);
    }
}

/**********************************************************************
 *                          AVCRawBinWritePaddedString()
 *
 * Write a string and pad the end of the field (up to nFieldSize) with
 * spaces number of spaces at the current position in the file.
 *
 * If a problem happens, then CPLError() will be called and 
 * CPLGetLastErrNo() can be used to test if a write operation was 
 * succesful.
 **********************************************************************/
void AVCRawBinWritePaddedString(AVCRawBinFile *psFile, int nFieldSize,
                                const char *pszString)
{
    char acSpaces[8] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    int i, nLen, numSpaces;

    nLen = strlen(pszString);
    nLen = MIN(nLen, nFieldSize);
    numSpaces = nFieldSize - nLen;

    if (nLen > 0)
        AVCRawBinWriteBytes(psFile, nLen, (GByte*)pszString);

    /* Write spaces by 8 bytes chunks.  The last chunk may be less than 8 bytes
     */
    for(i=0; i< numSpaces; i+=8)
    {
        AVCRawBinWriteBytes(psFile, MIN(8,(numSpaces-i)), 
                            (GByte*)acSpaces);
    }
}

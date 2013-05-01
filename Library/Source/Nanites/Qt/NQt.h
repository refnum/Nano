/*	NAME:
		NQt.h

	DESCRIPTION:
		Qt support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NQT_HDR
#define NQT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRectangle.h"
#include "NPoint.h"
#include "NSize.h"
#include "NBundle.h"
#include "NString.h"





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Nano to Qt
inline QPointF ToQt(const NPoint &thePoint)
{
	return(QPointF(thePoint.x, thePoint.y));
}

inline QSizeF ToQt(const NSize &theSize)
{
	return(QSizeF(theSize.width, theSize.height));
}

inline QRectF ToQt(const NRectangle &theRect)
{
	return(QRectF(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height));
}

inline QPoint ToQtI(const NPoint &thePoint)
{
	return(QPoint((int) thePoint.x, (int) thePoint.y));
}

inline QSize ToQtI(const NSize &theSize)
{
	return(QSize((int) theSize.width, (int) theSize.height));
}

inline QRect ToQtI(const NRectangle &theRect)
{
	return(QRect((int) theRect.origin.x, (int) theRect.origin.y, (int) theRect.size.width, (int) theRect.size.height));
}

inline QColor ToQt(const NColor &theColor)
{	QColor		theResult;

	theResult.setRgbF(theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue(), theColor.GetAlpha());
	return(theResult);
}

inline QByteArray ToQt(const NData &theData)
{
	return(QByteArray((const char *) theData.GetData(), (int) theData.GetSize()));
}

inline QString ToQt(const NString &theString)
{
	return(QString::fromUtf8(theString.GetUTF8()).normalized(QString::NormalizationForm_C));
}



// Qt to Nano
inline NPoint ToNN(const QPoint &thePoint)
{
	return(NPoint(thePoint.x(), thePoint.y()));
}

inline NSize ToNN(const QSize &theSize)
{
	return(NSize(theSize.width(), theSize.height()));
}

inline NRectangle ToNN(const QRect &theRect)
{
	return(NRectangle(theRect.x(), theRect.y(), theRect.width(), theRect.height()));
}

inline NPoint ToNN(const QPointF &thePoint)
{
	return(NPoint(thePoint.x(), thePoint.y()));
}

inline NSize ToNN(const QSizeF &theSize)
{
	return(NSize(theSize.width(), theSize.height()));
}

inline NRectangle ToNN(const QRectF &theRect)
{
	return(NRectangle(theRect.x(), theRect.y(), theRect.width(), theRect.height()));
}

inline NColor ToNN(const QColor &theColor)
{
	return(NColor(theColor.redF(), theColor.greenF(), theColor.blueF(), theColor.alphaF()));
}

inline NData ToNN(const QByteArray &theData)
{
	return(NData(theData.size(), theData.data()));
}

inline NString ToNN(const QString &theString)
{
	return(NString(theString.toUtf8().data(), kNStringLength));
}



// Bundles
inline QString NQtBundleResource(const NString &theName, const NString &theType="", const NString &subDir="")
{	QString		thePath;
	NFile		theFile;

	theFile = NBundleResource(theName, theType, subDir);
	if (theFile.Exists())
		thePath = ToQt(theFile.GetPath());
	
	return(thePath);
}

inline QString NQtBundleString(const NString &theKey, const NString &defaultValue="", const NString &tableName="")
{
	return(ToQt(NBundleString(theKey, defaultValue, tableName)));
}

inline QPixmap NQtBundlePixmap(const NString &theKey, const NString &defaultValue="", const NString &tableName="")
{
	return(QPixmap(NQtBundleResource(theKey, defaultValue, tableName)));
}

inline QIcon NQtBundleIcon(const NString &theKey, const NString &defaultValue="", const NString &tableName="")
{
	return(QIcon(NQtBundleResource(theKey, defaultValue, tableName)));
}





#endif // NQT_HDR






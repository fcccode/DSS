#ifndef __DEBLOOM_H__
#define __DEBLOOM_H__

#include "DSSProgress.h"
#include "DSSTools.h"
#include "BitmapExt.h"

/* ------------------------------------------------------------------- */

class CBloomInfo
{
public :
	CPointExt			m_ptRef;
	double				m_fBloom;
	double				m_fRadius;
	double				m_fAngle;

private:
	void	CopyFrom(const CBloomInfo & right)
	{
		m_ptRef		= right.m_ptRef;
		m_fBloom	= right.m_fBloom;
		m_fRadius	= right.m_fRadius;
		m_fAngle	= right.m_fAngle;
	};

public:
	CBloomInfo()
	{
	};
	~CBloomInfo()
	{
	};

	CBloomInfo(const CBloomInfo & right)
	{
		CopyFrom(right);
	};

	CBloomInfo & operator = (const CBloomInfo & right)
	{
		CopyFrom(right);
		return (*this);
	};
};

typedef std::vector<CBloomInfo>			BLOOMINFOVECTOR;

/* ------------------------------------------------------------------- */

class CBloomedStar
{
public :
	CPointExt			m_ptStar;
	std::vector<CPoint>	m_vBloomed;
	BLOOMINFOVECTOR		m_vBlooms;
	double				m_fRadius;
	double				m_fBloom;


private:
	void	CopyFrom(const CBloomedStar & right)
	{
		m_ptStar	= right.m_ptStar;
		m_vBloomed  = right.m_vBloomed;
		m_vBlooms	= right.m_vBlooms;
		m_fRadius   = right.m_fRadius;
		m_fBloom	= right.m_fBloom;
	};

public:
	CBloomedStar()
	{
	};
	~CBloomedStar()
	{
	};

	CBloomedStar(const CBloomedStar & right)
	{
		CopyFrom(right);
	};

	CBloomedStar & operator = (const CBloomedStar & right)
	{
		CopyFrom(right);
		return (*this);
	};

	bool operator < (const CBloomedStar & right) const
	{
		return true;
	};
};

typedef std::vector<CBloomedStar>		BLOOMEDSTARVECTOR;

/* ------------------------------------------------------------------- */

class CBloomedStarGradient
{
public :
	CPointExt			ptStar;
	double				fdX,
						fdY;
	double				fNW, 
						fSW, 
						fNE, 
						fSE;
	double				fGradient;
	double				fPercentGradient;


private:
	void	CopyFrom(const CBloomedStarGradient & right)
	{
		ptStar	= right.ptStar	;
		fdX		= right.fdX		;
		fdY		= right.fdY		;
		fNW		= right.fNW		; 
		fSW		= right.fSW		; 
		fNE		= right.fNE		; 
		fSE		= right.fSE		;
		fGradient = right.fGradient;
		fPercentGradient = right.fPercentGradient;
	};

public:
	CBloomedStarGradient()
	{
		fdX = 0;
		fdY = 0;
		fGradient = 0;
		fPercentGradient = 0;
	};
	~CBloomedStarGradient()
	{
	};

	CBloomedStarGradient(const CBloomedStarGradient & right)
	{
		CopyFrom(right);
	};

	CBloomedStarGradient & operator = (const CBloomedStarGradient & right)
	{
		CopyFrom(right);
		return (*this);
	};
};

/* ------------------------------------------------------------------- */

class CDeBloom
{
private :
	double						m_fBloomThreshold;
	BLOOMEDSTARVECTOR			m_vBloomedStars;
	LONG						m_lWidth,
								m_lHeight;
	CSmartPtr<C8BitGrayBitmap>	m_pMask;
	CDSSProgress *				m_pProgress;
	double						m_fBackground;

	bool	IsLeftEdge(CMemoryBitmap * pBitmap, LONG x, LONG y);
	bool	IsRightEdge(CMemoryBitmap * pBitmap, LONG x, LONG y);
	bool	IsTopEdge(CMemoryBitmap * pBitmap, LONG x, LONG y);
	bool	IsBottomEdge(CMemoryBitmap * pBitmap, LONG x, LONG y);

	void	ComputeStarCenter(CMemoryBitmap * pBitmap, C8BitGrayBitmap * pMask, CBloomedStar & bs);

	void	ExpandBloomedArea(CMemoryBitmap * pBitmap, C8BitGrayBitmap * pMask, LONG x, LONG y);
	bool	CreateMask(CMemoryBitmap * pBitmap, C8BitGrayBitmap ** ppMask);

	void	AddStar(CMemoryBitmap * pBitmap, C8BitGrayBitmap * pMask, CBloomedStar & bs);
	double	ComputeValue(CMemoryBitmap * pBitmap, C8BitGrayBitmap * pMask, LONG x, LONG y, bool & bDone);
	void	DeBloom(CMemoryBitmap * pBitmap, C8BitGrayBitmap * pMask);
	void    SmoothMaskBorders(CMemoryBitmap * pBitmap, C8BitGrayBitmap * pMask);
	void	MarkBloomBorder(CMemoryBitmap * pMask, LONG x, LONG y, std::vector<CPointExt> & vBorders);
	void	MarkBorderAsBloomed(CMemoryBitmap * pMask, LONG x, LONG y, std::vector<CPoint> & vBloomed);

	double	ComputeBackgroundValue(CMemoryBitmap * pBitmap);
	double	ComputeStarGradient(CMemoryBitmap * pBitmap, C8BitGrayBitmap * pMask, CBloomedStarGradient & bsg, double fRadius);
	void	RefineStarCenter(CMemoryBitmap * pBitmap, C8BitGrayBitmap * pMask, CBloomedStar & bs);
	void	RefineStarCenter2(CMemoryBitmap * pBitmap, C8BitGrayBitmap * pMask, CBloomedStar & bs);

public :
	CDeBloom() 
	{
		m_fBloomThreshold = 0.85;
		m_fBackground = 0;
	};

	virtual ~CDeBloom() {};

	bool	CreateBloomMask(CMemoryBitmap * pBitmap, CDSSProgress * pProgress);
	void	DeBloomImage(CMemoryBitmap * pBitmap, CDSSProgress * pProgress);
};

#endif // __DEBLOOM_H__
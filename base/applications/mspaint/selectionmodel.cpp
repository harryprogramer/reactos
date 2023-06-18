/*
 * PROJECT:     PAINT for ReactOS
 * LICENSE:     LGPL
 * FILE:        base/applications/mspaint/selectionmodel.cpp
 * PURPOSE:     Keep track of selection parameters, notify listeners
 * PROGRAMMERS: Benedikt Freisen
 *              Katayama Hirofumi MZ
 */

#include "precomp.h"

SelectionModel selectionModel;

/* FUNCTIONS ********************************************************/

SelectionModel::SelectionModel()
    : m_hbmColor(NULL)
    , m_hbmMask(NULL)
    , m_ptStack(NULL)
    , m_iPtSP(0)
    , m_bShow(FALSE)
{
    ::SetRectEmpty(&m_rc);
    ::SetRectEmpty(&m_rcOld);
    m_ptHit.x = m_ptHit.y = -1;
}

SelectionModel::~SelectionModel()
{
    ClearColor();
    ClearMask();
    ResetPtStack();
}

void SelectionModel::ResetPtStack()
{
    if (m_ptStack)
    {
        free(m_ptStack);
        m_ptStack = NULL;
    }
    m_iPtSP = 0;
}

void SelectionModel::PushToPtStack(POINT pt)
{
#define GROW_COUNT 256
    if (m_iPtSP % GROW_COUNT == 0)
    {
        INT nNewCount = m_iPtSP + GROW_COUNT;
        LPPOINT pptNew = (LPPOINT)realloc(m_ptStack, sizeof(POINT) * nNewCount);
        if (pptNew == NULL)
            return;
        m_ptStack = pptNew;
    }
    m_ptStack[m_iPtSP] = pt;
    m_iPtSP++;
#undef GROW_COUNT
}

void SelectionModel::ShiftPtStack(INT dx, INT dy)
{
    for (INT i = 0; i < m_iPtSP; ++i)
    {
        POINT& pt = m_ptStack[i];
        pt.x += dx;
        pt.y += dy;
    }
}

void SelectionModel::BuildMaskFromPtStack()
{
    CRect rc = { MAXLONG, MAXLONG, 0, 0 };
    for (INT i = 0; i < m_iPtSP; ++i)
    {
        POINT& pt = m_ptStack[i];
        rc.left = min(pt.x, rc.left);
        rc.top = min(pt.y, rc.top);
        rc.right = max(pt.x, rc.right);
        rc.bottom = max(pt.y, rc.bottom);
    }
    rc.right += 1;
    rc.bottom += 1;

    m_rc = m_rcOld = rc;

    ClearMask();

    ShiftPtStack(-m_rcOld.left, -m_rcOld.top);

    HDC hdcMem = ::CreateCompatibleDC(NULL);
    m_hbmMask = ::CreateBitmap(rc.Width(), rc.Height(), 1, 1, NULL);
    HGDIOBJ hbmOld = ::SelectObject(hdcMem, m_hbmMask);
    ::FillRect(hdcMem, &rc, (HBRUSH)::GetStockObject(BLACK_BRUSH));
    HGDIOBJ hPenOld = ::SelectObject(hdcMem, GetStockObject(NULL_PEN));
    HGDIOBJ hbrOld = ::SelectObject(hdcMem, GetStockObject(WHITE_BRUSH));
    ::Polygon(hdcMem, m_ptStack, m_iPtSP);
    ::SelectObject(hdcMem, hbrOld);
    ::SelectObject(hdcMem, hPenOld);
    ::SelectObject(hdcMem, hbmOld);
    ::DeleteDC(hdcMem);

    ShiftPtStack(+m_rcOld.left, +m_rcOld.top);
}

void SelectionModel::DrawBackgroundPoly(HDC hDCImage, COLORREF crBg)
{
    if (::IsRectEmpty(&m_rcOld))
        return;

    HGDIOBJ hPenOld = ::SelectObject(hDCImage, ::GetStockObject(NULL_PEN));
    HGDIOBJ hbrOld = ::SelectObject(hDCImage, ::CreateSolidBrush(crBg));
    ::Polygon(hDCImage, m_ptStack, m_iPtSP);
    ::DeleteObject(::SelectObject(hDCImage, hbrOld));
    ::SelectObject(hDCImage, hPenOld);
}

void SelectionModel::DrawBackgroundRect(HDC hDCImage, COLORREF crBg)
{
    if (::IsRectEmpty(&m_rcOld))
        return;

    Rect(hDCImage, m_rcOld.left, m_rcOld.top, m_rcOld.right, m_rcOld.bottom, crBg, crBg, 0, 1);
}

void SelectionModel::DrawSelection(HDC hDCImage, COLORREF crBg, BOOL bBgTransparent)
{
    CRect rc = m_rc;
    if (::IsRectEmpty(&rc))
        return;

    BITMAP bm;
    if (!GetObject(m_hbmColor, sizeof(BITMAP), &bm))
        return;

    COLORREF keyColor = (bBgTransparent ? crBg : CLR_INVALID);

    HDC hMemDC = CreateCompatibleDC(hDCImage);
    HGDIOBJ hbmOld = SelectObject(hMemDC, m_hbmColor);
    ColorKeyedMaskBlt(hDCImage, rc.left, rc.top, rc.Width(), rc.Height(),
                      hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, m_hbmMask, keyColor);
    SelectObject(hMemDC, hbmOld);
    DeleteDC(hMemDC);
}

void SelectionModel::GetSelectionContents(HDC hDCImage)
{
    ClearColor();

    HDC hMemDC = ::CreateCompatibleDC(NULL);
    m_hbmColor = CreateColorDIB(m_rc.Width(), m_rc.Height(), RGB(255, 255, 255));
    HGDIOBJ hbmOld = ::SelectObject(hMemDC, m_hbmColor);
    ::BitBlt(hMemDC, 0, 0, m_rc.Width(), m_rc.Height(), hDCImage, m_rc.left, m_rc.top, SRCCOPY);
    ::SelectObject(hMemDC, hbmOld);
    ::DeleteDC(hMemDC);
}

BOOL SelectionModel::IsLanded() const
{
    return !m_hbmColor;
}

BOOL SelectionModel::TakeOff()
{
    if (!IsLanded() || ::IsRectEmpty(&m_rc))
        return FALSE;

    m_rgbBack = paletteModel.GetBgColor();
    GetSelectionContents(imageModel.GetDC());

    if (toolsModel.GetActiveTool() == TOOL_RECTSEL)
        ClearMask();

    m_rcOld = m_rc;

    imageModel.NotifyImageChanged();
    return TRUE;
}

void SelectionModel::Landing()
{
    if (IsLanded() && !m_bShow)
    {
        imageModel.NotifyImageChanged();
        return;
    }

    m_bShow = FALSE;

    if (!::EqualRect(m_rc, m_rcOld) && !::IsRectEmpty(m_rc) && !::IsRectEmpty(m_rcOld))
    {
        imageModel.PushImageForUndo();

        canvasWindow.m_drawing = FALSE;
        toolsModel.OnDrawOverlayOnImage(imageModel.GetDC());
    }

    HideSelection();
}

void SelectionModel::InsertFromHBITMAP(HBITMAP hBm, INT x, INT y)
{
    ::DeleteObject(m_hbmColor);
    m_hbmColor = CopyDIBImage(hBm);

    m_rc.left = x;
    m_rc.top = y;
    m_rc.right = x + GetDIBWidth(hBm);
    m_rc.bottom = y + GetDIBHeight(hBm);

    // If m_rc and m_rcOld were same, the image cannot be pasted to the canvas.
    // See also SelectionModel::Landing
    ::SetRect(&m_rcOld, -2, -2, -1, -1); // Outside of image

    ClearMask();
}

void SelectionModel::FlipHorizontally()
{
    TakeOff();

    HDC hdcMem = ::CreateCompatibleDC(NULL);
    if (m_hbmMask)
    {
        ::SelectObject(hdcMem, m_hbmMask);
        ::StretchBlt(hdcMem, m_rc.Width() - 1, 0, -m_rc.Width(), m_rc.Height(),
                     hdcMem, 0, 0, m_rc.Width(), m_rc.Height(), SRCCOPY);
    }
    if (m_hbmColor)
    {
        ::SelectObject(hdcMem, m_hbmColor);
        ::StretchBlt(hdcMem, m_rc.Width() - 1, 0, -m_rc.Width(), m_rc.Height(),
                     hdcMem, 0, 0, m_rc.Width(), m_rc.Height(), SRCCOPY);
    }
    ::DeleteDC(hdcMem);

    imageModel.NotifyImageChanged();
}

void SelectionModel::FlipVertically()
{
    TakeOff();

    HDC hdcMem = ::CreateCompatibleDC(NULL);
    if (m_hbmMask)
    {
        ::SelectObject(hdcMem, m_hbmMask);
        ::StretchBlt(hdcMem, 0, m_rc.Height() - 1, m_rc.Width(), -m_rc.Height(),
                     hdcMem, 0, 0, m_rc.Width(), m_rc.Height(), SRCCOPY);
    }
    if (m_hbmColor)
    {
        ::SelectObject(hdcMem, m_hbmColor);
        ::StretchBlt(hdcMem, 0, m_rc.Height() - 1, m_rc.Width(), -m_rc.Height(),
                     hdcMem, 0, 0, m_rc.Width(), m_rc.Height(), SRCCOPY);
    }
    ::DeleteDC(hdcMem);

    imageModel.NotifyImageChanged();
}

void SelectionModel::RotateNTimes90Degrees(int iN)
{
    HBITMAP hbm;
    HGDIOBJ hbmOld;
    HDC hdcMem = ::CreateCompatibleDC(NULL);

    switch (iN)
    {
        case 1:
        case 3:
            TakeOff();
            if (m_hbmColor)
            {
                hbmOld = ::SelectObject(hdcMem, m_hbmColor);
                hbm = Rotate90DegreeBlt(hdcMem, m_rc.Width(), m_rc.Height(), iN == 1, FALSE);
                ::SelectObject(hdcMem, hbmOld);
                ::DeleteObject(m_hbmColor);
                m_hbmColor = hbm;
            }
            if (m_hbmMask)
            {
                hbmOld = ::SelectObject(hdcMem, m_hbmMask);
                hbm = Rotate90DegreeBlt(hdcMem, m_rc.Width(), m_rc.Height(), iN == 1, TRUE);
                ::SelectObject(hdcMem, hbmOld);
                ::DeleteObject(m_hbmMask);
                m_hbmMask = hbm;
            }
            break;

        case 2:
            TakeOff();
            if (m_hbmColor)
            {
                hbmOld = ::SelectObject(hdcMem, m_hbmColor);
                ::StretchBlt(hdcMem, m_rc.Width() - 1, m_rc.Height() - 1, -m_rc.Width(), -m_rc.Height(),
                             hdcMem, 0, 0, m_rc.Width(), m_rc.Height(), SRCCOPY);
                ::SelectObject(hdcMem, hbmOld);
            }
            if (m_hbmMask)
            {
                hbmOld = ::SelectObject(hdcMem, m_hbmMask);
                ::StretchBlt(hdcMem, m_rc.Width() - 1, m_rc.Height() - 1, -m_rc.Width(), -m_rc.Height(),
                             hdcMem, 0, 0, m_rc.Width(), m_rc.Height(), SRCCOPY);
                ::SelectObject(hdcMem, hbmOld);
            }
            break;
    }

    ::DeleteDC(hdcMem);
    imageModel.NotifyImageChanged();
}

void SelectionModel::StretchSkew(int nStretchPercentX, int nStretchPercentY, int nSkewDegX, int nSkewDegY)
{
    if (nStretchPercentX == 100 && nStretchPercentY == 100 && nSkewDegX == 0 && nSkewDegY == 0)
        return;

    TakeOff();

    INT oldWidth = m_rc.Width();
    INT oldHeight = m_rc.Height();
    INT newWidth = oldWidth * nStretchPercentX / 100;
    INT newHeight = oldHeight * nStretchPercentY / 100;

    if (oldWidth != newWidth || oldHeight != newHeight)
    {
        HBITMAP hbm0 = CopyDIBImage(m_hbmColor, newWidth, newHeight);
        InsertFromHBITMAP(hbm0, m_rc.left, m_rc.top);
        ::DeleteObject(hbm0);
    }

    HDC hDC = ::CreateCompatibleDC(NULL);

    if (nSkewDegX)
    {
        ::SelectObject(hDC, m_hbmColor);
        HBITMAP hbm1 = SkewDIB(hDC, m_hbmColor, nSkewDegX, FALSE);
        InsertFromHBITMAP(hbm1, m_rc.left, m_rc.top);
        ::DeleteObject(hbm1);
    }

    if (nSkewDegY)
    {
        ::SelectObject(hDC, m_hbmColor);
        HBITMAP hbm2 = SkewDIB(hDC, m_hbmColor, nSkewDegY, TRUE);
        InsertFromHBITMAP(hbm2, m_rc.left, m_rc.top);
        ::DeleteObject(hbm2);
    }

    ::DeleteDC(hDC);

    m_bShow = TRUE;
    imageModel.NotifyImageChanged();
}

HBITMAP SelectionModel::CopyBitmap()
{
    if (m_hbmColor == NULL)
        GetSelectionContents(imageModel.GetDC());
    return CopyDIBImage(m_hbmColor);
}

int SelectionModel::PtStackSize() const
{
    return m_iPtSP;
}

void SelectionModel::DrawFramePoly(HDC hDCImage)
{
    /* draw the freehand selection inverted/xored */
    Poly(hDCImage, m_ptStack, m_iPtSP, 0, 0, 2, 0, FALSE, TRUE);
}

void SelectionModel::SetRectFromPoints(const POINT& ptFrom, const POINT& ptTo)
{
    m_rc.left = min(ptFrom.x, ptTo.x);
    m_rc.top = min(ptFrom.y, ptTo.y);
    m_rc.right = max(ptFrom.x, ptTo.x);
    m_rc.bottom = max(ptFrom.y, ptTo.y);
}

void SelectionModel::Dragging(CANVAS_HITTEST hit, POINT pt)
{
    switch (hit)
    {
        case HIT_NONE:
            break;
        case HIT_UPPER_LEFT:
            m_rc.left += pt.x - m_ptHit.x;
            m_rc.top += pt.y - m_ptHit.y;
            break;
        case HIT_UPPER_CENTER:
            m_rc.top += pt.y - m_ptHit.y;
            break;
        case HIT_UPPER_RIGHT:
            m_rc.right += pt.x - m_ptHit.x;
            m_rc.top += pt.y - m_ptHit.y;
            break;
        case HIT_MIDDLE_LEFT:
            m_rc.left += pt.x - m_ptHit.x;
            break;
        case HIT_MIDDLE_RIGHT:
            m_rc.right += pt.x - m_ptHit.x;
            break;
        case HIT_LOWER_LEFT:
            m_rc.left += pt.x - m_ptHit.x;
            m_rc.bottom += pt.y - m_ptHit.y;
            break;
        case HIT_LOWER_CENTER:
            m_rc.bottom += pt.y - m_ptHit.y;
            break;
        case HIT_LOWER_RIGHT:
            m_rc.right += pt.x - m_ptHit.x;
            m_rc.bottom += pt.y - m_ptHit.y;
            break;
        case HIT_BORDER:
        case HIT_INNER:
            OffsetRect(&m_rc, pt.x - m_ptHit.x, pt.y - m_ptHit.y);
            break;
    }
    m_ptHit = pt;
}

void SelectionModel::ClearMask()
{
    if (m_hbmMask)
    {
        ::DeleteObject(m_hbmMask);
        m_hbmMask = NULL;
    }
}

void SelectionModel::ClearColor()
{
    if (m_hbmColor)
    {
        ::DeleteObject(m_hbmColor);
        m_hbmColor = NULL;
    }
}

void SelectionModel::HideSelection()
{
    m_bShow = FALSE;
    ClearColor();
    ClearMask();
    ::SetRectEmpty(&m_rc);
    ::SetRectEmpty(&m_rcOld);

    imageModel.NotifyImageChanged();
}

void SelectionModel::DeleteSelection()
{
    if (!m_bShow)
        return;

    TakeOff();

    imageModel.PushImageForUndo();
    if (toolsModel.GetActiveTool() == TOOL_FREESEL)
        DrawBackgroundPoly(imageModel.GetDC(), paletteModel.GetBgColor());
    else
        DrawBackgroundRect(imageModel.GetDC(), paletteModel.GetBgColor());

    HideSelection();
}

/*
** HFCL - HybridOS Foundation Class Library
**
** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
**
** This file is part of HFCL.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "common/common.h"
#include "mgcl/mgcl.h"

namespace hfcl {

//void mgclUpdateWindowClips(HWND hWnd, const PCLIPRGN pclips)
void mgclUpdateWindowClips(HWND hWnd, const HDC hdc)
{
    RECT rcme;
    HWND htopwnd = HWND_NULL;

    if(!IsWindow(hWnd))
        return;

    while((htopwnd = GetNextMainWindow(htopwnd)) && htopwnd != HWND_INVALID && htopwnd != hWnd)
    {
        if((GetWindowExStyle(htopwnd) & WS_EX_TRANSPARENT)&& IsWindowVisible(htopwnd))
        {
            GetWindowRect(htopwnd, &rcme);
            IncludeClipRect(hdc, &rcme);
            UpdateWindow(htopwnd, TRUE);
        }
    }
}

void mgclUpdateTransparentWindow(HWND hWnd, const PCLIPRGN pclips, int xoff, int yoff)
{
    RECT rcme;
    RECT rc;
    HWND htopwnd = HWND_NULL;

    if(!IsWindow(hWnd))
        return;

    GetWindowRect(hWnd, &rcme);

    while((htopwnd = GetNextMainWindow(htopwnd)) && htopwnd != HWND_INVALID && htopwnd != hWnd)
    {
        GetWindowRect(htopwnd, &rc);
        if(GetWindowExStyle(htopwnd) & WS_EX_TRANSPARENT)
        {
            if(IntersectRect(&rc, &rcme, &rc))
            {
                if(!IsWindowVisible(htopwnd))
                    continue;
                UpdateWindow(htopwnd, TRUE);
            }//end of intersect
        }
    }
}

#define GetDesktopOperationSet()  SetCustomDesktopOperationSet(NULL)

static void fill_dc_with_widget(HDC hdc, HWND hwnd, const RECT *prc)
{
    RECT rc;
    HDC hclientdc = 0;
    HWND hchild = HWND_NULL;
    RECT clientrc;

    SendMessage(hwnd, MSG_NCPAINT, (WPARAM)hdc, 0);

    //create the client dc if need
    GetClientRect(hwnd, &rc);
    clientrc = rc;
    ClientToWindow(hwnd, &clientrc.left, &clientrc.top);
    ClientToWindow(hwnd, &clientrc.right, &clientrc.bottom);
    if(!IntersectRect(&clientrc, &clientrc, prc))
        return ;

    WindowToClient(hwnd, &clientrc.left, &clientrc.top);
    WindowToClient(hwnd, &clientrc.right, &clientrc.bottom);
    {
        RECT rcwindow;
        GetWindowRect(hwnd, &rcwindow);
        if(RECTW(rcwindow) == RECTW(rc) && RECTH(rcwindow) == RECTH(rc))
            hclientdc = hdc;
        else
        {
            ClientToWindow(hwnd, &rc.left, &rc.top);
            ClientToWindow(hwnd, &rc.right, &rc.bottom);
            hclientdc = CreateSubMemDC(hdc, rc.left, rc.top, RECTW(rc), RECTH(rc), TRUE);
        }
    }

    //draw bkground
    if((IsMainWindow(hwnd) || !(GetWindowExStyle(hwnd)&WS_EX_TRANSPARENT)))
    {
        SendMessage(hwnd, MSG_ERASEBKGND, (WPARAM)hclientdc, (LPARAM)&clientrc);
    }

    //draw client
    SendMessage(hwnd, MSG_NCPAINT, (WPARAM)hclientdc, 0);

    //draw the sub windows
    while(IsWindow((hchild = GetNextChild(hwnd, hchild))))
    {
        RECT rcwindow;
        RECT rcarea;
        GetWindowRect(hchild, &rcwindow);
        if(IntersectRect(&rcarea, &rcwindow, &clientrc))
        {
            HDC hsubdc = CreateSubMemDC(hclientdc, rcwindow.left,
                    rcwindow.top, RECTW(rcwindow), RECTH(rcwindow), TRUE);
            OffsetRect(&rcarea, -rcwindow.left, -rcwindow.top);
            fill_dc_with_widget(hsubdc, hchild, &rcarea);
            DeleteMemDC(hsubdc);
        }
    }

    if(hclientdc != hdc)
        DeleteMemDC(hclientdc);
}

static void restore_window(HWND hwnd, HDC hdc_restore, const RECT *prc)
{
    RECT rc = *prc;
    ScreenToWindow(hwnd, &rc.left, &rc.top);
    ScreenToWindow(hwnd, &rc.right, &rc.bottom);
    fill_dc_with_widget(hdc_restore, hwnd, &rc);
}

void update_under_window(HWND hwnd, HDC hmemdc, const PRECT pupdate_rc,
        PCLIPRGN prgn, const POINT* ptoff)
{
    RECT rc;
    BOOL bneedsubtract;
    BOOL have_secondary_dc;
    HDC  hdc_restore;
    PCLIPRGN prestore_rgn;
    int x = 0, y = 0;

BEGIN:

    if(hwnd == HWND_NULL || hwnd == HWND_DESKTOP)
    {
        DESKTOPOPS *dsk_ops = GetDesktopOperationSet();
        if(dsk_ops && dsk_ops->paint_desktop)
        {
            if(!prgn)
            {
                OffsetRect(&rc, -ptoff->x, -ptoff->y);
                dsk_ops->paint_desktop(NULL, hmemdc, &rc);
            }
            else {
                CLIPRECT *pclprc;
                OffsetRegion(prgn, -ptoff->x, -ptoff->y);
                pclprc = prgn->head;
                while(pclprc)
                {
                    if(!IsRectEmpty(&pclprc->rc))
                        dsk_ops->paint_desktop(NULL, hmemdc, &pclprc->rc);
                    pclprc = pclprc->next;
                }
            }
        }
        return ;
    }

    GetWindowRect(hwnd, &rc);

    if(!IsWindowVisible(hwnd) || !IntersectRect(&rc, &rc, pupdate_rc)) {
        hwnd = GetNextMainWindow(hwnd);
        goto BEGIN;
    }

    have_secondary_dc = GetSecondaryDC(hwnd) != HDC_SCREEN;

    bneedsubtract = have_secondary_dc
                    || !(GetWindowExStyle(hwnd) & WS_EX_TRANSPARENT);

    //copy a rgn
    prestore_rgn = CreateClipRgn();
    ClipRgnCopy(prestore_rgn, prgn);

    //subtract the prgn
    if(bneedsubtract)
        SubtractClipRect(prgn, &rc);
    update_under_window(GetNextMainWindow(hwnd), hmemdc, pupdate_rc, prgn, ptoff);

    IntersectClipRect(prestore_rgn, &rc);

    //translate the screen to window coord
    WindowToScreen(hwnd, &x, &y);
    OffsetRegion(prestore_rgn, -x, -y);

    //create or get a restore dc to draw
    if(have_secondary_dc)
    {
        hdc_restore = GetSecondaryDC(hwnd);
        if(HDC_INVALID == hdc_restore)
        {
            if(prestore_rgn != prgn)
                DestroyClipRgn(prestore_rgn);
            return;
        }
    }
    else {
        hdc_restore = CreateSecondaryDC(hwnd);
        if(HDC_INVALID == hdc_restore)
        {
            if(prestore_rgn != prgn)
                DestroyClipRgn(prestore_rgn);
            return;
        }
        SelectClipRegion(hdc_restore, prestore_rgn);
        restore_window(hwnd, hdc_restore, &rc);
    }

    //bitblt to the hmemdc
    if(!bneedsubtract) {
        SetMemDCAlpha(hdc_restore, MEMDC_FLAG_SRCALPHA, 0);
    }
    //translate the prestore_rgn from hwnd to the coord of hmemdc
    OffsetRegion(prestore_rgn, x - ptoff->x, y - ptoff->y);
    //let the hmemdc to select it
    SelectClipRegion(hmemdc, prestore_rgn);
    //bitblt
    BitBlt(hdc_restore, rc.left - x, rc.top - y, RECTW(rc), RECTH(rc),
            hmemdc, rc.left - ptoff->x, rc.top - ptoff->y, 0);

    if(!bneedsubtract)
        SetMemDCAlpha(hdc_restore, 0, 0);

    //free
    if(prestore_rgn != prgn)
        DestroyClipRgn(prestore_rgn);
    if(have_secondary_dc)
        ReleaseSecondaryDC(hwnd, hdc_restore);
    else
        DeleteMemDC(hdc_restore);

}

HDC mgclBeginTransparentBkgnd(HWND hWnd, HDC hdc, int bkg_alpha, TRANSPARENTINFO *pinfo)
{
    PCLIPRGN prgn;
    RECT rcBound;
    HDC hmemdc = hdc;
    POINT ptoff = {0, 0};

    memset(pinfo, 0, sizeof(TRANSPARENTINFO));
    pinfo->howner = hdc;
    pinfo->htrans = HDC_INVALID;
    if(!pinfo)
        return hdc;

    if(bkg_alpha >= 255)
        return hdc;


    //If hdc is generic dc, not memory dc. We should create a temporary dc to
    //get background.
    if(WindowFromDC(hdc) != HWND_NULL) {
        hmemdc = CreateCompatibleDC(hdc);
        //_DBG_PRINTF ("\n enter WindowFromDC(hdc) != HWND_NULL  hmemdc is:%p \n\n" ,hmemdc);
    }

    if (hmemdc == HDC_INVALID)
        return hdc;

    pinfo->hmemdc = hmemdc;
    pinfo->bkg_alpha = bkg_alpha;
    pinfo->htrans = (HDC)0;

    prgn = CreateClipRgn();
    GetClipRegion(hdc, prgn);
    CopyRect(&rcBound, &prgn->rcBound);

    ClientToScreen(hWnd, &ptoff.x, &ptoff.y);
    OffsetRegion(prgn, ptoff.x, ptoff.y);

    SaveDC(hmemdc);
    //translate the prgn to screen coord
    update_under_window(GetNextMainWindow(hWnd), hmemdc, &prgn->rcBound, prgn, &ptoff);
    RestoreDC(hmemdc, -1);


    pinfo->htrans = CreateCompatibleDC(hmemdc);
    //if(!pinfo->htrans)
    if (HDC_INVALID == pinfo->htrans)
        goto end;

    //SetBrushColor(pinfo->htrans, RGBA2Pixel(pinfo->htrans, 0, 0, 0, 0));
    //FillBox(pinfo->htrans, 0, 0, 240, 320);

    GetClipRegion(hdc, prgn);
    SelectClipRegion(pinfo->htrans, prgn);

end:
    DestroyClipRgn(prgn);
    //return !pinfo->htrans ? hmemdc : pinfo->htrans;
    return HDC_INVALID == pinfo->htrans ? hmemdc : pinfo->htrans;
}

void mgclEndTransparentBkgnd(TRANSPARENTINFO *pinfo)
{
    //if(!pinfo || !pinfo->htrans)
    if(!pinfo || HDC_INVALID == pinfo->htrans)
        return;

    SetMemDCAlpha(pinfo->htrans, MEMDC_FLAG_SRCALPHA, pinfo->bkg_alpha);
    BitBlt(pinfo->htrans,0, 0, 0, 0, pinfo->hmemdc, 0, 0,0);
    DeleteMemDC(pinfo->htrans);

    if(pinfo->howner != pinfo->hmemdc) {
        BitBlt(pinfo->hmemdc, 0, 0, 0, 0,
                pinfo->howner, 0, 0, 0);
        DeleteMemDC(pinfo->hmemdc);
    }
}

} // namespace hfcl

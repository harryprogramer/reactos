/*
 * PROJECT:     PAINT for ReactOS
 * LICENSE:     LGPL
 * FILE:        base/applications/mspaint/resource.h
 * PURPOSE:     Defines the resource IDs
 * PROGRAMMERS: Benedikt Freisen
 */

#pragma once

/* DEFINES **********************************************************/

#define IDI_APPICON 500

#define IDB_TOOLBARICONS 510
#define IDB_FONTSTOOLBAR 511

#define IDI_TRANSPARENT    526
#define IDI_NONTRANSPARENT 527

#define IDC_FILL     530
#define IDC_COLOR    531
#define IDC_ZOOM     532
#define IDC_PEN      533
#define IDC_AIRBRUSH 534

#define IDI_HORZSTRETCH 535
#define IDI_VERTSTRETCH 536
#define IDI_HORZSKEW    537
#define IDI_VERTSKEW    538

#define ID_MENU 501

#define IDM_FILENEW    201
#define IDM_FILEOPEN   202
#define IDM_FILESAVE   203
#define IDM_FILESAVEAS 204

#define IDM_FILEFROMSCANNERORCAMERA 205

#define IDM_FILEPAGEVIEW  206
#define IDM_FILEPAGESETUP 207
#define IDM_FILEPRINT     208

#define IDM_FILESEND 209

#define IDM_FILEASWALLPAPERPLANE     210
#define IDM_FILEASWALLPAPERCENTERED  211
#define IDM_FILEASWALLPAPERSTRETCHED 212

#define IDM_FILEMOSTRECENTLYUSEDFILE 213
#define IDM_FILE1 214
#define IDM_FILE2 215
#define IDM_FILE3 216
#define IDM_FILE4 217

#define IDM_FILEEXIT 218

#define IDM_EDITUNDO            220
#define IDM_EDITREDO            221
#define IDM_EDITCUT             222
#define IDM_EDITCOPY            223
#define IDM_EDITPASTE           224
#define IDM_EDITDELETESELECTION 225
#define IDM_EDITINVERTSELECTION 226
#define IDM_EDITSELECTALL       227
#define IDM_EDITCOPYTO          228
#define IDM_EDITPASTEFROM       229

#define IDM_VIEWTOOLBOX       230
#define IDM_VIEWCOLORPALETTE  231
#define IDM_VIEWSTATUSBAR     232
#define IDM_FORMATICONBAR     233
#define IDM_VIEWZOOM125       234
#define IDM_VIEWZOOM25        235
#define IDM_VIEWZOOM50        236
#define IDM_VIEWZOOM100       237
#define IDM_VIEWZOOM200       238
#define IDM_VIEWZOOM400       239
#define IDM_VIEWZOOM800       240
#define IDM_VIEWFULLSCREEN    241
#define IDM_VIEWSHOWGRID      242
#define IDM_VIEWSHOWMINIATURE 243

#define IDM_IMAGEROTATEMIRROR 250
#define IDM_IMAGESTRETCHSKEW  251
#define IDM_IMAGECROP         252
#define IDM_IMAGEINVERTCOLORS 253
#define IDM_IMAGEATTRIBUTES   254
#define IDM_IMAGEDELETEIMAGE  255
#define IDM_IMAGEDRAWOPAQUE   256

#define IDM_COLORSEDITPALETTE   260
#define IDM_COLORSMODERNPALETTE 261
#define IDM_COLORSOLDPALETTE    262

#define IDM_HELPHELPTOPICS 270
#define IDM_HELPINFO       271

#define IDM_BOLD        280
#define IDM_ITALIC      281
#define IDM_UNDERLINE   282
#define IDM_VERTICAL    283

/* the following 16 numbers need to be in order, increasing by 1 */
#define ID_FREESEL  600
#define ID_RECTSEL  601
#define ID_RUBBER   602
#define ID_FILL     603
#define ID_COLOR    604
#define ID_ZOOM     605
#define ID_PEN      606
#define ID_BRUSH    607
#define ID_AIRBRUSH 608
#define ID_TEXT     609
#define ID_LINE     610
#define ID_BEZIER   611
#define ID_RECT     612
#define ID_SHAPE    613
#define ID_ELLIPSE  614
#define ID_RRECT    615
#define NUM_TOOLS (ID_RRECT - ID_FREESEL + 1)

#define ID_ACCELERATORS 800

#define IDD_MIRRORROTATE      700
#define IDD_MIRRORROTATEGROUP 701
#define IDD_MIRRORROTATERB1   702
#define IDD_MIRRORROTATERB2   703
#define IDD_MIRRORROTATERB3   704
#define IDD_MIRRORROTATERB4   705
#define IDD_MIRRORROTATERB5   706
#define IDD_MIRRORROTATERB6   707

#define IDD_ATTRIBUTES         710
#define IDD_ATTRIBUTESEDIT1    711
#define IDD_ATTRIBUTESEDIT2    712
#define IDD_ATTRIBUTESTEXT1    715
#define IDD_ATTRIBUTESTEXT2    716
#define IDD_ATTRIBUTESTEXT3    717
#define IDD_ATTRIBUTESTEXT4    718
#define IDD_ATTRIBUTESTEXT5    719
#define IDD_ATTRIBUTESTEXT6    720
#define IDD_ATTRIBUTESTEXT7    721
#define IDD_ATTRIBUTESTEXT8    722
#define IDD_ATTRIBUTESSTANDARD 723
#define IDD_ATTRIBUTESGROUP1   724
#define IDD_ATTRIBUTESGROUP2   725
#define IDD_ATTRIBUTESRB1      726
#define IDD_ATTRIBUTESRB2      727
#define IDD_ATTRIBUTESRB3      728
#define IDD_ATTRIBUTESRB4      729
#define IDD_ATTRIBUTESRB5      730

#define IDD_STRETCHSKEW             740
#define IDD_STRETCHSKEWGROUPSTRETCH 741
#define IDD_STRETCHSKEWICONHSTRETCH 742
#define IDD_STRETCHSKEWTEXTHSTRETCH 743
#define IDD_STRETCHSKEWEDITHSTRETCH 744
#define IDD_STRETCHSKEWTEXTHPERCENT 745
#define IDD_STRETCHSKEWICONVSTRETCH 746
#define IDD_STRETCHSKEWTEXTVSTRETCH 747
#define IDD_STRETCHSKEWEDITVSTRETCH 748
#define IDD_STRETCHSKEWTEXTVPERCENT 749
#define IDD_STRETCHSKEWGROUPSKEW    750
#define IDD_STRETCHSKEWICONHSKEW    751
#define IDD_STRETCHSKEWTEXTHSKEW    752
#define IDD_STRETCHSKEWEDITHSKEW    753
#define IDD_STRETCHSKEWTEXTHDEG     754
#define IDD_STRETCHSKEWICONVSKEW    755
#define IDD_STRETCHSKEWTEXTVSKEW    756
#define IDD_STRETCHSKEWEDITVSKEW    757
#define IDD_STRETCHSKEWTEXTVDEG     758

#define IDD_FONTS           760
#define IDD_FONTSNAMES      761
#define IDD_FONTSSIZES      762
#define IDD_FONTSTOOLBAR    764

#define IDS_PROGRAMNAME 900
#define IDS_WINDOWTITLE 901
#define IDS_INFOTITLE   902
#define IDS_INFOTEXT    903

#define IDS_SAVEPROMPTTEXT  904
#define IDS_DEFAULTFILENAME 905
#define IDS_MINIATURETITLE  906

#define IDS_TOOLTIP1  910
#define IDS_TOOLTIP2  911
#define IDS_TOOLTIP3  912
#define IDS_TOOLTIP4  913
#define IDS_TOOLTIP5  914
#define IDS_TOOLTIP6  915
#define IDS_TOOLTIP7  916
#define IDS_TOOLTIP8  917
#define IDS_TOOLTIP9  918
#define IDS_TOOLTIP10 919
#define IDS_TOOLTIP11 920
#define IDS_TOOLTIP12 921
#define IDS_TOOLTIP13 922
#define IDS_TOOLTIP14 923
#define IDS_TOOLTIP15 924
#define IDS_TOOLTIP16 925

#define IDS_ALLFILES        926
#define IDS_ALLPICTUREFILES 927
#define IDS_FILESIZE        928

#define IDS_INTNUMBERS 930
#define IDS_PERCENTAGE 931
#define IDS_ANGLE      932

#define IDS_LOADERRORTEXT 933
#define IDS_ENLARGEPROMPTTEXT 934

#define IDS_BOLD        935
#define IDS_ITALIC      936
#define IDS_UNDERLINE   937
#define IDS_VERTICAL    938
#define IDS_PRINTRES    939
#define IDS_CANTPASTE   940

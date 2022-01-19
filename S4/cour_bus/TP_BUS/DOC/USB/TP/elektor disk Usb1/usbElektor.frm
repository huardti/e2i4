VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "USB Thermometer + Ports"
   ClientHeight    =   5490
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4200
   FillColor       =   &H00E0E0E0&
   ForeColor       =   &H8000000B&
   LinkTopic       =   "Form1"
   ScaleHeight     =   5490
   ScaleWidth      =   4200
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer Timer2 
      Interval        =   1000
      Left            =   0
      Top             =   1560
   End
   Begin VB.HScrollBar HScroll2 
      Height          =   255
      Left            =   840
      Max             =   15
      TabIndex        =   14
      Top             =   720
      Value           =   7
      Width           =   2055
   End
   Begin VB.Frame Frame4 
      Caption         =   "Port 1"
      Height          =   615
      Left            =   2280
      TabIndex        =   11
      Top             =   2760
      Width           =   1335
      Begin VB.Label Label10 
         Caption         =   "Label10"
         Height          =   255
         Left            =   240
         TabIndex        =   13
         Top             =   240
         Width           =   855
      End
   End
   Begin VB.Frame Frame3 
      Caption         =   "Port 0"
      Height          =   615
      Left            =   600
      TabIndex        =   10
      Top             =   2760
      Width           =   1335
      Begin VB.Label Label9 
         Caption         =   "Label9"
         Height          =   255
         Left            =   240
         TabIndex        =   12
         Top             =   240
         Width           =   855
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Button"
      Height          =   615
      Left            =   2280
      TabIndex        =   8
      Top             =   1800
      Width           =   1335
      Begin VB.Label Label8 
         Caption         =   "Label8"
         Height          =   255
         Left            =   240
         TabIndex        =   9
         Top             =   240
         Width           =   615
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Temperature"
      Height          =   615
      Left            =   600
      TabIndex        =   6
      Top             =   1800
      Width           =   1455
      Begin VB.Label Label7 
         Caption         =   "Label7"
         Height          =   255
         Left            =   360
         TabIndex        =   7
         Top             =   240
         Width           =   855
      End
   End
   Begin VB.HScrollBar HScroll3 
      Height          =   255
      Left            =   840
      Max             =   15
      TabIndex        =   1
      Top             =   1200
      Value           =   7
      Width           =   2055
   End
   Begin VB.HScrollBar HScroll1 
      Height          =   255
      Left            =   840
      Max             =   31
      TabIndex        =   0
      Top             =   240
      Value           =   15
      Width           =   2055
   End
   Begin VB.Timer Timer1 
      Interval        =   200
      Left            =   3600
      Top             =   600
   End
   Begin VB.Label Label11 
      Caption         =   "Label11"
      Height          =   375
      Left            =   960
      TabIndex        =   17
      Top             =   4200
      Width           =   1455
   End
   Begin VB.Label Label5 
      Caption         =   "7"
      Height          =   255
      Left            =   3120
      TabIndex        =   16
      Top             =   720
      Width           =   375
   End
   Begin VB.Label Label2 
      Caption         =   "P1"
      Height          =   255
      Left            =   360
      TabIndex        =   15
      Top             =   720
      Width           =   255
   End
   Begin VB.Label Label6 
      Caption         =   "7"
      Height          =   375
      Left            =   3120
      TabIndex        =   5
      Top             =   1200
      Width           =   495
   End
   Begin VB.Label Label4 
      Caption         =   "255"
      Height          =   255
      Left            =   3120
      TabIndex        =   4
      Top             =   240
      Width           =   495
   End
   Begin VB.Label Label3 
      Caption         =   "LED"
      Height          =   255
      Left            =   360
      TabIndex        =   3
      Top             =   1200
      Width           =   375
   End
   Begin VB.Label Label1 
      Caption         =   "P0"
      Height          =   255
      Left            =   360
      TabIndex        =   2
      Top             =   240
      Width           =   255
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim sFileName As String
Dim htemp As Long
Dim lIn As Long, lInSize As Long, lOut As Long, lOutSize As Long, lSize As Long
Dim lTemp As Long
Sub USB_IO()
  sFileName = "\\.\Thermometer_0"
  hgDrvrHnd = CreateFile(sFileName, GENERIC_WRITE Or GENERIC_READ, FILE_SHARE_WRITE Or FILE_SHARE_READ, Security, OPEN_EXISTING, 0, 0)
  lTemp = DeviceIoControl(hgDrvrHnd, 4&, lIn, lInSize, lOut, lOutSize, lSize, gOverlapped)
  htemp = CloseHandle(hgDrvrHnd)
End Sub
Sub Brightness(Level)
  lIn = Level * 256 + 14
  lInSize = 2
  lOutSize = 1
  USB_IO
End Sub
Function RdPort(Port) As Integer
  lIn = Port * 256 + 20
  lInSize = 2
  lOutSize = 2
  USB_IO
  RdPort = (lOut / 256) And 255
End Function
Sub WrRAM(Adresse, Wert)
  lIn = 65536 * Wert + Adresse * 256 + 23
  lInSize = 3
  lOutSize = 1
  USB_IO
End Sub
Private Sub HScroll1_Change()
  Wert = HScroll1.Value * 8 + 7
  WrRAM 46, Wert
  Label4.Caption = Str$(Wert)
End Sub
Private Sub HScroll2_Change()
  WrRAM 47, HScroll2.Value
  Label5.Caption = HScroll2.Value
End Sub

Private Sub HScroll3_Change()
  Brightness HScroll3.Value
  Label6 = HScroll3.Value
End Sub

Private Sub Timer1_Timer()
  lIn = 11
  lInSize = 1
  lOutSize = 3
  USB_IO
  Temp = ((lOut \ 256) And 255) / 2
  Minus = (lOut \ 65536) And 255
  If Minus > 0 Then Temp = Temp * -1
  Button = (lOut \ 16777216) And 255
  Label7.Caption = Str$(Temp)
  Label8.Caption = Str$(Button)
  Label9.Caption = Str$(RdPort(0))
  Label10.Caption = Str$((RdPort(1)) And 15)
End Sub


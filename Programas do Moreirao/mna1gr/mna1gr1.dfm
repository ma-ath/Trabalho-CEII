object Form1: TForm1
  Left = 243
  Top = 124
  Width = 696
  Height = 480
  Caption = 'Mna1gr'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 680
    Height = 422
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    Lines.Strings = (
      'Programa demonstrativo de analise nodal modificada'
      'Por Antonio Carlos M. de Queiroz - acmq@ufrj.br')
    ParentFont = False
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object OpenDialog1: TOpenDialog
    FileName = 'teste.net'
    Filter = 'netlists|*.net'
    Options = [ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
    Left = 632
    Top = 16
  end
  object MainMenu1: TMainMenu
    Left = 88
    Top = 16
    object Arquivo1: TMenuItem
      Caption = 'Arquivo'
      object Abrir1: TMenuItem
        Caption = 'Abrir'
        OnClick = Abrir1Click
      end
    end
    object Opes1: TMenuItem
      Caption = 'Op'#231#245'es'
      object Mostrarestampas1: TMenuItem
        AutoCheck = True
        Caption = 'Mostrar estampas'
        Checked = True
      end
    end
  end
end

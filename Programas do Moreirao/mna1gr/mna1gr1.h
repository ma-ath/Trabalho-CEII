//---------------------------------------------------------------------------

#ifndef mna1gr1H
#define mna1gr1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TOpenDialog *OpenDialog1;
        TMemo *Memo1;
        TMainMenu *MainMenu1;
        TMenuItem *Arquivo1;
        TMenuItem *Abrir1;
        TMenuItem *Opes1;
        TMenuItem *Mostrarestampas1;
        void __fastcall Abrir1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

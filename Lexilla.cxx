// Lexilla lexer library
/** @file Lexilla.cxx
 ** Lexer infrastructure.
 ** Provides entry points to shared library.
 **/
// Copyright 2019 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be
// distributed.

#include <cstring>

#include <vector>
#include <initializer_list>

#if defined(_WIN32)
#define EXPORT_FUNCTION __declspec(dllexport)
#define CALLING_CONVENTION __stdcall
#else
#define EXPORT_FUNCTION __attribute__((visibility("default")))
#define CALLING_CONVENTION
#endif

#include "ILexer.h"

#include "LexerModule.h"
#include "Lexilla.h"
#include "CatalogueModules.h"

using namespace Lexilla;

//++Autogenerated -- run lexilla/scripts/LexillaGen.py to regenerate
//**\(extern const LexerModule \*;\n\)
extern const LexerModule lmA68k;
extern const LexerModule lmAbaqus;
extern const LexerModule lmAda;
extern const LexerModule lmAPDL;
extern const LexerModule lmAs;
extern const LexerModule lmAsciidoc;
extern const LexerModule lmAsm;
extern const LexerModule lmAsn1;
extern const LexerModule lmASY;
extern const LexerModule lmAU3;
extern const LexerModule lmAVE;
extern const LexerModule lmAVS;
extern const LexerModule lmBaan;
extern const LexerModule lmBash;
extern const LexerModule lmBatch;
extern const LexerModule lmBibTeX;
extern const LexerModule lmBlitzBasic;
extern const LexerModule lmBullant;
extern const LexerModule lmCaml;
extern const LexerModule lmCIL;
extern const LexerModule lmClw;
extern const LexerModule lmClwNoCase;
extern const LexerModule lmCmake;
extern const LexerModule lmCOBOL;
extern const LexerModule lmCoffeeScript;
extern const LexerModule lmConf;
extern const LexerModule lmCPP;
extern const LexerModule lmCPPNoCase;
extern const LexerModule lmCsound;
extern const LexerModule lmCss;
extern const LexerModule lmD;
extern const LexerModule lmDart;
extern const LexerModule lmDataflex;
extern const LexerModule lmDiff;
extern const LexerModule lmDMAP;
extern const LexerModule lmDMIS;
extern const LexerModule lmECL;
extern const LexerModule lmEDIFACT;
extern const LexerModule lmEiffel;
extern const LexerModule lmEiffelkw;
extern const LexerModule lmErlang;
extern const LexerModule lmErrorList;
extern const LexerModule lmESCRIPT;
extern const LexerModule lmF77;
extern const LexerModule lmFlagShip;
extern const LexerModule lmForth;
extern const LexerModule lmFortran;
extern const LexerModule lmFreeBasic;
extern const LexerModule lmFSharp;
extern const LexerModule lmGAP;
extern const LexerModule lmGDScript;
extern const LexerModule lmGui4Cli;
extern const LexerModule lmHaskell;
extern const LexerModule lmHollywood;
extern const LexerModule lmHTML;
extern const LexerModule lmIHex;
extern const LexerModule lmIndent;
extern const LexerModule lmInno;
extern const LexerModule lmJSON;
extern const LexerModule lmJulia;
extern const LexerModule lmKix;
extern const LexerModule lmKVIrc;
extern const LexerModule lmLatex;
extern const LexerModule lmLISP;
extern const LexerModule lmLiterateHaskell;
extern const LexerModule lmLot;
extern const LexerModule lmLout;
extern const LexerModule lmLua;
extern const LexerModule lmMagikSF;
extern const LexerModule lmMake;
extern const LexerModule lmMarkdown;
extern const LexerModule lmMatlab;
extern const LexerModule lmMaxima;
extern const LexerModule lmMETAPOST;
extern const LexerModule lmMMIXAL;
extern const LexerModule lmModula;
extern const LexerModule lmMSSQL;
extern const LexerModule lmMySQL;
extern const LexerModule lmNim;
extern const LexerModule lmNimrod;
extern const LexerModule lmNix;
extern const LexerModule lmNncrontab;
extern const LexerModule lmNsis;
extern const LexerModule lmNull;
extern const LexerModule lmOctave;
extern const LexerModule lmOpal;
extern const LexerModule lmOScript;
extern const LexerModule lmPascal;
extern const LexerModule lmPB;
extern const LexerModule lmPerl;
extern const LexerModule lmPHPSCRIPT;
extern const LexerModule lmPLM;
extern const LexerModule lmPO;
extern const LexerModule lmPOV;
extern const LexerModule lmPowerPro;
extern const LexerModule lmPowerShell;
extern const LexerModule lmProgress;
extern const LexerModule lmProps;
extern const LexerModule lmPS;
extern const LexerModule lmPureBasic;
extern const LexerModule lmPython;
extern const LexerModule lmR;
extern const LexerModule lmRaku;
extern const LexerModule lmREBOL;
extern const LexerModule lmRegistry;
extern const LexerModule lmRuby;
extern const LexerModule lmRust;
extern const LexerModule lmSAS;
extern const LexerModule lmScriptol;
extern const LexerModule lmSmalltalk;
extern const LexerModule lmSML;
extern const LexerModule lmSorc;
extern const LexerModule lmSpecman;
extern const LexerModule lmSpice;
extern const LexerModule lmSQL;
extern const LexerModule lmSrec;
extern const LexerModule lmStata;
extern const LexerModule lmSTTXT;
extern const LexerModule lmTACL;
extern const LexerModule lmTADS3;
extern const LexerModule lmTAL;
extern const LexerModule lmTCL;
extern const LexerModule lmTCMD;
extern const LexerModule lmTEHex;
extern const LexerModule lmTeX;
extern const LexerModule lmTOML;
extern const LexerModule lmTroff;
extern const LexerModule lmTxt2tags;
extern const LexerModule lmVB;
extern const LexerModule lmVBScript;
extern const LexerModule lmVerilog;
extern const LexerModule lmVHDL;
extern const LexerModule lmVisualProlog;
extern const LexerModule lmX12;
extern const LexerModule lmXML;
extern const LexerModule lmYAML;
extern const LexerModule lmZig;

//--Autogenerated -- end of automatically generated section

namespace {

CatalogueModules catalogueLexilla;

void AddEachLexer() {

  if (catalogueLexilla.Count() > 0) {
    return;
  }

  catalogueLexilla.AddLexerModules(std::vector<const LexerModule *>{
      //++Autogenerated -- run scripts/LexillaGen.py to regenerate
      //**\(\t\t&\*,\n\)
      &lmA68k,       &lmAbaqus,     &lmAda,          &lmAPDL,      &lmAs,
      &lmAsciidoc,   &lmAsm,        &lmAsn1,         &lmASY,       &lmAU3,
      &lmAVE,        &lmAVS,        &lmBaan,         &lmBash,      &lmBatch,
      &lmBibTeX,     &lmBlitzBasic, &lmBullant,      &lmCaml,      &lmCIL,
      &lmClw,        &lmClwNoCase,  &lmCmake,        &lmCOBOL,     &lmCoffeeScript,
      &lmConf,       &lmCPP,        &lmCPPNoCase,    &lmCsound,    &lmCss,
      &lmD,          &lmDart,       &lmDataflex,     &lmDiff,      &lmDMAP,
      &lmDMIS,       &lmECL,        &lmEDIFACT,      &lmEiffel,    &lmEiffelkw,
      &lmErlang,     &lmErrorList,  &lmESCRIPT,      &lmF77,       &lmFlagShip,
      &lmForth,      &lmFortran,    &lmFreeBasic,    &lmFSharp,    &lmGAP,
      &lmGDScript,   &lmGui4Cli,    &lmHaskell,      &lmHollywood, &lmHTML,
      &lmIHex,       &lmIndent,     &lmInno,         &lmJSON,      &lmJulia,
      &lmKix,        &lmKVIrc,      &lmLatex,        &lmLISP,      &lmLiterateHaskell,
      &lmLot,        &lmLout,       &lmLua,          &lmMagikSF,   &lmMake,
      &lmMarkdown,   &lmMatlab,     &lmMaxima,       &lmMETAPOST,  &lmMMIXAL,
      &lmModula,     &lmMSSQL,      &lmMySQL,        &lmNim,       &lmNimrod,
      &lmNix,        &lmNncrontab,  &lmNsis,         &lmNull,      &lmOctave,
      &lmOpal,       &lmOScript,    &lmPascal,       &lmPB,        &lmPerl,
      &lmPHPSCRIPT,  &lmPLM,        &lmPO,           &lmPOV,       &lmPowerPro,
      &lmPowerShell, &lmProgress,   &lmProps,        &lmPS,        &lmPureBasic,
      &lmPython,     &lmR,          &lmRaku,         &lmREBOL,     &lmRegistry,
      &lmRuby,       &lmRust,       &lmSAS,          &lmScriptol,  &lmSmalltalk,
      &lmSML,        &lmSorc,       &lmSpecman,      &lmSpice,     &lmSQL,
      &lmSrec,       &lmStata,      &lmSTTXT,        &lmTACL,      &lmTADS3,
      &lmTAL,        &lmTCL,        &lmTCMD,         &lmTEHex,     &lmTeX,
      &lmTOML,       &lmTroff,      &lmTxt2tags,     &lmVB,        &lmVBScript,
      &lmVerilog,    &lmVHDL,       &lmVisualProlog, &lmX12,       &lmXML,
      &lmYAML,       &lmZig,

      //--Autogenerated -- end of automatically generated section
  });
}

} // namespace

extern "C" {

EXPORT_FUNCTION int CALLING_CONVENTION GetLexerCount() {
  AddEachLexer();
  return static_cast<int>(catalogueLexilla.Count());
}

EXPORT_FUNCTION void CALLING_CONVENTION GetLexerName(unsigned int index, char *name,
                                                     int buflength) {
  AddEachLexer();
  *name = 0;
  const char *lexerName = catalogueLexilla.Name(index);
  if (static_cast<size_t>(buflength) > strlen(lexerName)) {
    strcpy(name, lexerName);
  }
}

EXPORT_FUNCTION LexerFactoryFunction CALLING_CONVENTION
GetLexerFactory(unsigned int index) {
  AddEachLexer();
  return catalogueLexilla.Factory(index);
}

EXPORT_FUNCTION Scintilla::ILexer5 *CALLING_CONVENTION CreateLexer(const char *name) {
  AddEachLexer();
  for (size_t i = 0; i < catalogueLexilla.Count(); i++) {
    const char *lexerName = catalogueLexilla.Name(i);
    if (0 == strcmp(lexerName, name)) {
      return catalogueLexilla.Create(i);
    }
  }
  return nullptr;
}

EXPORT_FUNCTION const char *CALLING_CONVENTION LexerNameFromID(int identifier) {
  AddEachLexer();
  const LexerModule *pModule = catalogueLexilla.Find(identifier);
  if (pModule) {
    return pModule->languageName;
  }
  return nullptr;
}

EXPORT_FUNCTION const char *CALLING_CONVENTION GetLibraryPropertyNames() { return ""; }

EXPORT_FUNCTION void CALLING_CONVENTION SetLibraryProperty(const char *, const char *) {
  // Null implementation
}

EXPORT_FUNCTION const char *CALLING_CONVENTION GetNameSpace() { return "lexilla"; }
}

// Not exported from binary as LexerModule must be built exactly the same as
// modules listed above
void AddStaticLexerModule(const LexerModule *plm) {
  AddEachLexer();
  catalogueLexilla.AddLexerModule(plm);
}

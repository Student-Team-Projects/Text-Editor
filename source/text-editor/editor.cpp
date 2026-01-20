#include <text-editor/editor.hpp>
#include <iostream>
#include <fstream>
#include <scintilla/lexlib/LexerModule.h>

extern Scintilla::LexerModule lmCPP;
extern Scintilla::LexerModule lmPython;
extern Scintilla::LexerModule lmJSON;
extern Scintilla::LexerModule lmBash;
extern Scintilla::LexerModule lmPascal;

Editor::Editor(){
    scintilla.setParent(this);
    //configure margins
    turbo::call(scintilla, SCI_SETMARGINS, 0, 0);

    // Indentation
    turbo::call(scintilla, SCI_SETUSETABS, false, 0U);
    turbo::call(scintilla, SCI_SETINDENT, 4, 0U);
    turbo::call(scintilla, SCI_SETTABWIDTH, 4, 0U);
    turbo::call(scintilla, SCI_SETTABINDENTS, true, 0U);
    turbo::call(scintilla, SCI_SETBACKSPACEUNINDENTS, true, 0U);
}

void Editor::setSize(const TPoint& size){
    this->size = size;
}

void Editor::paint(TDrawSurface& surface, TRect area){
    turbo::paint(scintilla, surface, area);
}

void Editor::handleEvent(TEvent& event){
    if(event.what == evCommand){
        if(event.message.command == cmSave){
            std::cerr << "Save\n";
            saveFile();
        }
    }else if(event.what == evKeyboard){
        turbo::handleKeyDown(scintilla, event.keyDown);
    }else if(event.what & evMouse){
        if(event.what == evMouseWheel){
            int direction = (event.mouse.wheel == mwUp) ? -1 : 1;
            turbo::call(scintilla, SCI_LINESCROLL, 0, direction);
        }else{
            turbo::handleMouse(scintilla, event.what, event.mouse);
        }
    }
}

TPoint Editor::pointMainCaret(){
    return turbo::pointMainCaret(scintilla);
}

void Editor::addObserver(EditorObserver* observer){
    observers.insert(observer);
}

void Editor::removeObserver(EditorObserver* observer){
    observers.erase(observer);
}

TPoint Editor::getEditorSize() noexcept{
    return size;
}

void Editor::invalidate(TRect area) noexcept{
    // may be useful later for understanding scintilla events
    // std::cerr << "invalidate " << area.a.x << " " << area.a.y << " " << area.b.x << " " << area.b.y << "\n";
    updateAll();
}

void Editor::handleNotification(const SCNotification &scn){
    // std::cerr << "handleNotification\n";
    // updateAll();
}

void Editor::setVerticalScrollPos(int delta, int limit) noexcept{
    // std::cerr << "setVerticalScrollPos " << delta << " " << limit << "\n";
    updateAll();
}

void Editor::setHorizontalScrollPos(int delta, int limit) noexcept{
    // std::cerr << "setHorizontalScrollPos " << delta << " " << limit << "\n";
    updateAll();
}

static char buffer alignas(4*1024) [128*1024 + 1024];
long buffSize = 128*1024;


static const std::unordered_map<std::string, Language> ext2lang = {
    {".c",   Language::CPP},
    {".cc",  Language::CPP},
    {".cpp", Language::CPP},
    {".h",   Language::CPP},

    {".py",  Language::Python},

    {".json",Language::JSON},

    {".sh",  Language::Bash},

    {".pas", Language::Pascal}
};

Language detectFileLanguage(const std::string &path)
{
    size_t pos = path.find_last_of('.');
    if (pos != std::string::npos)
    {
        std::string ext = path.substr(pos);
        auto it = ext2lang.find(ext);
        if (it != ext2lang.end())
            return it->second;
    }

    size_t slash = path.find_last_of("/\\");
    std::string name = (slash == std::string::npos) ? path : path.substr(slash+1);

    auto it2 = ext2lang.find(name);
    if (it2 != ext2lang.end())
        return it2->second;

    return Language::None;
}

Scintilla::ILexer5 *getLexerForLanguage(Language lang)
{
    switch(lang)
    {
        case Language::CPP:     return lmCPP.Create();
        case Language::Python:  return lmPython.Create();
        case Language::JSON:    return lmJSON.Create();
        case Language::Bash:    return lmBash.Create();
        case Language::Pascal:    return lmPascal.Create();
        default: return nullptr;
    }
}

void Editor::openFile(const std::string& path){
    this->path = path;

    Language language = detectFileLanguage(path);
    configureStyling(language);
}

void Editor::readFile(){
    turbo::call(scintilla, SCI_CLEARALL, 0, 0);
    std::ifstream f(path, std::ios::in | std::ios::binary);
    //move input position indicator to end of file
    f.seekg(0, std::ios_base::end);
    long totalBytes = f.tellg(); 
    long allocBytes = totalBytes + 1000; //extra 1000 bytes like SciTE and turbo
    turbo::call(scintilla, SCI_ALLOCATE, allocBytes, 0);
    
    long remainingBytes = totalBytes;
    f.seekg(0, std::ios_base::beg);
    while(remainingBytes > 0){
        int readBytes = std::min(remainingBytes, buffSize); 
        f.read(buffer, readBytes);
        turbo::call(scintilla, SCI_APPENDTEXT, readBytes, (sptr_t)buffer);
        remainingBytes -= readBytes;
    }
    turbo::call(scintilla, SCI_EMPTYUNDOBUFFER, 0, 0);
    turbo::call(scintilla, SCI_SETSAVEPOINT, 0, 0);
    updateAll();
}

void Editor::saveFile(){
    std::ofstream f(path, std::ios::out | std::ios::binary);
    long totalBytes = turbo::call(scintilla, SCI_GETLENGTH, 0, 0);
    long pos = 0;
    while(pos < totalBytes){
        long endpos = std::min(totalBytes, pos + buffSize);
        Sci_TextRange range{Sci_CharacterRange{pos, endpos}, buffer};
        turbo::call(scintilla, SCI_GETTEXTRANGE, 0, (sptr_t)&range);
        long bytesRead = endpos - pos;
        f.write(buffer, bytesRead);
        pos += bytesRead;
    }
    turbo::call(scintilla, SCI_SETSAVEPOINT, 0, 0);
}

bool Editor::isModified(){
    return turbo::call(scintilla, SCI_GETMODIFY, 0, 0);
}

void Editor::updateAll(){
    for(auto observer : observers)observer->editorUpdate();
}

constexpr std::array<int, 4> stylesC[] =
{
    {SCE_C_DEFAULT,                 0,  0xffffff, 0x000000},
    {SCE_C_COMMENT,                 10, 0xff0000, 0x000000},
    {SCE_C_COMMENTLINE,             10, 0xff0000, 0x222222},
    {SCE_C_COMMENTDOC,              10, 0xff0000, 0x0000FF},
    {SCE_C_NUMBER,                  13, 0x00ff00, 0x000000},
    {SCE_C_WORD,                    5,  0x00ffff, 0x000000},
    {SCE_C_STRING,                  11, 0xffff00, 0x000000},
    {SCE_C_CHARACTER,               12, 0x0000ff, 0x000000},
    {SCE_C_PREPROCESSOR,            8,  0xff00ff, 0x000000},
    {SCE_C_OPERATOR,                9,  0xaa00bb, 0x000000},
    {SCE_C_COMMENTLINEDOC,          10, 0xff0000, 0x00FFFF},
    {SCE_C_WORD2,                   6,  0x000fff, 0x000000},
    {SCE_C_GLOBALCLASS,             7,  0xf0ab0f, 0x000000},
    {SCE_C_PREPROCESSORCOMMENT,     10, 0xf888ff, 0x000000},
    {SCE_C_PREPROCESSORCOMMENTDOC,  10, 0xf2ff80, 0x000000},
    {SCE_C_ESCAPESEQUENCE,          14, 0x8888ff, 0x000000},
};

std::pair<int, std::string> keywordsC[] =
{
    {0,
"alignas alignof and and_eq asm auto bitand bitor break case catch class compl "
"concept consteval constexpr constinit const_cast continue co_await co_return "
"co_yield decltype default delete do dynamic_cast else enum explicit export "
"false final for friend goto if import inline module namespace new noexcept not "
"not_eq nullptr operator or or_eq override private protected public "
"reinterpret_cast return sizeof static_assert static_cast struct switch "
"template this throw true try typedef typeid typename union using virtual while "
"xor xor_eq "
    },
    {1,
"bool char char8_t char16_t char32_t const double extern float int long mutable "
"register static short signed unsigned thread_local void volatile wchar_t int8_t "
"uint8_t int16_t uint16_t int32_t uint32_t int64_t uint64_t size_t ptrdiff_t "
"intptr_t uintptr_t far near uchar ushort uint ulong "
    },
    {3,
"std"
    },
};

std::pair<std::string, std::string> propertiesC[] =
{
    {"styling.within.preprocessor",         "1"},
    {"lexer.cpp.track.preprocessor",        "0"},
    {"lexer.cpp.escape.sequence",           "1"},
};


constexpr std::array<int, 4> stylesPython[] =
{
    {SCE_P_DEFAULT,        0xFFFFFF, 0x000000},
    {SCE_P_COMMENTLINE,    0x008000, 0x000000},
    {SCE_P_NUMBER,         0x00FF00, 0x000000},
    {SCE_P_STRING,         0xFFFF00, 0x000000},
    {SCE_P_CHARACTER,      0x0000FF, 0x000000},
    {SCE_P_WORD,           0x0000FF, 0x000000},
    {SCE_P_TRIPLE,         0xFFFF00, 0x000000},
    {SCE_P_TRIPLEDOUBLE,   0xFFFF00, 0x000000},
    {SCE_P_CLASSNAME,      0x00FFFF, 0x000000},
    {SCE_P_DEFNAME,        0x00FFFF, 0x000000},
    {SCE_P_OPERATOR,       0xAA00BB, 0x000000},
    {SCE_P_IDENTIFIER,     0xFFFFFF, 0x000000},
    {SCE_P_COMMENTBLOCK,   0x008000, 0x000000},
    {SCE_P_STRINGEOL,      0xFFFF00, 0x000000},
    {SCE_P_WORD2,          0x00AAAA, 0x000000},
    {SCE_P_DECORATOR,      0xFF8800, 0x000000},
    {SCE_P_FSTRING,        0xFFFF00, 0x000000},
    {SCE_P_FCHARACTER,     0x0000FF, 0x000000},
    {SCE_P_FTRIPLE,        0xFFFF00, 0x000000},
    {SCE_P_FTRIPLEDOUBLE,  0xFFFF00, 0x000000}
};

std::pair<int, std::string> keywordsPython[] =
{
    {0,
"and as assert break class continue def del elif else except exec finally for "
"from global if import in is lambda not or pass print raise return try while "
"with yield"
    },
    {1,
"int float complex list tuple range str bytes bytearray memoryview set frozenset "
"dict "
    },
};

std::pair<std::string, std::string> propertiesPython[] =
{
    {"lexer.python.keywords2.no.sub.identifiers",       "1"},
};

constexpr std::array<int, 4> stylesJSON[] =
{
    {SCE_JSON_DEFAULT,        0, 0xFFFFFF, 0x000000},
    {SCE_JSON_NUMBER,         13, 0x00FF00, 0x000000},
    {SCE_JSON_STRING,         11, 0xFFFF00, 0x000000},
    {SCE_JSON_STRINGEOL,      11, 0xFFFF00, 0x000000},
    {SCE_JSON_PROPERTYNAME,   8,  0xFF8800, 0x000000},
    {SCE_JSON_ESCAPESEQUENCE, 14, 0x8888FF, 0x000000},
    {SCE_JSON_LINECOMMENT,    10, 0x008000, 0x000000},
    {SCE_JSON_BLOCKCOMMENT,   10, 0x008000, 0x000000},
    {SCE_JSON_OPERATOR,       9,  0xAA00BB, 0x000000},
    {SCE_JSON_URI,            11, 0x0000FF, 0x000000},
    {SCE_JSON_COMPACTIRI,     6,  0x00AAAA, 0x000000},
    {SCE_JSON_KEYWORD,        5,  0x0000FF, 0x000000},
    {SCE_JSON_LDKEYWORD,      6,  0x00AAAA, 0x000000},
    {SCE_JSON_ERROR,          15, 0xFF0000, 0x000000},
};

std::pair<int, std::string> keywordsJSON[] =
{
    {0, "false true null"},
    {1,
"@id @context @type @value @language @container @list @set @reverse @index "
"@base @vocab @graph "
    },
};

std::pair<std::string, std::string> propertiesJSON[] =
{
    {"lexer.json.escape.sequence", "1"},
    {"lexer.json.allow.comments", "1"},
};

constexpr std::array<int, 4> stylesBash[] =
{
    {SCE_SH_DEFAULT,      0, 0xFFFFFF, 0x000000},
    {SCE_SH_ERROR,        15, 0xFF0000, 0x000000},
    {SCE_SH_COMMENTLINE,  10, 0x008000, 0x000000},
    {SCE_SH_NUMBER,       13, 0x00FF00, 0x000000},
    {SCE_SH_WORD,         6,  0x0000FF, 0x000000},
    {SCE_SH_STRING,       11, 0xFFFF00, 0x000000},
    {SCE_SH_CHARACTER,    12, 0x0000FF, 0x000000},
    {SCE_SH_OPERATOR,     9,  0xAA00BB, 0x000000},
    {SCE_SH_IDENTIFIER,   0,  0xFFFFFF, 0x000000},
    {SCE_SH_SCALAR,       5,  0xFF8800, 0x000000},
    {SCE_SH_PARAM,        5,  0xFF8800, 0x000000},
    {SCE_SH_BACKTICKS,    5,  0xFF8800, 0x000000},
    {SCE_SH_HERE_DELIM,   7,  0xAAAAAA, 0x000000},
    {SCE_SH_HERE_Q,       7,  0xAAAAAA, 0x000000},
};

std::pair<int, std::string> keywordsBash[] =
{
    {0,
// Keywords
"case do done elif else esac fi for function if in select then time until while "
// Builtins
"alias bg bind break builtin caller cd command compgen complete compopt continue "
"declare dirs disown echo enable eval exec exit export fc fg getopts hash help "
"history jobs kill let local logout mapfile popd printf pushd pwd read readarray "
"readonly return set shift shopt source suspend test times trap type typeset ulimit "
"umask unalias unset wait "
    },
};

constexpr std::array<int, 4> stylesPascal[] =
{
    {SCE_PAS_DEFAULT,       0, 0xFFFFFF, 0x000000},
    {SCE_PAS_IDENTIFIER,    0, 0xFFFFFF, 0x000000},
    {SCE_PAS_COMMENT,       10, 0x008000, 0x000000},
    {SCE_PAS_COMMENT2,      10, 0x008000, 0x000000},
    {SCE_PAS_COMMENTLINE,   10, 0x008000, 0x000000},
    {SCE_PAS_PREPROCESSOR,  8,  0xFF00FF, 0x000000},
    {SCE_PAS_PREPROCESSOR2, 8,  0xFF00FF, 0x000000},
    {SCE_PAS_NUMBER,        13, 0x00FF00, 0x000000},
    {SCE_PAS_HEXNUMBER,     13, 0x00FF00, 0x000000},
    {SCE_PAS_WORD,          5,  0x0000FF, 0x000000},
    {SCE_PAS_STRING,        11, 0xFFFF00, 0x000000},
    {SCE_PAS_STRINGEOL,     11, 0xFFFF00, 0x000000},
    {SCE_PAS_CHARACTER,     12, 0x0000FF, 0x000000},
    {SCE_PAS_OPERATOR,      9,  0xAA00BB, 0x000000},
    {SCE_PAS_ASM,           7,  0xAAAAAA, 0x000000}
};

std::pair<int, std::string> keywordsPascal[] =
{
    {0,
// Pascal
"and array asm begin break case const constructor continue destructor div do "
"downto else end false file for function goto if implementation in inline interface "
"label mod nil not object of on operator or packed procedure program record "
"repeat set shl shr string then to true type unit until uses var while with xor "

"as class constref dispose except exit exports finalization finally inherited "
"initialization is library new on out property raise self threadvar try far near "

"absolute abstract alias assembler cdecl Cppdecl default export external forward "
"generic index local name nostackframe oldfpccall override pascal private protected "
"public published read register reintroduce safecall softfloat specialize stdcall "
"virtual write "

// Delphi
"resourcestring dispinterface strict nodefault stored automated final readonly "
"unsafe reference varargs contains helper overload implements winapi delayed package "
"requires deprecated resident writeonly dispid platform dynamic sealed experimental "
"message static "
    }
};

void Editor::configureStyling(Language language){
    //Style configuration
    TColorAttr textColor = {0xFFFFFF, 0x000000};
    turbo::setStyleColor(scintilla, STYLE_DEFAULT, textColor);
    turbo::call(scintilla, SCI_STYLECLEARALL, 0, 0);

    TColorAttr selectionColor = {0xFFFFFF, 0x444466};
    turbo::setSelectionColor(scintilla, selectionColor);

    Scintilla::ILexer5 *lexer = getLexerForLanguage(language);

    if(!lexer) {
        turbo::call(scintilla, SCI_SETLEXER, SCLEX_NULL, 0);
        return;
    }

    //lexer configuration
    int id = turbo::call(scintilla, SCI_GETLEXER, 0, 0);
    std::cerr << "Id before setilexer " << id << "\n";
    turbo::call(scintilla, SCI_SETILEXER, 0, (sptr_t) lexer);
    id = turbo::call(scintilla, SCI_GETLEXER, 0, 0);
    std::cerr << "Id after setilexer " << id << "\n";
    turbo::call(scintilla, SCI_COLOURISE, 0, -1);


    switch (language) {
        case Language::CPP:
            for(const auto &s : stylesC) {
                TColorAttr color = { s[2], s[3] };
                turbo::setStyleColor(scintilla, s[0], color);
            }
            for(const auto &k : keywordsC)
                turbo::call(scintilla, SCI_SETKEYWORDS, k.first, (sptr_t)k.second.c_str());
            for(const auto &p : propertiesC)
                turbo::call(scintilla, SCI_SETPROPERTY, (sptr_t)p.first.c_str(), (sptr_t)p.second.c_str());
            break;

        case Language::Python:
            for(const auto &s : stylesPython) {
                TColorAttr color = { s[1], s[2] };
                turbo::setStyleColor(scintilla, s[0], color);
            }
            for(const auto &k : keywordsPython)
                turbo::call(scintilla, SCI_SETKEYWORDS, k.first, (sptr_t)k.second.c_str());
            for(const auto &p : propertiesPython)
                turbo::call(scintilla, SCI_SETPROPERTY, (sptr_t)p.first.c_str(), (sptr_t)p.second.c_str());
            break;

        case Language::JSON:
            for(const auto &s : stylesJSON) {
                TColorAttr color = { s[1], s[2] };
                turbo::setStyleColor(scintilla, s[0], color);
            }
            for(const auto &k : keywordsJSON)
                turbo::call(scintilla, SCI_SETKEYWORDS, k.first, (sptr_t)k.second.c_str());
            for(const auto &p : propertiesJSON)
                turbo::call(scintilla, SCI_SETPROPERTY, (sptr_t)p.first.c_str(), (sptr_t)p.second.c_str());
            break;

        case Language::Bash:
            for(const auto &s : stylesBash) {
                TColorAttr color = { s[1], s[2] };
                turbo::setStyleColor(scintilla, s[0], color);
            }
            for(const auto &k : keywordsBash)
                turbo::call(scintilla, SCI_SETKEYWORDS, k.first, (sptr_t)k.second.c_str());
            break;

        case Language::Pascal:
            for(const auto &s : stylesPascal) {
                TColorAttr color = { s[1], s[2] };
                turbo::setStyleColor(scintilla, s[0], color);
            }
            for(const auto &k : keywordsPascal)
                turbo::call(scintilla, SCI_SETKEYWORDS, k.first, (sptr_t)k.second.c_str());
            break;

        default:
            break;
    }
}
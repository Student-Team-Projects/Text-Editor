

//
//
// !!!!!
// DO NOT SAVE THIS FILE BEFORE READING THE WARNING AT THE END
//
//
//
//
//
//
//
//

// Define the standard order in which to include header files
// All platform headers should be included before Scintilla headers
// and each of these groups are then divided into directory groups.

// C standard library
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C++ wrappers of C standard library
#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

// C++ standard library
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <forward_list>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// POSIX
#include <dlfcn.h>
#include <sys/time.h>

// Scintilla headers

// Non-platform-specific headers

// include
/*#include <Platform.h"*/

/*#include "../include/scintilla/ILexer.h"*/
/*#include "../include/scintilla/ILoader.h"*/
/*#include <../include/scintilla/Sci_Position.h>*/
/*#include <../include/scintilla/Scintilla.h>*/
/*#include <../include/scintilla/ScintillaWidget.h>*/

/*#include <SciLexer.h>*/

/*// lexlib*/
/*#include <Accessor.h>*/
/*#include <CatalogueModules.h>*/
/*#include <CharacterCategory.h>*/
/*#include <CharacterSet.h>*/
/*#include <DefaultLexer.h>*/
/*#include <LexAccessor.h>*/
/*#include <LexerBase.h>*/
/*#include <LexerModule.h>*/
/*#include <LexerNoExceptions.h>*/
/*#include <LexerSimple.h>*/
/*#include <OptionSet.h>*/
/*#include <PropSetSimple.h>*/
/*#include <SparseState.h>*/
/*#include <StringCopy.h>*/
/*#include <StyleContext.h>*/
/*#include <SubStyles.h>*/
/*#include <WordList.h>*/

// src
/*#include <Catalogue.h>*/

/*#include <CallTip.h>*/
/*#include <CaseConvert.h>*/
/*#include <CaseFolder.h>*/
/*#include <CellBuffer.h>*/
/*#include <CharClassify.h>*/
/*#include <ContractionState.h>*/
/*#include <DBCS.h>*/
/*#include <Decoration.h>*/
/*#include <Document.h>*/
/*#include <EditModel.h>*/
/*#include <EditView.h>*/
/*#include <Editor.h>*/
/*#include <ElapsedPeriod.h>*/
/*#include <FontQuality.h>*/
/*#include <Indicator.h>*/
/*#include <IntegerRectangle.h>*/
/*#include <KeyMap.h>*/
/*#include <LineMarker.h>*/
/*#include <MarginView.h>*/
/*#include <Partitioning.h>*/
/*#include <PerLine.h>*/
/*#include <Position.h>*/
/*#include <PositionCache.h>*/
/*#include <RESearch.h>*/
/*#include <RunStyles.h>*/
/*#include <Selection.h>*/
/*#include <SparseVector.h>*/
/*#include <SplitVector.h>*/
/*#include <Style.h>*/
/*#include <UniConversion.h>*/
/*#include <UniqueString.h>*/
/*#include <ViewStyle.h>*/
/*#include <XPM.h>*/

/*#include <AutoComplete.h>*/
/*#include <ScintillaBase.h>*/

/*#include <ExternalLexer.h>*/

// WARNING!!!!!
// THESE ARE INCLUDED IN VERY SPECIFIC ORDER
// REARANGE THEM AND IT WILL BREAK
// D O   N O T   U S E   C L A N G   F O R M A T   O N   T H I S   F I L E
// THESE WERE SORTED   M A N U A L L Y   IN O(n^2)
// </WARNING>

#include <../include/scintilla/Scintilla.h>
#include <../include/scintilla/CaseConvert.h>
#include <../include/scintilla/CaseFolder.h>
#include <../include/scintilla/CharClassify.h>
#include <../include/scintilla/CharacterCategoryMap.h>
#include <../include/scintilla/CharacterType.h>
#include <../include/scintilla/DBCS.h>
#include <../include/scintilla/Debugging.h>
#include <../include/scintilla/ElapsedPeriod.h>
#include <../include/scintilla/Geometry.h>
#include <../include/scintilla/ILexer.h>
#include <../include/scintilla/ILoader.h>
#include <../include/scintilla/Position.h>
#include <../include/scintilla/RESearch.h>
#include <../include/scintilla/Sci_Position.h>
#include <../include/scintilla/ScintillaMessages.h>
#include <../include/scintilla/ScintillaTypes.h>
#include <../include/scintilla/ScintillaWidget.h>
#include <../include/scintilla/Selection.h>
#include <../include/scintilla/SplitVector.h>
#include <../include/scintilla/UniConversion.h>
#include <../include/scintilla/UniqueString.h>
#include <../include/scintilla/CellBuffer.h>
#include <../include/scintilla/ContractionState.h>
#include <../include/scintilla/KeyMap.h>
#include <../include/scintilla/Partitioning.h>
#include <../include/scintilla/PerLine.h>
#include <../include/scintilla/Platform.h>
#include <../include/scintilla/RunStyles.h>
#include <../include/scintilla/ScintillaCall.h>
#include <../include/scintilla/ScintillaStructures.h>
#include <../include/scintilla/SparseVector.h>
#include <../include/scintilla/Style.h>
#include <../include/scintilla/UndoHistory.h>
#include <../include/scintilla/XPM.h>
#include <../include/scintilla/AutoComplete.h>
#include <../include/scintilla/CallTip.h>
#include <../include/scintilla/ChangeHistory.h>
#include <../include/scintilla/Decoration.h>
#include <../include/scintilla/Document.h>
#include <../include/scintilla/Indicator.h>
#include <../include/scintilla/LineMarker.h>
#include <../include/scintilla/ViewStyle.h>
#include <../include/scintilla/PositionCache.h>
#include <../include/scintilla/EditModel.h>
#include <../include/scintilla/MarginView.h>
#include <../include/scintilla/EditView.h>
#include <../include/scintilla/Editor.h>
#include <../include/scintilla/ScintillaBase.h>

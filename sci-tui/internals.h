/*#include <cstddef>*/
/*#include <cstdlib>*/
/*#include <cstdint>*/
/*#include <cstring>*/
/*#include <cstdio>*/
/*#include <cmath>*/

/*#include <string>*/
/*#include <string_view>*/
/*#include <vector>*/
/*#include <map>*/
/*#include <optional>*/
/*#include <algorithm>*/
/*#include <memory>*/
/*#include <sstream>*/

/*#include "../scintilla/../scintilla/include/ScintillaTypes.h"*/
/*#include "../scintilla/../scintilla/include/ScintillaMessages.h"*/

/*#include "../scintilla/../scintilla/src/Debugging.h"*/
/*#include "../scintilla/../scintilla/src/Geometry.h"*/
/*#include "../scintilla/../scintilla/src/Platform.h"*/

/*#include "../scintilla/../scintilla/include/Scintilla.h"*/
/*#include "../scintilla/../scintilla/include/ScintillaWidget.h"*/
/*#include "../scintilla/../scintilla/src/CharacterType.h"*/
/*#include "../scintilla/../scintilla/src/XPM.h"*/
/*#include "../scintilla/../scintilla/src/UniConversion.h"*/
/*#include "../scintilla/../scintilla/src/Platform.h"*/
/*#include "../scintilla/../scintilla/src/Editor.h"*/

/*using namespace Scintilla;*/
/*using namespace Scintilla::Internal;*/


#ifndef TURBO_SCINTILLA_INTERNALS_H
#define TURBO_SCINTILLA_INTERNALS_H

// Define the standard order in which to include header files
// All platform headers should be included before Scintilla headers
// and each of these groups are then divided into directory groups.

// C standard library
#include <string.h>
#include <assert.h>

// C++ wrappers of C standard library

// C++ standard library
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <forward_list>
#include <algorithm>
#include <memory>
#include <chrono>
#include <set>
#include <optional>
#include <array>

// Scintilla headers

// Non-platform-specific headers

// include
#include "../scintilla/include/Sci_Position.h"
#include "../scintilla/include/ILoader.h"
#include "../scintilla/include/ILexer.h"
#include "../scintilla/include/Scintilla.h"
#include "../scintilla/include/ScintillaMessages.h"
#include "../scintilla/include/ScintillaTypes.h"
#include "../scintilla/include/ScintillaWidget.h"
#include "../scintilla/include/ScintillaCall.h"
#include "../scintilla/include/ScintillaStructures.h"

// lexlib
/*#include "../scintilla/lexlib/StringCopy.h"*/
/*#include "../scintilla/lexlib/PropSetSimple.h"*/
/*#include "../scintilla/lexlib/WordList.h"*/
/*#include "../scintilla/lexlib/LexAccessor.h"*/
/*#include "../scintilla/lexlib/Accessor.h"*/
/*#include "../scintilla/lexlib/StyleContext.h"*/
/*#include "../scintilla/lexlib/CharacterSet.h"*/
/*#include "../scintilla/lexlib/CharacterCategory.h"*/
/*#include "../scintilla/lexlib/LexerModule.h"*/
/*#include "../scintilla/lexlib/CatalogueModules.h"*/
/*#include "../scintilla/lexlib/OptionSet.h"*/
/*#include "../scintilla/lexlib/SparseState.h"*/
/*#include "../scintilla/lexlib/SubStyles.h"*/
/*#include "../scintilla/lexlib/DefaultLexer.h"*/
/*#include "../scintilla/lexlib/LexerBase.h"*/
/*#include "../scintilla/lexlib/LexerSimple.h"*/
/*#include "../scintilla/lexlib/LexerNoExceptions.h"*/

// src
#include "../scintilla/src/Debugging.h"
#include "../scintilla/src/Position.h"
#include "../scintilla/src/UniqueString.h"
#include "../scintilla/src/SplitVector.h"
#include "../scintilla/src/Partitioning.h"
#include "../scintilla/src/RunStyles.h"
#include "../scintilla/src/SparseVector.h"
#include "../scintilla/src/ContractionState.h"
#include "../scintilla/src/CellBuffer.h"
#include "../scintilla/src/PerLine.h"
#include "../scintilla/src/KeyMap.h"
#include "../scintilla/src/CharClassify.h"
#include "../scintilla/src/Decoration.h"
#include "../scintilla/src/CaseFolder.h"
#include "../scintilla/src/CaseConvert.h"
#include "../scintilla/src/UniConversion.h"
#include "../scintilla/src/DBCS.h"
#include "../scintilla/src/Selection.h"
#include "../scintilla/src/ElapsedPeriod.h"
#include "../scintilla/src/ChangeHistory.h"
#include "../scintilla/src/CharacterType.h"
#include "../scintilla/src/CharacterCategoryMap.h"
#include "../scintilla/src/Geometry.h"
#include "../scintilla/src/UndoHistory.h"
#include "../scintilla/src/Document.h"
#include "../scintilla/src/Platform.h"
#include "../scintilla/src/CallTip.h"
#include "../scintilla/src/Indicator.h"
#include "../scintilla/src/XPM.h"
#include "../scintilla/src/LineMarker.h"
#include "../scintilla/src/Style.h"
#include "../scintilla/src/AutoComplete.h"
#include "../scintilla/src/ViewStyle.h"
#include "../scintilla/src/PositionCache.h"
#include "../scintilla/src/EditModel.h"
#include "../scintilla/src/MarginView.h"
#include "../scintilla/src/RESearch.h"
#include "../scintilla/src/EditView.h"
#include "../scintilla/src/Editor.h"
#include "../scintilla/src/ScintillaBase.h"


#endif // TURBO_SCINTILLA_INTERNALS_H

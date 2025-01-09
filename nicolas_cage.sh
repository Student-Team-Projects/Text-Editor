
for a in $(ls include/scintilla); do
    echo "#include <../include/scintilla/$a>"
done
